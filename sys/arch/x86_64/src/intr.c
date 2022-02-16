#include <arch/arch.h>

// TEMP
#include <arch/pic.h>

void irq_handler()
{
    //send_eoi();
    pic_eoi();    
}
