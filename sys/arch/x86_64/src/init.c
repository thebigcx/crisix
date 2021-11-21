#include <sys/arch.h>

void arch_init()
{
    init_cpu_descs(&g_cpus[0]);
}
