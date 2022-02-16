#include <arch/lapic.h>
#include <arch/mmu.h>
#include <arch/cpufunc.h>

// Local APIC registers
#define R_ID          0x020
#define R_VERS        0x030
#define R_TPR         0x080
#define R_APR         0x090
#define R_PPR         0x0a0
#define R_EOI         0x0b0
#define R_RRD         0x0c0
#define R_LDR         0x0d0
#define R_DFR         0x0e0
#define R_SIVR        0x0f0
#define R_ISR         0x100
#define R_TMR         0x180
#define R_IRR         0x200
#define R_ERR         0x280
#define R_ICRLO       0x300
#define R_ICRHI       0x310

#define R_LVT_TIME    0x320
#define R_LVT_THERM   0x330
#define R_LVT_PERF    0x340
#define R_LVT_LINT0   0x350
#define R_LVT_LINT1   0x360
#define R_LVT_ERR     0x370

#define R_TIME_INIT   0x380
#define R_TIME_CURR   0x390
#define R_TIME_DIVCFG 0x3e0

#define ICR_IDLE      0x0000
#define ICR_SEND_PEND 0x1000

#define ICR_DST_SHFT  24

#define DELIV_INIT   0x00500
#define DELIV_STRTUP 0x00600

#define LVL_DEASSRT  0x00000
#define LVL_ASSRT    0x04000

static volatile uintptr_t mmio_base;

static uintptr_t get_base()
{
    return rdmsr(0x1b);
}

static void set_base(uintptr_t base)
{
    wrmsr(0x1b, base);
}

static void write(uint32_t off, uint32_t val)
{
    *((volatile uint32_t*)(mmio_base + off)) = val;
}

static uint32_t read(uint32_t off)
{
    return *((volatile uint32_t*)(mmio_base + off));
}

// Write to the Interrupt-Control-Register
static void write_icr(uint8_t id, uint32_t flags)
{
    write(R_ICRHI, (uint32_t)id << ICR_DST_SHFT);
    write(R_ICRLO, flags);

    while (read(R_ICRLO) & ICR_SEND_PEND);
}

// Send an Inter-Processor Interrupt
void send_ipi(uint8_t id, uint8_t vec, uint32_t sh)
{
    write_icr(id, vec | sh | LVL_ASSRT);
}

// Send Start-Up IPI
void send_strt_ipi(uint8_t id)
{
    write_icr(id, DELIV_STRTUP | LVL_ASSRT);
}

// Send INIT Level De-assert IPI
void send_init_ipi(uint8_t id)
{
    write_icr(id, DELIV_INIT);
}

// Send an End-Of-Interrupt to the LAPIC
void send_eoi()
{
    write(R_EOI, 0);
}

// Setup LAPIC for the BSP (map the MMIO)
void lapic_setup()
{
    mmio_base = (uintptr_t)vmalloc(1);
    vkmmap((void*)mmio_base, (void*)get_base(), 1, MMU_PR | MMU_RW | MMU_NOCACHE | MMU_WTHRU);
    
    lapic_enable();
}

void lapic_enable()
{
    set_base(get_base() | (1 << 11));
    write(R_SIVR, read(R_SIVR) | 0x1ff);
}

void intr_init()
{
    lapic_setup();
}
