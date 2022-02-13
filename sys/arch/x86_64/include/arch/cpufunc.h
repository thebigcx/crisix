#pragma once

#include <sys/platform.h>

struct desc;

void lgdt_full(struct desc*, uint16_t, uint16_t);

FORCE_INLINE void ltr(uint16_t tr)
{
    asm volatile ("ltr %0" :: "r"(tr));
}

struct desc;

FORCE_INLINE void lgdt(struct desc *desc)
{
    asm volatile ("lgdt (%0)" :: "r"(desc));
}

FORCE_INLINE void lidt(struct desc *desc)
{
    asm volatile ("lidt (%0)" :: "r"(desc));
}

FORCE_INLINE void lcr3(uintptr_t cr3)
{
    asm volatile ("mov %0, %%cr3" :: "r"(cr3));
}

FORCE_INLINE uintptr_t rdmsr(unsigned int msr)
{
    uint32_t lo, hi;
    asm volatile ("rdmsr" : "=d"(hi), "=a"(lo) : "c"(msr));
    return ((uintptr_t)hi << 32) | (uintptr_t)lo;
}

FORCE_INLINE void wrmsr(unsigned int msr, uintptr_t val)
{
    asm volatile ("wrmsr" :: "a"(val & 0xffffffff), "d"(val >> 32), "c"(msr));
}
