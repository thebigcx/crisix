#include <arch/arch.h>
#include <arch/pic.h>

#define REGSEL 0x00
#define IOWIN  0x10

#define R_IOAPICID  0
#define R_IOAPICVER 1

// Redirection table entry
#define REDTBL(irq) (2 * irq + 0x10)

static uintptr_t s_pbase = 0;
static volatile uintptr_t s_mmio;

uint32_t read32(uint32_t reg)
{
    *(volatile uint32_t*)(s_mmio + REGSEL) = reg;
    return *(volatile uint32_t*)(s_mmio + IOWIN);
}

void write32(uint32_t reg, uint32_t val)
{
    *(volatile uint32_t*)(s_mmio + REGSEL) = reg;
    *(volatile uint32_t*)(s_mmio + IOWIN)  = val;
}

void write64(uint32_t reg, uint64_t val)
{
    // Write two 32-bit values
    write32(reg, val & 0xffffffff);
    write32(reg + 1, val >> 32);
}

static void redirect(uint8_t irq, uint8_t vec)
{
    write64(REDTBL(irq), (uint32_t)vec);
}

void ioapic_setbase(uintptr_t base)
{
    s_pbase = base;
}

void ioapic_init()
{
    s_mmio = (uintptr_t)vmalloc(1);
    vkmmap((void*)s_mmio, (void*)s_pbase, 1, MMU_PR | MMU_RW | MMU_WTHRU | MMU_NOCACHE);

    pic_disable();
    
    redirect(2, 32);
}
