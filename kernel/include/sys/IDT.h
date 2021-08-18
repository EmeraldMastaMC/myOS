#ifndef __MY_OS_IDT__
#define __MY_OS_IDT__

#include "../stdint.h"
typedef struct
{
	uint16_t offset_lo : 16;
	uint16_t selector   : 16;
	uint8_t  ist        :  8;
	uint8_t  gate_type  :  4;
	uint8_t  Z          :  1;
	uint8_t  DPL        :  2;
	uint8_t  P          :  1;
	uint16_t offset_mi : 16;
	uint32_t offset_hi  : 32;
	uint32_t zero       : 32;
} __attribute__((packed)) IDT_entry_t;

typedef struct
{
	uint16_t size;
	uint64_t offset;
} __attribute__((packed)) IDT_descriptor_t;

#define IDT_GATE_TYPE_32_BIT_TASK_GATE        ( 5 << 0)
#define IDT_GATE_TYPE_32_BIT_INTERRUPT_GATE   (14 << 0)
#define IDT_GATE_TYPE_32_BIT_TRAP_GATE        (15 << 0)
#define IDT_GATE_TYPE_16_BIT_INTERRUPT_GATE   ( 6 << 0)
#define IDT_GATE_TYPE_16_BIT_TRAP_GATE        ( 7 << 0)

#define IDT_GATE_DESCRIPTOR_PRIVILEGE_LEVEL_0 ( 0 << 5)
#define IDT_GATE_DESCRIPTOR_PRIVILEGE_LEVEL_1 ( 1 << 5)
#define IDT_GATE_DESCRIPTOR_PRIVILEGE_LEVEL_2 ( 2 << 5)
#define IDT_GATE_DESCRIPTOR_PRIVILEGE_LEVEL_3 ( 3 << 5)

#define IDT_GATE_PRESENT                      ( 1 << 7)

void IDT_set_entry(uint8_t index, uint64_t offset, uint16_t selector, uint8_t type_attr);

void init_IDT();

void load_IDT();

#endif
