#include <sys/arch.h>

void arch_init()
{
    init_cpu_descs(&g_cpus[0]);
    kmap_init();

    void *ptr = vmalloc(16);
    
    vfree(ptr, 16);
}
