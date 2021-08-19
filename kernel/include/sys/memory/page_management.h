#ifndef __MY_OS_PAGE_MANAGEMENT__
#define __MY_OS_PAGE_MANAGEMENT__

#include "../../stdint.h"

struct free_list_node
{
	void*    start;
	uint64_t size;
} __attribute__((packed));

void* pmalloc(uint64_t size);
bool pfree_is_already_freed(void* address);


typedef struct free_list_node free_list_node;

// 2 4 8 16 32 64 128 256 512 1024

#define PROGRAM_BASE_ADDRESS (void*)0x200000
#define PAGE_SIZE (uint64_t)(KILOBYTE << 2)

// pfree error codes
#define MEMORY_ALREADY_FREED               0x01
#define MEMORY_FREE_REQUEST_SIZE_TOO_LARGE 0x02
#define 

#endif
