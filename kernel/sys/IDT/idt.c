#include "../../include/stdint.h"
#include "../../include/sys/IDT.h"
#include "../../include/sys/io.h"
#include "../../include/sys/sys.h"

IDT_entry_t IDT[256];
IDT_descriptor_t IDT_descriptor;

extern uint64_t IRQ0;
extern uint64_t IRQ1;

void IDT_set_entry(uint8_t index, uint64_t offset, uint16_t selector, uint8_t type_attr)
{
	IDT[index].offset_lo = (offset & 0x000000000000FFFF) >>  0;
	IDT[index].offset_mi = (offset & 0x00000000FFFF0000) >> 16;
	IDT[index].offset_hi = (offset & 0xFFFFFFFF00000000) >> 32;

	IDT[index].selector  = selector;

	IDT[index].gate_type = (type_attr & 0b00001111) >> 0;
	IDT[index].Z         = (type_attr & 0b00010000) >> 4;
	IDT[index].DPL       = (type_attr & 0b01100000) >> 5;
	IDT[index].P         = (type_attr & 0b10000000) >> 7;

	IDT[index].ist       = 0x00;
	IDT[index].zero      = 0x00000000;
}

void init_IDT()
{
	IDT_set_entry(0x20, (uint64_t)&IRQ0, CODE_SEG, IDT_GATE_PRESENT | IDT_GATE_DESCRIPTOR_PRIVILEGE_LEVEL_0 | IDT_GATE_TYPE_32_BIT_INTERRUPT_GATE);
	IDT_set_entry(0x21, (uint64_t)&IRQ1, CODE_SEG, IDT_GATE_PRESENT | IDT_GATE_DESCRIPTOR_PRIVILEGE_LEVEL_0 | IDT_GATE_TYPE_32_BIT_INTERRUPT_GATE);

	IDT_descriptor.size   = sizeof(IDT) - 1;
	IDT_descriptor.offset = (uint64_t)IDT;

	load_IDT();
}
