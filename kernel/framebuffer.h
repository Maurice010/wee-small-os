#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "mailbox.h"

/*
 * Framebuffer properties:
 * width, height - the actual physical width and height of the screen in pixels
 * pitch - the number of bytes per row in the framebuffer (depends on width and color depth)
 * isrgb - flag indicating the pixel order (RGB or BGR)
 * fb - pointer to the framebuffer in memory
 */
struct fb_properties {
    unsigned int width, height, pitch, isrgb;
    unsigned char *fb;
};

extern struct fb_properties fb_properties;

unsigned int vgapal[] = {
    0x000000,
    0x0000AA,
    0x00AA00,
    0x00AAAA,
    0xAA0000,
    0xAA00AA,
    0xAA5500,
    0xAAAAAA,
    0x555555,
    0x5555FF,
    0x55FF55,
    0x55FFFF,
    0xFF5555,
    0xFF55FF,
    0xFFFF55,
    0xFFFFFF
};

void fb_init(void);
void draw_pixel(int x, int y, unsigned char color);

#endif /* FRAMEBUFFER_H */