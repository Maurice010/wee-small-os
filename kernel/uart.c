#include "uart.h"

// struct uart_info {
//     unsigned int baud, length;
// };

// struct uart_device {

// };

void out32(unsigned long reg, unsigned int data)
{
    *(volatile unsigned int *) reg = data;
}

unsigned int in32(unsigned long reg)
{
    return *(volatile unsigned int *) reg;
}

void uart_init(void) {
    /* Choose GPIO pins for UART connection */
    // our aim -> xxxx xxxx xxxx xxx0 1001 0xxx xxxx xxxx
    unsigned int reg;
    reg = in32(GPFSEL0);
    reg &=  ~(63 << 12); // clear bits 12-17
    reg |= (18 << 12); // set bits 12-17 to 010010

    /*
        Remove pull up/down states from pins
        Actually I'm not sure if it's necessary, but at least it has some educational value
     */

    /* TODO: Implement wait(cycles_num) function */
    out32(GPPUD, 0);
    busy_wait_cycles(150);
    out32(GPPUDCLK0, (3 << 14));
    busy_wait_cycles(150);
    out32(GPPUDCLK0, 0);

    /* Config UART transmission settings */
    out32(AUX_ENABLES, 1); // Mini UART enable
    out32(AUX_MU_CNTL, 0); // Disable transmission
    out32(AUX_MU_IER, 0); // Disable interrupts
    out32(AUX_MU_LCR, LEN_8); // 8-bit data length mode
    out32(AUX_MU_MCR, 0); // UART1_RTS high - not ready to send data
    out32(AUX_MU_BAUD, BAUD_115200); // Set baudrate
    out32(AUX_MU_CNTL, 3); // Enable transmission
}

int uart_write(char *buffer, int size) {
    int i = 0;
    while (i < size) {
        while (!(in32(AUX_MU_LSR) & 0x20));
        out32(AUX_MU_IO, *buffer++);
    }

    return i;
}

unsigned int uart_read() {
    return 0;
}