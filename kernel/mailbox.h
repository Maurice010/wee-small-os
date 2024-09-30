#ifndef MAILBOX_H
#define MAILBOX_H
/*
 * Mailbox communication between CPU and GPU.
 *
 * Structure of a mailbox message 
 * ------------------------------
 * Bracket notation coresponds to mbox array: [0] <-> mbox[0]
 * 
 * Basic message info:
 * [0] Message Length: total size of the message in bytes, including all tags and padding.
 * [1] Message Code: MBOX_REQUEST or MBOX_RESPONSE
 * 
 * Tag:
 * [2] Tag Identifier: like MBOX_TAG_GET_MAC
 * Here documentation says:
 * [3] Value Buffer Size: the size of the value buffer associated with this tag, in bytes.
 * [4] Tag Status: MBOX_REQUEST or MBOX_RESPONSE,
 * 
 * but it seems like it's more like this
 * [3] Request Value Buffer Size: the size of the value buffer associated with request.
 * [4] Response Value Buffer Size: the size of the value buffer associated with response.
 * [5->n] Values used in tag: each value has its own index in mbox
 * [n+1] Padding (optional):
 * 
 * Other Tags (if needed)
 * [n+2] ...
 * 
 * [last] Tag End: MBOX_TAG_END, marks the end of a message
 * 
 * Note: As it comes to [4] it seems that it doesnt matter what I type there.
 * ------------------------------
 * 
 * How to send and receive a message
 * ------------------------------
 * 1. Fill mbox[] as described above
 * 2. Call unsigned int mail = mbox_create_mail(mbox)
 * 3. Call mbox_write(mail)
 * 4. Call mbox_read(): currently it waits in a loop for a reply and returns it
 *                      (response should be in the same mbox array that was send before)
 * 5. Parse the response
 */
#include "io.h"
#include "uart.h"

#define MBOX_BASE       PERIPHERAL_BASE + 0x0000B880

/* Mailbox Registes */
#define MBOX_READ       MBOX_BASE + 0x0
#define MBOX_PEEK       MBOX_BASE + 0x10
#define MBOX_SENDER     MBOX_BASE + 0x14
#define MBOX_STATUS     MBOX_BASE + 0x18
#define MBOX_CONFIG     MBOX_BASE + 0x1c
#define MBOX_WRITE      MBOX_BASE + 0x20

/* Mailbox Channels */
#define MBOX_POWER          0 // Power management
#define MBOX_FRAMEBUFFER    1 // Framebuffer configuration and management (depricated)
#define MBOX_VUART          2 // Virtual UART
#define MBOX_VCHIQ          3 // VideoCore Host Interface Queue
#define MBOX_LEDS           4 // Control for LEDs
#define MBOX_BUTTONS        5 // Input channel for buttons
#define MBOX_TSCREEN        6 // Input channel for touch screen
#define MBOX_PTAGS_AG       8 // Property tags (ARM to GPU communication)
#define MBOX_PTAGS_GA       9 // Property tags (GPU to ARM communication)

/* Mailbox Tags (Property Tags used in mailbox messages) */
#define MBOX_TAG_SET_PHY            0x00048003 // Set physical (display) width/height
#define MBOX_TAG_SET_VIRT           0x00048004 // Set virtual (buffer) width/height
#define MBOX_TAG_SET_DEPTH          0x00048005 // Set depth
#define MBOX_TAG_SET_PIXEL_ORDER    0x00048006 // Set pixel order
#define MBOX_TAG_SET_VIRT_OFFSET    0x00048009 // Set virtual offset
#define MBOX_TAG_ALLOC_BUFFER       0x00040001 // Allocate buffer
#define MBOX_TAG_GET_PITCH          0x00040008 // Get the number of bytes per line in the framebuffer
#define MBOX_TAG_GET_MAC            0x00010003 // Get board MAC address
#define MBOX_TAG_END                0x0

/* Flags */
#define MBOX_REQUEST    0x00000000 // Mailbox request flag (message is a request)
#define MBOX_RESPONSE   0x80000000 // Mailbox response flag (message is a response)
#define MBOX_FULL       0x80000000 // Mailbox status flag indicating it is full
#define MBOX_EMPTY      0x40000000 // Mailbox status flag indicating it is empty

extern volatile unsigned int mbox[36]; // Buffer to store mailbox messages

unsigned int mbox_create_mail(unsigned char channel, unsigned int data);
int mbox_write(unsigned int mail);
unsigned int mbox_read(void);

#endif /* MAILBOX_H */