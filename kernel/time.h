#ifndef TIME_H
#define TIME_H

#define TIMERS_BASE 0x3F003000

/*
    Busy-waiting
    1. Certain operations in assembly take 1 cycle which means we can use simple loop
    2. Use Performance Monitors registers and read from them
    - PMCCNTR_EL0 - increments on every CPU cycle
    - PMCNTENSET_EL0 - bit [31] when set enables the Cycle Count Register, PMCCNTR_EL0
    - PMUSERENR_EL0 - enables or disables EL0 access to the Performance Monitors
 */

inline void busy_wait_cycles(unsigned int cycles_num) {
    asm volatile (
        "1: subs %0, %0, #1 \n"
        "bne 1b"
        : "=r"(cycles_num)
        : "0"(cycles_num)
        : "cc"
    );
}

#endif /* TIME_H */