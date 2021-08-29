#ifndef __MY_OS_PIC__
#define __MY_OS_PIC__

#include "../stdint.h"
#include "io.h"

#define PIC1_COMMAND     0x20 
#define PIC2_COMMAND     0xA0
#define PIC1_DATA        0x21
#define PIC2_DATA        0xA1


#define ICW1_ICW4        0x01    /* ICW4 (not) needed */
#define ICW1_SINGLE      0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4   0x04    /* Call address interval 4 (8) */
#define ICW1_LEVEL       0x08    /* Level triggered (edge) mode */
#define ICW1_INIT        0x10    /* Initialization - required! */
 
#define ICW4_8086        0x01    /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO        0x02    /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE   0x08    /* Buffered mode/slave */
#define ICW4_BUF_MASTER  0x0C    /* Buffered mode/master */
#define ICW4_SFNM        0x10    /* Special fully nested (not) */

void init_PIC(void);

void clear_mask_PIC(uint8_t IRQ_line);
void set_mask_PIC(uint8_t IRQ_line);

void remap_PIC(uint8_t offset0, uint8_t offset1);
#define EOI 0x20

#define SEND_EOI() outb(PIC1_COMMAND, EOI); outb(PIC2_COMMAND, EOI)
#endif
