#include <zephyr/kernel.h>

#include "say_hello.h"

void say_hello(void)
{
    printk("Hello, World! From say_hello modules.\n");
}