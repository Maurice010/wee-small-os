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

/* Generic Timer */
/* TODO: Should we take into account how much time actions take here? */
inline void busy_wait_time(unsigned int n, unsigned int factor)
{
    unsigned int frequency, cycles, to_wait, tmp_count, current_count;
    /* Get frequency */
    asm volatile(
        "mrs %0, CNTFRQ_EL0 \n"
        : "=r" (frequency)
    );
    cycles = ((frequency / factor) * n);
    /* Get current count */
    asm volatile(
        "mrs %0, CNTPCT_EL0 \n"
        : "=r" (tmp_count)
    );

    to_wait = tmp_count + cycles;
    current_count = 0;
    /* Check if n ms has passed */
    while (current_count < to_wait) {
        asm volatile(
            "mrs %0, CNTPCT_EL0 \n"
            : "=r" (current_count)
        );
    }
}

#endif /* TIME_H */