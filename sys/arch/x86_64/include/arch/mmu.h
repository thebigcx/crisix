#pragma once

#include <sys/types.h>
#include <sys/platform.h>

#define KBASE 0xffffffff80000000

// Standard page size (4K)
#define PAGE_SIZE 0x1000

// Larger page sizes
#define PAGE2M    0x200000
#define PAGE1G    0x40000000

#define PAGEALIGN ALIGN(PAGE_SIZE)

#define PMLCNT 512

#define MMU_PR      (1 << 0)
#define MMU_RW      (1 << 1)
#define MMU_USR     (1 << 2)
#define MMU_WTHRU   (1 << 3)
#define MMU_NOCACHE (1 << 4)
#define MMU_ACC     (1 << 5)
#define MMU_SZ      (1 << 7)

typedef uint64_t page_t;
typedef page_t pml_t[PMLCNT];

struct vm_map
{
    
};

void kmap_init();

void* vmalloc(size_t n);
void vfree(void *ptr, size_t n);
