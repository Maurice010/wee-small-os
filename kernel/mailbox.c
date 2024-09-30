#include "mailbox.h"

volatile unsigned int  __attribute__((aligned(16))) mbox[36];

unsigned int mbox_create_mail(unsigned char channel, unsigned int data)
{
    unsigned int mail = data & ~0xF;
    mail |= channel & 0xF;

    return mail;
}

int mbox_write(unsigned int mail)
{
    while (in32(MBOX_STATUS) & MBOX_FULL);
    out32(MBOX_WRITE, mail);

    return 0;
}

unsigned int mbox_read(void)
{
    while (in32(MBOX_STATUS) & MBOX_EMPTY);
    return in32(MBOX_READ);
}