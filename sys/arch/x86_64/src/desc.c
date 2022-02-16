#include <arch/desc.h>
#include <arch/cpu.h>
#include <arch/cpufunc.h>
#include <arch/idtstubs.h>

#include <sys/libk.h>

#define IDT_INTR 0xe
#define IDT_TRAP 0xf

static struct idt s_idt[256];
static int s_idtinit = 0;

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

// Make IDT Entry
static struct idt mkidtent(void (*handler)(), int user, int type)
{
    return (struct idt)
    {
        .offlo = (uintptr_t)handler & 0xffff,
        .select = GDT_CODE0,
        .ist = 0,
        .type_attr =
        {
            .type = type,
            .store = 0,
            .dpl = user ? 3 : 0,
            .present = 1
        },
        .offmid = ((uintptr_t)handler >> 16) & 0xffff,
        .offhi = ((uintptr_t)handler >> 32) & 0xffffffff,
        .zero = 0
    };
}

#define MKISRINTR(n, u) s_idt[n] = mkidtent(isr##n, u, IDT_INTR)
#define MKISRTRAP(n, u) s_idt[n] = mkidtent(isr##n, u, IDT_TRAP)
#define MKIRQ(n, u) s_idt[n + 32] = mkidtent(irq##n, u, IDT_INTR)

// Initialize static IDT
static void init_idt()
{
    if (s_idtinit) return;

    MKISRINTR(0, 0);
    MKISRTRAP(1, 1);
    MKISRINTR(2, 0);
    MKISRTRAP(3, 1);
    MKISRTRAP(4, 0);
    MKISRINTR(5, 0);
    MKISRINTR(6, 0);
    MKISRINTR(7, 0);
    MKISRINTR(8, 0);
    MKISRINTR(9, 0);
    MKISRINTR(10, 0);
    MKISRINTR(11, 0);
    MKISRINTR(12, 0);
    MKISRINTR(13, 0);
    MKISRINTR(14, 0);
    MKISRINTR(15, 0);
    MKISRINTR(16, 0);
    MKISRINTR(17, 0);
    MKISRINTR(18, 0);
    MKISRINTR(19, 0);
    MKISRINTR(20, 0);
    MKISRINTR(21, 0);
    MKISRINTR(22, 0);
    MKISRINTR(23, 0);
    MKISRINTR(24, 0);
    MKISRINTR(25, 0);
    MKISRINTR(26, 0);
    MKISRINTR(27, 0);
    MKISRINTR(28, 0);
    MKISRINTR(29, 0);
    MKISRINTR(30, 0);
    MKISRINTR(31, 0);

    MKIRQ(0, 0);
    MKIRQ(1, 0);
    MKIRQ(2, 0);
    MKIRQ(3, 0);
    MKIRQ(4, 0);
    MKIRQ(5, 0);
    MKIRQ(6, 0);
    MKIRQ(7, 0);
    MKIRQ(8, 0);
    MKIRQ(9, 0);
    MKIRQ(10, 0);
    MKIRQ(11, 0);
    MKIRQ(12, 0);
    MKIRQ(13, 0);
    MKIRQ(14, 0);
    MKIRQ(15, 0);

    s_idtinit = 1;
}

// Load (and initalize if necessary) CPU descriptor registers
void init_cpu_descs(struct cpu *cpu)
{
    init_gdt(cpu->gdt, &cpu->tss);

    struct desc desc = {
        .lim = sizeof(cpu->gdt) - 1,
        .base = (uintptr_t)cpu->gdt
    };

    lgdt_full(&desc, GDT_CODE0, GDT_DATA0);
    ltr(GDT_TSS | 3);

    init_idt();

    desc = (struct desc) {
        .lim = sizeof(s_idt) - 1,
        .base = (uintptr_t)s_idt
    };

    lidt(&desc);
}

