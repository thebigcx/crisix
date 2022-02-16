SRC += arch/x86_64/src/init.c    \
       arch/x86_64/src/arch.c    \
       arch/x86_64/src/desc.c    \
       arch/x86_64/src/asm.s     \
       arch/x86_64/src/mmu.c     \
       arch/x86_64/src/lapic.c   \
       arch/x86_64/src/intrasm.s \
       arch/x86_64/src/ioapic.c  \
       arch/x86_64/src/pic.c     \
       arch/x86_64/src/intr.c    \

INCLUDE := -Iarch/x86_64/include
