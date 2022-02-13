#include <sys/stivale2.h>
#include <sys/platform.h>
#include <sys/arch.h>

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

    *((uint32_t*)0xb8000) = 0x2f4b2f4f;
    for (;;);
}
