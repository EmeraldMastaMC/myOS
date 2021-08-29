#ifndef __MY_OS_PAGE_MANAGEMENT__
#define __MY_OS_PAGE_MANAGEMENT__

#include "../../stdint.h"
#include "../../stdlib.h"
#include "../../stdbool.h"

struct free_list_node
{
	void*    start;
	size_t size;
} __attribute__((packed));

void* pmalloc(size_t size);
uint8_t pfree(void* allocated_memory, size_t size);
bool pfree_is_already_freed(void* address);


typedef struct free_list_node free_list_node;

// 2 4 8 16 32 64 128 256 512 1024

#define PROGRAM_BASE_ADDRESS (void*)0x200000
#define PAGE_SIZE (size_t)(KILOBYTE << 2)

// pfree error codes
#define DOUBLE_FREE                        0x01
#define MEMORY_FREE_REQUEST_SIZE_TOO_LARGE 0x02

#define PFREE_ALIGNED_LEFT(prev_node, address_to_be_freed, size_to_be_freed, next_node)    ((((size_t)prev_node.start     + ((size_t)prev_node.size   << 12)) == (size_t)address_to_be_freed) && (((size_t)address_to_be_freed + ((size_t)size << 12)) <= (size_t)next_node.start))
#define PFREE_ALIGNED_RIGHT(prev_node, address_to_be_freed, size_to_be_freed, next_node)   ((((size_t)address_to_be_freed + ((size_t)size_to_be_freed << 12)) == (size_t)next_node.start    ) && (((size_t)address_to_be_freed + ((size_t)size << 12)) <= (size_t)next_node.start))

#define PFREE_ALIGNED_PERFECT(prev_node, address_to_be_freed, size_to_be_freed, next_node) (PFREE_ALIGNED_LEFT(prev_node, address_to_be_freed, size_to_be_freed, next_node) && PFREE_ALIGNED_RIGHT(prev_node, address_to_be_freed, size_to_be_freed, next_node))
#define PFREE_ALIGNED_MIDDLE(prev_node, address_to_be_freed, size_to_be_freed, next_node)  ((!PFREE_ALIGNED_LEFT(prev_node, address_to_be_freed, size_to_be_freed, next_node)) && (!PFREE_ALIGNED_RIGHT(prev_node, address_to_be_freed, size_to_be_freed, next_node)))

#endif
