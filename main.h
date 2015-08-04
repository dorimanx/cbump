// Copyright (C) 2015, Paulo Roberto.

// This header contains code from Google's Android platform system core,
// Copyright (C) The Android Open Source Project

#ifndef CBUMP_MAIN_H
#define CBUMP_MAIN_H

#include <stdint.h>

typedef struct boot_img_hdr boot_img_hdr;

#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_ARGS_SIZE 512

struct boot_img_hdr {
    uint8_t magic[BOOT_MAGIC_SIZE];

    uint32_t kernel_size;   /* size in bytes */
    uint32_t kernel_addr;   /* physical load addr */

    uint32_t ramdisk_size;  /* size in bytes */
    uint32_t ramdisk_addr;  /* physical load addr */

    uint32_t second_size;   /* size in bytes */
    uint32_t second_addr;   /* physical load addr */

    uint32_t tags_addr; /* physical addr for kernel tags */
    uint32_t page_size; /* flash page size we assume */
    uint32_t dt_size;   /* device tree in bytes */
    uint32_t unused;    /* future expansion: should be 0 */

    uint8_t name[BOOT_NAME_SIZE];   /* asciiz product name */

    uint8_t cmdline[BOOT_ARGS_SIZE];

    uint32_t id[8]; /* timestamp / checksum / sha1 / etc */
};

#endif //CBUMP_MAIN_H
