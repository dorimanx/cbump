// Copyright (C) 2015, Paulo Roberto.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "lib.h"

/* Default stderr */
void
perr(const char *str, ...) {
    char a[1024];
    va_list args;
    int err = errno;

    va_start(args, str);
    vsnprintf(a, sizeof(a), str, args);
    va_end(args);

    if (err != 0) {
        perror(a);
    }
    else {
        printf("%s\n", a);
    }

    exit(err);
}

/* Pointer to paged target size */
void
pad_bits(size_t target, size_t mask, size_t *dst) {
    *dst = (target + mask) & ~mask;
}

int
get_size(int fd, off_t *size) {
    struct stat s;

    if (fstat(fd, &s) < 0) {
        return -1;
    }

    *size = s.st_size;

    return 0;
}

int
detect_partition(const char *ptr) {
    if (strstr(ptr, "boot") != NULL || strstr(ptr, "kernel") != NULL) {
        return 1;
    } else if (strstr(ptr, "recovery") != NULL) {
        return 0;
    } else {
        return -1;
    }
}

/* Get last index of an occurrence in a string */
size_t
str_last_index(const char *ptr, int val) {
    char *a;
    size_t index;

    a = strrchr(ptr, val);

    if (!a) return 0;

    index = (size_t) (a - ptr + 1);

    return index;
}

/* Get file from file path */
char *
str_get_file(const char *ptr) {
    char *a;
    size_t ptr_sz = strlen(ptr);
    size_t index;

    if (!(index = str_last_index(ptr, '/'))) {
        return NULL;
    }

    if (!(a = calloc((ptr_sz - index) + 1, sizeof(char)))) {
        return NULL;
    }

    strncpy(a, &ptr[index], ptr_sz - index);

    return a;
}

/* Get path from file path */
char *
str_get_path(const char *ptr) {
    char *a;
    const char *b = str_get_file(ptr);
    size_t ptr_sz = strlen(ptr);
    size_t index;

    if (!(index = str_last_index(ptr, '/'))) {
        return NULL;
    }

    if (!(a = calloc((ptr_sz - strlen(b)) + 1, sizeof(char)))) {
        return NULL;
    }

    strncpy(a, &ptr[0], index - 1);

    return a;
}

/* Append 'c' prefix to an file or file path */
char *
append_c_out(const char *str) {
    char *a, *f, *p;

    size_t str_sz = strlen(str);

    if (!(a = calloc(str_sz + 2, sizeof(char)))) {
        return NULL;
    }

    size_t str_b_index = str_last_index(str, '/');

    if (!str_b_index) {
        strcpy(a, "c");
        strcat(a, str);

        return a;
    }

    const char *a_f = str_get_file(str);
    const char *a_p = str_get_path(str);

    if (!(f = calloc((str_sz - str_b_index) + 2, sizeof(char)))) {
        return NULL;
    }

    if (!(p = calloc(str_b_index + 1, sizeof(char)))) {
        return NULL;
    }

    strcpy(f, "c");
    strcat(f, a_f);
    strcpy(p, a_p);
    strcpy(a, p);
    strcat(a, "/");
    strcat(a, f);

    return a;
}
