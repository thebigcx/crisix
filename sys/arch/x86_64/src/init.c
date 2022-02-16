#include <sys/arch.h>
#include <sys/debug.h>

// TEMP
#include <arch/ioapic.h>
#include <arch/pic.h>

void arch_init()
{
    init_cpu_descs(&g_cpus[0]);
    kmap_init();
    init_pmm();

    //intr_init();
    //ioapic_init(0xfec00000);
    pic_init();

    /*void *ptr = vmalloc(16);
    
    vfree(ptr, 16);

    void *phys = pmalloc(16);
    dbgprintf("Phys: %d\n", phys);
    
    vkmmap(ptr, phys, 16, MMU_PR);

    dbgprintf("ptr: %d\n", ptr);
    *((uint32_t*)ptr) = 0xdeadbeef;

    void *phys2 = pmalloc(16);
    dbgprintf("Phys: %d\n", phys2);*/
    //send_eoi();
    

    asm ("sti");
    //for(;;);
}
