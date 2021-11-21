#pragma once

#include <sys/platform.h>
#include <sys/types.h>

FORCE_INLINE void memcpy(void *dst, const void *src, size_t n)
{
    while (n--) *((uint8_t*)dst++) = *((uint8_t*)src++);
}

FORCE_INLINE void memset(void *dst, uint8_t c, size_t n)
{
    while (n--) *((uint8_t*)dst++) = c;
}
