#include "uart.h"

int main(void) {
    uart_init();
    uart_write("Hello world", 11);
    while (1);
}