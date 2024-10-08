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

extern unsigned int vgapal[];

void fb_init(void);
void draw_pixel(int x, int y, unsigned char color);

#endif /* FRAMEBUFFER_H */