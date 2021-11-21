#pragma once

#include <arch/desc.h>

struct cpu
{
    union gdt gdt[GDT_LEN];
    struct tss tss;
};

extern struct cpu g_cpus[64];
extern unsigned int g_cpucnt;
