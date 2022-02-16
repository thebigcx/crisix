#include <sys/stivale2.h>
#include <sys/platform.h>
#include <sys/arch.h>

// TEMP
#include <sys/heap.h>
#include <sys/debug.h>

static char __stack[4096];

SECTION(".stivale2hdr") USED
static struct st2_header st2hdr = {
    .entry = 0,
    .stack = (uintptr_t)__stack + sizeof(__stack),
    .flags = 0,
    .tags  = 0
};

void kmain_st2(struct st2_struct *st2)
{
    (void)st2;

    arch_init();

    void *ptr = malloc(0x10);
    ptr = malloc(0x10);
    ptr = malloc(0x10);
    ptr = malloc(0x10);
    dbgprintf("ptr: %d\n", ptr);

    *((uint32_t*)0xb8000) = 0x2f4b2f4f;
    for (;;);
}
