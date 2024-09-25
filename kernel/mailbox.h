#ifndef MAILBOX_H
#define MAILBOX_H

#include "io.h"

#define MBOX_BASE       PERIPHERAL_BASE + 0x0000B880

/* Registers */
#define MBOX_READ       MBOX_BASE + 0x0
#define MBOX_PEEK       MBOX_BASE + 0x10
#define MBOX_SENDER     MBOX_BASE + 0x14
#define MBOX_STATUS     MBOX_BASE + 0x18
#define MBOX_CONFIG     MBOX_BASE + 0x1c
#define MBOX_WRITE      MBOX_BASE + 0x20

/* Channels */
#define MBOX_POWER          0 // Power management
#define MBOX_FRAMEBUFFER    1
#define MBOX_VUART          2 // Virtual UART
#define MBOX_VCHIQ          3 // VideoCore Host Interface Queue
#define MBOX_LEDS           4
#define MBOX_BUTTONS        5
#define MBOX_TSCREEN        6 // Touch screen
#define MBOX_PTAGS_AG       8 // Property tags (ARM to GPU)
#define MBOX_PTAGS_GA       9 // Property tags (GPU to ARM)	

/* Tags */

/* Flags */
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

unsigned int mbox_create_mail(unsigned char channel, unsigned int data);
int mbox_write(unsigned int mail);
unsigned int mbox_read(void);

#endif /* MAILBOX_H */