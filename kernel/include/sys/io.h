#ifndef __MY_OS_IO__
#define __MY_OS_IO__

#include "../stdint.h"

void   outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);

#endif