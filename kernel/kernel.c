#include "uart.h"
#include "mailbox.h"

int main(void) {
    uart_init();
    uart_write("Hello world\n", 11);

    while (1)
    {

    }
}