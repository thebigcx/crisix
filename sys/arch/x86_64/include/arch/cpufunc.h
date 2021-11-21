#pragma once

#include <sys/platform.h>

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
