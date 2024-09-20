.section ".text.boot"

.global _start
_start:
    mrs x0, MPIDR_EL1
    and x0, x0, #0xFF
    cmp x0, #0
    bne proc_loop
    b proc_main
proc_loop:
    b proc_loop

proc_main:
    mov sp, 0x80000
    ldr x0, =__bss_start
    ldr w1, =__bss_size
clear_bss:
    cmp w1, #0
    beq exit
    str wzr, [x0], #8
    sub w1, w1, #1
    b clear_bss
exit:
    bl main
    b .