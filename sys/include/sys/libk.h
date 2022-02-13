#pragma once

#include <sys/platform.h>
#include <sys/types.h>

typedef __builtin_va_list va_list;

#define va_start(v, l)  __builtin_va_start(v, l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v, l)    __builtin_va_arg(v, l)
#define va_copy(d, s)   __builtin_va_copy(d, s)

FORCE_INLINE void memcpy(void *dst, const void *src, size_t n)
{
    while (n--) *((uint8_t*)dst++) = *((uint8_t*)src++);
}

FORCE_INLINE void memset(void *dst, uint8_t c, size_t n)
{
    while (n--) *((uint8_t*)dst++) = c;
}

FORCE_INLINE size_t strlen(const char *str)
{
    for (size_t len = 0;; len++) if (str[len] == 0) return len;
}