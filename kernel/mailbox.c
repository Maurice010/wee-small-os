#include "mailbox.h"

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

int mbox_call(struct mbox_property_message* message, unsigned char channel)
{
    unsigned int mail = mbox_create_mail(channel, (unsigned int)message);
    mbox_write(mail);

    while (1) {
        if (mail == mbox_read()) {
            return message->status == MBOX_RESPONSE;
        }
    }

    return 0;
}

struct mbox_property_message get_MAC_address(void)
{
    struct mbox_property_message msg;
    struct mbox_property_tag tags[2];
    unsigned int buf[2];

    tags[0].buffer_size = 6;
    tags[0].id = MBOX_TAG_GETMAC;
    tags[0].buffer = buf;
    tags[1].id = MBOX_TAG_END;

    msg.size = 32;
    msg.status = MBOX_REQUEST;
    msg.tags = tags;

    mbox_call(&msg, MBOX_PTAGS_AG);

    return msg;
}