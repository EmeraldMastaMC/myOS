#ifndef __MY_OS_GDT__
#define __MY_OS_GDT__

#include "../stdint.h"

typedef struct
{
	int16_t limit_low   : 16;
	int16_t base_low    : 16;
	int8_t  base_mid   :  8;
	int8_t  Ac         :  1;
	int8_t  RW         :  1;
	int8_t  DC         :  1;
	int8_t  Ex         :  1;
	int8_t  S          :  1;
	int8_t  Privl      :  2;
	int8_t  Pr         :  1;
	int8_t  limit_high :  4;
	int8_t  null       :  1;
	int8_t  L          :  1;
	int8_t  Sz         :  1;
	int8_t  Gr         :  1;
	int8_t  base_high  :  8;
} __attribute__((packed)) GDT_entry_t;

typedef struct
{
	int16_t size;
	int64_t offset;
} __attribute__((packed)) GDT_descriptor_t;

extern void load_GDT(uint64_t, uint16_t, uint16_t);
void GDT_set_entry(uint16_t index, uint32_t base, uint32_t limit, uint8_t accessByte, uint8_t flags);
void initGDT();


// Access Byte
#define GDT_VALID_ENTRY      (1 << 7) // Pr    Bit
#define GDT_RING_0           (0 << 5) // Privl Bit
#define GDT_RING_1           (1 << 5)
#define GDT_RING_2           (2 << 5)
#define GDT_RING_3           (3 << 5)
#define GDT_CODE_DATA_ENTRY  (1 << 4) // S  Bit
#define GDT_EXECUTABLE_ENTRY (1 << 3) // Ex Bit
#define GDT_CODE_CONFORMING  (1 << 2) // DC Bit
#define GDT_STACK_GROWS_DOWN (1 << 2) // DC Bit
#define GDT_READ_WRITE       (1 << 1) // RW Bit

// Flags
#define GDT_PAGE_GRANULARITY (1 << 3) // Gr Bit
#define GDT_BYTE_GRANULARITY (0 << 3)
#define GDT_16_BIT           (0 << 1) // Sz and L bits
#define GDT_32_BIT           (2 << 1)
#define GDT_64_BIT           (1 << 1)

#endif

