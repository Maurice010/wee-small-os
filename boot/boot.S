.section ".text.boot"

.global _start
_start:
    // check CPU id, stop all except when id == 0
    mrs x0, MPIDR_EL1
    and x0, x0, #0xFF
    cmp x0, #0
    bne proc_loop
    b curr_EL
proc_loop:
    b proc_loop

curr_EL:
    // get current EL
    mrs x0, CurrentEL
    and x0, x0, #12 // EL info is on bits 2 & 3

check_EL3:
    // if not EL3 go to check_EL2
    cmp x0, #12
    bne check_EL2
    // else go from EL3 to EL1
    mov     x2, #(1 << 31)
    msr     HCR_EL2, x2

    mov     x2, #0x4b1
    msr     SCR_EL3, x2

    mov     x2, #0x3c5
    msr     SPSR_EL3, x2

    adr     x2, proc_main
    msr     ELR_EL3, x2

    eret

check_EL2:
    // if EL1 go to proc_main
    cmp x0, #4
    beq proc_main
    // else go from EL2 to EL1
    mov     x2, #(1 << 31)
    msr     HCR_EL2, x2

    mov     x2, #0x3c4
    msr     SPSR_EL2, x2

    adr     x2, proc_main
    msr     ELR_EL2, x2

    eret

proc_main:
    ldr x1, =_start
    mov sp, x1
    
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