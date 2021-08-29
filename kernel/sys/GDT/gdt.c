#include "../../include/stdint.h"
#include "../../include/sys/GDT.h"

GDT_entry_t GDT[5];

GDT_descriptor_t GDTDescriptor;

void GDTSetEntry(uint16_t index, uint32_t base, uint32_t limit, uint8_t accessByte, uint8_t flags)
{
  // Base
  GDT[index].base_low  = (base & 0x0000FFFF) >> 0;
  GDT[index].base_mid  = (base & 0x00FF0000) >> 16;
  GDT[index].base_high  = (base & 0xFF000000) >> 24;


  // Limit
  GDT[index].limit_low  = (limit & 0x0FFFF) >> 0;
  GDT[index].limit_high = (limit & 0xF0000) >> 16;


  // Access Byte
  GDT[index].Pr    = (accessByte & 0b10000000) >> 7;
  GDT[index].Privl = (accessByte & 0b01100000) >> 5;
  GDT[index].S     = (accessByte & 0b00010000) >> 4;
  GDT[index].Ex    = (accessByte & 0b00001000) >> 3;
  GDT[index].DC    = (accessByte & 0b00000100) >> 2;
  GDT[index].RW    = (accessByte & 0b00000010) >> 1;
  GDT[index].Ac    = (accessByte & 0b00000001) >> 0;

  // Flags
  GDT[index].null    = (flags & 0b00000001) >> 0;
  GDT[index].L       = (flags & 0b00000010) >> 1;
  GDT[index].Sz      = (flags & 0b00000100) >> 2;
  GDT[index].Gr      = (flags & 0b00001000) >> 3;
}

void init_GDT()
{
  GDTSetEntry(0, 0x00000000, 0x00000, 0b00000000, 0b00000000);

  GDTSetEntry(1, 0x00000000, 0xFFFFF, GDT_VALID_ENTRY | GDT_RING_0 | GDT_CODE_DATA_ENTRY | GDT_EXECUTABLE_ENTRY | GDT_READ_WRITE, GDT_PAGE_GRANULARITY | GDT_64_BIT);
  GDTSetEntry(2, 0x00000000, 0xFFFFF, GDT_VALID_ENTRY | GDT_RING_0 | GDT_CODE_DATA_ENTRY | GDT_READ_WRITE, GDT_PAGE_GRANULARITY | GDT_64_BIT);

  GDTSetEntry(3, 0x00000000, 0xFFFFF, GDT_VALID_ENTRY | GDT_RING_3 | GDT_CODE_DATA_ENTRY | GDT_EXECUTABLE_ENTRY | GDT_READ_WRITE, GDT_PAGE_GRANULARITY | GDT_64_BIT);
  GDTSetEntry(4, 0x00000000, 0xFFFFF, GDT_VALID_ENTRY | GDT_RING_3 | GDT_CODE_DATA_ENTRY | GDT_READ_WRITE, GDT_PAGE_GRANULARITY | GDT_64_BIT);

  GDTDescriptor.size   = sizeof(GDT) - 1;
  GDTDescriptor.offset = (uint64_t)GDT;

  load_GDT((uint64_t)&GDTDescriptor, 0x08, 0x10);
}
