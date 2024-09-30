#include "framebuffer.h"

struct fb_properties fb_properties;

void fb_init(void)
{
    mbox[0] = 35*4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = MBOX_TAG_SET_PHY;
    mbox[3] = 8;
    mbox[4] = 8;
    mbox[5] = 1920; // width in pixels
    mbox[6] = 1080; // height in pixels

    mbox[7] = MBOX_TAG_SET_VIRT;
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = 1920;
    mbox[11] = 1080;

    mbox[12] = MBOX_TAG_SET_VIRT_OFFSET;
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0; // X in pixels
    mbox[16] = 0; // Y in pixels

    mbox[17] = MBOX_TAG_SET_DEPTH;
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32; // bits per pixel

    mbox[21] = MBOX_TAG_SET_PIXEL_ORDER;
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1; // RGB: 1, BGR: 0

    /* Returns the address of the framebuffer and its size */
    mbox[25] = MBOX_TAG_ALLOC_BUFFER;
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096; // size
    mbox[29] = 0;    // alignment

    /* Returns number of bytes that are used per row of pixels in the framebuffer */
    mbox[30] = MBOX_TAG_GET_PITCH;
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0; // bytes per line

    mbox[34] = MBOX_TAG_END;

    unsigned int mail = mbox_create_mail(MBOX_PTAGS_AG, (unsigned int)mbox);
    mbox_write(mail);

    if (mail == mbox_read() && mbox[28] != 0) {
        mbox[28] &= 0x3FFFFFFF;                                 // convert GPU address to ARM address
        fb_properties.width = mbox[5];                          // physical width
        fb_properties.height = mbox[6];                         // physical height
        fb_properties.pitch = mbox[33];                         // bytes per line
        fb_properties.isrgb = mbox[24];                         // channel order
        fb_properties.fb = (void*)((unsigned long)mbox[28]);    // set the framebuffer pointer to the allocated memory address
    }
}

void draw_pixel(int x, int y, unsigned char color)
{
    int offset = x * 4 + y * fb_properties.pitch;
    *((unsigned int*)(fb_properties.fb + offset)) = vgapal[color & 0x0f]; // (unsigned int)(*(fb_properties.fb + offset)) is wrong :)
}
