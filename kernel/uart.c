#include "uart.h"

// struct uart_info {
//     unsigned int baud, length;
// };

// struct uart_device {

// };
void uart_init(void) {
    /* Choose GPIO pins for UART connection (alternate function 5 for pins 14 & 15) */
    // our aim -> xxxx xxxx xxxx xxx0 1001 0xxx xxxx xxxx
    unsigned int reg;
    reg = in32(GPFSEL0);
    reg &=  ~(63 << 12); // clear bits 12-17
    reg |= (18 << 12); // set bits 12-17 to 010010

    /*
        Put pins 14 & 15 in floating state (remove pull up/down)
        Actually I'm not sure why it's necessary, but at least it has some educational value
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

int uart_write32(unsigned int data)
{
    while (!(in32(AUX_MU_LSR) & 0x20));
    out32(AUX_MU_IO, data);

    return 1;
}

int uart_write(char *buffer, int size)
{
    int i = 0;
    while (i < size) {
        while (!(in32(AUX_MU_LSR) & 0x20));
        if (*buffer == '\n') {
            out32(AUX_MU_IO, '\r');
        }
        out32(AUX_MU_IO, *buffer++);
        i++;
    }

    return i;
}

int uart_read(char *buffer, int size)
{
    int i = 0;
    while (i < size) {
        while (!(in32(AUX_MU_LSR) & 0x01));
        *buffer++ = in32(AUX_MU_IO);
        i++;
    }

    return i;
}