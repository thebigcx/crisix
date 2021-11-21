#include <arch/mmu.h>
#include <arch/cpufunc.h>

#include <sys/libk.h>

static pml_t kpml4 PAGEALIGN;
static pml_t kpdpt PAGEALIGN;
static pml_t kpd   PAGEALIGN;

static pml_t kheapd          PAGEALIGN;
static pml_t kheapts[PMLCNT] PAGEALIGN;

#define HEAPBASE 0xffffffffc0000000

// Standard kernel page map level
#define KPML(pml) ((uintptr_t)&pml - KBASE) | MMU_PR | MMU_RW

// Initialize kernel virtual memory map (higher half)
void kmap_init()
{
    memset(&kpml4, 0, sizeof(pml_t));
    memset(&kpdpt, 0, sizeof(pml_t));

    kpml4[511] = KPML(kpdpt);
    kpml4[0]   = kpml4[511];

    kpdpt[510] = KPML(kpd);
    kpdpt[0]   = kpdpt[510];

    for (int i = 0; i < PMLCNT; i++)
        kpd[i] = (PAGE2M * i) | MMU_PR | MMU_RW | MMU_SZ;

    kpdpt[511] = KPML(kheapd);

    for (int i = 0; i < PMLCNT; i++)
        kheapd[i] = KPML(kheapts[i]);

    lcr3((uintptr_t)&kpml4 - KBASE);
}

// First free 'n' heap pages
static uintptr_t first_free(size_t n)
{
    // Number of contiguous free pages
    size_t free = 0;

    for (size_t i = 0; i < PMLCNT; i++)
    for (size_t j = 0; j < PMLCNT; j++)
    {
        if (kheapts[i][j] & MMU_PR)
            free = 0;
        else if (++free == n)
            return i * PMLCNT + j + 1 - free;
    }

    return (uintptr_t)-1;
}

// Allocate virtual memory pages
void *vmalloc(size_t n)
{
    uintptr_t a = first_free(n);
    if (a == (uintptr_t)-1) return NULL;

    for (uintptr_t i = a; i < a + n; i++)
        kheapts[i / 512][i % 512] = MMU_PR;

    return (void*)(a * PAGE_SIZE + 0xffffffffc0000000);
}

// Free virtual memory pages
void vfree(void *ptr, size_t n)
{
    uintptr_t a = ((uintptr_t)ptr - 0xffffffffc0000000) / PAGE_SIZE;

    for (uintptr_t i = a; i < a + n; i++)
        kheapts[i / 512][i % 512] = 0;
}
