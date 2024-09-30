#include "uart.h"
#include "framebuffer.h"

int main(void) {
    uart_init();
    uart_write("Hello world\n", 12);

    fb_init();

    while (1)
    {

    }
}