#ifndef GPIO_H
#define GPIO_H

#define PERIPHERAL_BASE 0x3F000000

#define GPFSEL0 PERIPHERAL_BASE + 0x00200000
#define GPFSEL1 PERIPHERAL_BASE + 0x00200004
#define GPFSEL2 PERIPHERAL_BASE + 0x00200008
#define GPFSEL3 PERIPHERAL_BASE + 0x0020000C
#define GPFSEL4 PERIPHERAL_BASE + 0x00200010
#define GPFSEL5 PERIPHERAL_BASE + 0x00200014

#define GPPUD PERIPHERAL_BASE + 0x00200094
#define GPPUDCLK0 PERIPHERAL_BASE + 0x00200098
#define GPPUDCLK1 PERIPHERAL_BASE + 0x0020009C

#endif /* GPIO_H */