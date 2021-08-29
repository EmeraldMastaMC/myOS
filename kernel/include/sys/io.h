#ifndef __MY_OS_IO__
#define __MY_OS_IO__

#include "../stdint.h"

static inline uint8_t inbyte(uint16_t port) 
{
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline void outbyte(uint16_t port, uint8_t val)
{
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void(*volatile outb)(uint16_t, uint8_t) = outbyte;
static uint8_t(*volatile inb)(uint16_t) = inbyte;
#endif
