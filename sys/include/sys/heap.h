#pragma once

#include <sys/types.h>

void *malloc(size_t size);
void *calloc(size_t nitems, size_t size);
void free(void *ptr);
