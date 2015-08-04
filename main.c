// Copyright (C) 2015, Paulo Roberto.

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "main.h"
#include "lib.h"

#ifdef ANDROID
#define P_ADDR "/dev/block/platform/msm_sdcc.1/by-name"
#endif
#define KEY "\x41\xa9\xe4\x67\x74\x4d\x1d\x1b\xa4\x29\xf2\xec\xea\x65\x52\x79"

int
check_key(int fd, off_t size) {
    uint8_t key_buf[16];
    int i, r = 1;

    for (i = 0; i <= size; i += 16) {
        lseek(fd, size - i, SEEK_SET);
        read(fd, key_buf, 16);
        if (!memcmp(key_buf, KEY, 16)) {
            r = 0; break;
        }
    }

    return r;
}

#ifdef ANDROID
void
flash_partition(const char *in, int p) {
    int ifd, ofd;
    void *ifd_buffer;
    off_t ifd_size;
    char buf[64];

    sprintf(buf, "%s/%s", P_ADDR, p ? "boot" : "recovery");

    if ((ifd = open(in, O_RDONLY)) < 0) {
        perr("Failed to open %s", in);
    }

    if ((ofd = open(buf, O_WRONLY)) < 0) {
        perr("Failed to open partition block");
    }

    get_size(ifd, &ifd_size);

    ifd_buffer = mmap(0, (size_t) ifd_size, PROT_READ, MAP_PRIVATE, ifd, 0);

    if (ifd_buffer == MAP_FAILED) {
        perr("Failed to map file in memory");
    }

    if (check_key(ifd, ifd_size)) {
        perr("File %s is not bumped", in);
    }

    if (write(ofd, ifd_buffer, (size_t) ifd_size) != ifd_size) {
        perr("Failed to write %s to output partition", in);
    }

    close(ifd);
    close(ofd);
}
#endif

void
bump(const char *in, const char *out) {
    boot_img_hdr *hdr;
    int ifd, ofd;
    off_t ifd_size;
    void *ifd_buffer;
    size_t page_mask, paged_kernel, paged_ramdisk;
    ssize_t total_size;

    if ((ifd = open(in, O_RDONLY)) < 0) {
        perr("Failed to open %s", in);
    }

    if ((ofd = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        perr("Failed to create output for writing");
    }

    get_size(ifd, &ifd_size);

    ifd_buffer = mmap(0, (size_t) ifd_size, PROT_READ, MAP_PRIVATE, ifd, 0);

    if (ifd_buffer == MAP_FAILED) {
        perr("Failed to map file to memory");
    }

    if (!check_key(ifd, ifd_size)) {
        printf("File is already bumped, Copying readed data to %s\n", out);
        if (write(ofd, ifd_buffer, (size_t) ifd_size) != ifd_size) {
            perr("Failed to write file to output");
        }
        return;
    }

    hdr = ifd_buffer;

    page_mask = hdr->page_size - 1;
    pad_bits(hdr->kernel_size, page_mask, &paged_kernel);
    pad_bits(hdr->ramdisk_size, page_mask, &paged_ramdisk);

    total_size = hdr->page_size + paged_kernel + paged_ramdisk + hdr->second_size + hdr->dt_size;

    if (write(ofd, ifd_buffer, total_size) != total_size) {
        perr("Failed to write file to output");
    }

    if (write(ofd, KEY, 16) != 16) {
        perr("Failed to write KEY to output");
    }

    close(ifd);
    close(ofd);
}

int
main(int argc, char *argv[]) {
    if (argc == 2) {
        bump(argv[1], append_c_out(argv[1]));
    } else if (argc == 3) {
#ifdef ANDROID
        if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "--flash")) {
            int i = detect_partition(argv[2]);
            if (i < 0) perr("Can't detect partition by file name");
            flash_partition(argv[2], i);
        } else if (!strcmp(argv[1], "-a") || !strcmp(argv[1], "--avexado")) {
            int i = detect_partition(argv[2]);
            if (i < 0) perr("Can't detect partition by file name");
            bump(argv[2], append_c_out(argv[2]));
            flash_partition(append_c_out(argv[2]), i);
        } else {
            bump(argv[1], argv[2]);
        }
#else
        bump(argv[1], argv[2]);
#endif
    } else {
        printf("usage: cbump "
#ifdef ANDROID
               "[-f|--flash] [-a|--avexado] "
#endif
               "[stdin] [stdin]\n\n"
               "[stdin] : Bump file if not bumped, output is writed with 'c' prefix.\n"
#ifdef ANDROID
              "[-f|--flash] : Check if file is bumped and flash.\n"
              "[-a|--avexado] : Check if file is bumped, bump file if not and flash.\n"
#else
              "[stdin] [stdin] : Same as first, but you define output.\n");
#endif

#ifdef ANDROID
              "Cbump indentify partition by file name, (e.g, If you use"
              " -f with a recovery.img file, cbump flash as recovery).\n");
#endif
    }

    return 0;
}