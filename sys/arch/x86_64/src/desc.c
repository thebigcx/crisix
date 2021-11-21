#include <arch/desc.h>
#include <arch/cpu.h>
#include <arch/cpufunc.h>

#include <sys/libk.h>

static union gdt s_gdt[GDT_LEN] = {
    { .low = 0,      .high = 0        }, // Null entry
    { .low = 0xffff, .high = 0xaf9800 }, // GDT_CODE0
    { .low = 0xffff, .high = 0xaf9300 }, // GDT_DATA0
    { .low = 0xffff, .high = 0xaff800 }, // GDT_CODE3
    { .low = 0xffff, .high = 0xaff200 }, // GDT_DATA3
    { .low = 0,      .high = 0xe900   }, // GDT_TSS
    { .low = 0,      .high = 0        }  // GDT_TSS2
};

static void init_gdt(union gdt gdt[GDT_LEN], struct tss *tss)
{
    memcpy(gdt, s_gdt, sizeof(s_gdt));

    gdt[5].baselo  = (uintptr_t)tss;
    gdt[5].basemid = (uintptr_t)tss >> 16;
    gdt[5].basehi  = (uintptr_t)tss >> 24;
    gdt[5].limlo   = (sizeof(struct tss) - 1);
    gdt[5].limhi   = (sizeof(struct tss) - 1) >> 16;

    gdt[6].low  = (uintptr_t)tss >> 32;
    gdt[6].high = 0;

    memset(tss, 0, sizeof(struct tss));
    tss->iomap = sizeof(struct tss);

}

void init_cpu_descs(struct cpu *cpu)
{
    init_gdt(cpu->gdt, &cpu->tss);

    struct desc desc = {
        .lim = sizeof(cpu->gdt) - 1,
        .base = (uintptr_t)cpu->gdt
    };

    lgdt_full(&desc, GDT_CODE0, GDT_DATA0);
    ltr(GDT_TSS | 3);
}

