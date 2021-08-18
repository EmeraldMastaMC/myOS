#include "../include/stdint.h"

struct free_space_block
{
	uint32_t size;
	void*    start;
	struct free_space_block* next;
} __attribute__((packed));
typedef struct free_space_block free_space_block_t;

__attribute__((aligned(0x1000))) free_space_block_t memory_map = {(512*512)*8, (void*)0x200000, (free_space_block_t*)0x00000000};
void*   start_of_progam_space                = (void*)0x200000;
void*   allocator_base                       = (void*)&memory_map;
free_space_block_t* current_free_space_block = &memory_map; // Used for allocating memory for our memory allocators, weird right?
uint32_t sizeof_free_block_list = 1;


void* pmalloc(uint32_t amount)
{
	static uint8_t first = 1;
	if(first)
	{
		memory_map.size  = 512*512*8;
		memory_map.start = (void*)0x200000;
		first = 0;
	}

	if(memory_map.size >= amount)
	{
		void* result = memory_map.start;
		memory_map.size  -= amount;
		memory_map.start += amount << 12; // Shift 12 to multiply faster
		return result;
	} else
	{
		if(memory_map.next == (void*)0x0000) return 0x0000; // Caller is expected to handle null pointers
		current_free_space_block = memory_map.next;
		while(1)
		{
			if(current_free_space_block->size >= amount)
			{
				void* result = current_free_space_block->start;
				current_free_space_block->size -= amount;
				current_free_space_block->size += amount << 12;
				return result;
			}
			if(current_free_space_block->next == (void*)0x0000) return 0x0000;
			current_free_space_block= current_free_space_block->next;
		}
	}
}

void pfree(void* page_start, uint32_t size)
{
	// If the memory address is below what we are allowed to allocate it, ignore the request
	if(page_start < (void*)0x200000) return;

	// If memory is already freed, then don't even try to free it
	current_free_space_block = allocator_base;
	for(unsigned int i = 0; i < sizeof_free_block_list; i++)
	{
		if((page_start >= current_free_space_block->start) && (page_start < (current_free_space_block->start + current_free_space_block->size)))
		{
			return;
		}
		current_free_space_block = current_free_space_block->next;
	}

	current_free_space_block = allocator_base;
	if(memory_map.start > page_start)
	{
		// Used Space from base to next free space
		uint32_t used_space = ((uint64_t)memory_map.start - (uint64_t)allocator_base) >> 12; // Shift 12 to divide faster
		if(used_space < size) return;

		if(used_space == size && page_start == start_of_progam_space)
		{
			memory_map.start  = allocator_base;
			memory_map.size  += used_space;
			return;
		} if(used_space > size)
		{
			// Used space frome page_start to next free space
			uint32_t delta = ((uint64_t)memory_map.start - (uint64_t)page_start) >> 12;
			if(delta == size)
			{
				memory_map.size += size;
				memory_map.start = page_start;
				return;
			} else 
			{
				current_free_space_block += sizeof_free_block_list;
				sizeof_free_block_list++;

				current_free_space_block->start = memory_map.start;
				current_free_space_block->size  = memory_map.size;
				current_free_space_block->next  = memory_map.next;
				
				memory_map.start = page_start;
				memory_map.size = size;
				memory_map.next = current_free_space_block;
				current_free_space_block--;
				return;
			}
		}
	} if(memory_map.start < page_start)
	{
		current_free_space_block = memory_map.next;
		free_space_block_t* prev_free_block = (free_space_block_t*)0x0000;
		for(unsigned int i = 1; i < sizeof_free_block_list; i++)
		{
			if(current_free_space_block->start < page_start)
			{ 
				prev_free_block          = current_free_space_block;       // Update Previous block
				current_free_space_block = current_free_space_block->next; // Next block
				continue;
			} 
			// The memory is inbetween the previous and the current
			else
			{
				uint32_t delta = ((uint64_t)current_free_space_block->start - (uint64_t)page_start) >> 12;

				// If the memory fits perfectly inbetween two of them
				if((delta == size) && (page_start == (void*)(((uint64_t)prev_free_block->start) + ((uint64_t)prev_free_block->size << 12))))
				{
					sizeof_free_block_list--;
					prev_free_block->size += size;
					prev_free_block->size += current_free_space_block->size;
					prev_free_block->next  = current_free_space_block->next;
					return;
				}

				// If the memory that is being freed touches the current block
				if(delta == size)
				{
					current_free_space_block->size  += size;
					current_free_space_block->start  = page_start;
					return;
				} 
				// If the memory that is being freed touches the previous block
				else if(page_start == (void*)(((uint64_t)prev_free_block->start) + ((uint64_t)prev_free_block->size << 12)))
				{
					prev_free_block->size += size;
					return;
				} 

				// The memory that is being freed is in the middle, touching neither
				else
				{
					free_space_block_t* new_free_block = current_free_space_block + sizeof_free_block_list;
					sizeof_free_block_list++;
					prev_free_block->next = new_free_block;
					new_free_block->size  = size;
					new_free_block->start = page_start;
					new_free_block->next  = current_free_space_block;
					return;
				}
			}
		}
	}
	return;
}
