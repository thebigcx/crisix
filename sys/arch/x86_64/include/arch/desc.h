#pragma once

#include <sys/types.h>
#include <sys/platform.h>

#define GDT_CODE0   0x08
#define GDT_DATA0   0x10
#define GDT_CODE3   0x18
#define GDT_DATA3   0x20
#define GDT_TSS     0x28
#define GDT_TSS2    0x30
#define GDT_LEN     7

// Interrupt Descriptor Table entry
struct PACKED idt
{
    uint16_t offlo;
    uint16_t select;
    uint8_t  ist;

    // Type Attributes
    struct
    {
        uint8_t type    : 4;
        uint8_t store   : 1;
        uint8_t dpl     : 2;
        uint8_t present : 1;
    } type_attr;

    uint16_t offmid;
    uint32_t offhi;
    uint32_t zero;
};

// Descriptor record
struct PACKED desc
{
    uint16_t lim;
    uint64_t base;
};

// Global Descriptor Table entry
union PACKED gdt
{
    struct PACKED
    {
        uint16_t limlo;
        uint16_t baselo;
        uint8_t  basemid;
        uint32_t type       : 4;
        uint32_t dtype      : 1;
        uint32_t dpl        : 2;
        uint32_t present    : 1;
        uint32_t limhi      : 4;
        uint32_t avail      : 1;
        uint32_t size64     : 1;
        uint32_t size32     : 1;
        uint32_t gran       : 1;
        uint8_t  basehi;
    };
    struct PACKED
    {
        uint32_t low;
        uint32_t high;
    };
};

// Task State Segment
struct PACKED tss
{
    uint32_t res1;
    uint64_t rsp[3];
    uint64_t res2;
    uint64_t ist[7];
    uint64_t res3;
    uint16_t res4;
    uint16_t iomap;
};

struct cpu;

void init_cpu_descs(struct cpu *cpu);
