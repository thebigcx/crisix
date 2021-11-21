.global lgdt_full
lgdt_full:
    lgdt    (%rdi)
    mov     %dx, %ax
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    mov     %ax, %ss

    movq    %rsp, %rbx

    leaq    .done(%rip), %rax

    pushq   %rdx
    pushq   %rbx
    pushfq
    pushq   %rsi
    pushq   %rax
    iretq

.done:
    ret
