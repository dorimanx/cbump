// Copyright (C) 2015, Paulo Roberto.

#ifndef CBUMP_LIB_H
#define CBUMP_LIB_H

void perr(const char *str, ...);

void pad_bits(size_t target, size_t mask, size_t *dst);

int get_size(int fd, off_t *size);

int detect_partition(const char *ptr);

size_t str_last_index(const char *ptr, int val);

char *str_get_file(const char *ptr);

char *str_get_path(const char *ptr);

char *append_c_out(const char *str);

#endif //CBUMP_LIB_H
