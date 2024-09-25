#ifndef UART_H
#define UART_H

#include "io.h"
#include "time.h"

/* mini UART Registers */
#define AUX_ENABLES     PERIPHERAL_BASE + 0x00215004
#define AUX_MU_IO       PERIPHERAL_BASE + 0x00215040
#define AUX_MU_IER      PERIPHERAL_BASE + 0x00215044
#define AUX_MU_IIR      PERIPHERAL_BASE + 0x00215048
#define AUX_MU_LCR      PERIPHERAL_BASE + 0x0021504C
#define AUX_MU_MCR      PERIPHERAL_BASE + 0x00215050
#define AUX_MU_LSR      PERIPHERAL_BASE + 0x00215054
#define AUX_MU_MSR      PERIPHERAL_BASE + 0x00215058
#define AUX_MU_SCRATCH  PERIPHERAL_BASE + 0x0021505C
#define AUX_MU_CNTL     PERIPHERAL_BASE + 0x00215060
#define AUX_MU_STAT     PERIPHERAL_BASE + 0x00215064
#define AUX_MU_BAUD     PERIPHERAL_BASE + 0x00215068

/* Baudrates */
#define BAUD_115200 270

/* Data length */
#define LEN_8 3

void uart_init(void);
int uart_write(char *buffer, int size);
int uart_read(char *buffer, int size);

#endif /* UART_H */