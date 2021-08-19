#include "../../../include/sys/memory/page_management.h"
#include "../../../include/stdbool.h"
#include "../../../include/stdlib.h"

__attribute__((aligned(PAGE_SIZE))) free_list_node free_list[1024];
uint16_t used_nodes;


void* pmalloc(uint64_t size)
{
	static bool first = true;

	// This code is here to initialized global variables
	if(first)
	{
		free_list[0].start = PROGRAM_BASE_ADDRESS;
		free_list[0].size  = GIGABYTE << 3; // GIGABYTE * 8
		used_nodes = 1;                     // There will always be one big chunk of free memory at first, so there will only be one node.
		first ^= first;                     // Faster way to make first = 0
	}

	// Iterate through the free_list nodes
	for(unsigned int i = 0; i < used_nodes; i++)
	{
		if(free_list[i].size >= size)
		{
			void* allocated_memory = free_list[i].start; 
			free_list[i].size     -= size;
			free_list[i].start    += size << 12; // size * 4096 
			return allocated_memory;
		}
	}
	return nullptr; // Caller is expected to deal with a nullptr
}

uint8_t pfree(void* allocated_memory, uint64_t size)
{
	if(pfree_is_already_freed(allocated_memory)) return;

	for(int i = 0; i < used_nodes; i++)
	{
		if((uint64_t)allocated_memory > /*place holder!!!*/size)
		{
			
		}
	}
}

bool pfree_perfect_fit(free_list_node prev_start, void* address_to_be_freed, uint64_t size_to_be_freed, free_list_node next_start)
{
	return ((((uint64_t)prev_start.start + ((uint64_t)prev_start.size << 12)) == (uint64_t)address_to_be_freed) && (size_to_be_freed == (((uint64_t)prev_start.start + ((uint64_t)prev_start.size << 12)) - (uint64_t)next_start.start)));
}

bool pfree_is_already_freed(void* address)
{
	for(unsigned int i = 0; i < used_nodes; i++)
	{
		if(((uint64_t)free_list[i].start <= (uint64_t)address) && ((uint64_t)address <= ((uint64_t)free_list[i].start + ((uint64_t)free_list[i].start + (free_list[i].size << 12))))) return true;
	}
	return false;
}

