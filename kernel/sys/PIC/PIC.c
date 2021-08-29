#include "../../include/sys/PIC.h"
#include "../../include/stdint.h"
#include "../../include/sys/io.h"

void set_mask_PIC(uint8_t IRQ_line)
{
  outb(IRQ_line < 8 ? PIC1_DATA : PIC2_DATA, inb(IRQ_line < 8 ? PIC1_DATA : PIC2_DATA) |  (1 << IRQ_line));
}

void clear_mask_PIC(uint8_t IRQ_line)
{
  outb(IRQ_line < 8 ? PIC1_DATA : PIC2_DATA, inb(IRQ_line < 8 ? PIC1_DATA : PIC2_DATA) & ~(1 << IRQ_line));
}

void remap_PIC(uint8_t offset0, uint8_t offset1)
{
  // https://wiki.osdev.org/8259_PIC 
  
  uint8_t masks0 = inb(PIC1_DATA);
  uint8_t masks1 = inb(PIC2_DATA);

  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Start initialization.
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

  outb(PIC1_DATA, offset0);                  // Offset in IDT
  outb(PIC2_DATA, offset1);

  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);

  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);

  outb(PIC1_DATA, masks0);
  outb(PIC2_DATA, masks1);
}

void init_PIC()
{
  remap_PIC(0x20, 0x28);
  outb(PIC1_DATA, 0xFD);
  outb(PIC2_DATA, 0xFF);
}
