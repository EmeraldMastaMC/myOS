#include "../../../include/sys/memory/page_management.h"
#include "../../../include/stdbool.h"
#include "../../../include/stdlib.h"

__attribute__((aligned(PAGE_SIZE))) free_list_node free_list[1024];
uint16_t used_nodes;

#define MAX_SPACE (GIGABYTE << 3)

void* pmalloc(size_t size)
{
  static bool first = true;

  // This code is here to initialized global variables
  if(first)
  {
    free_list[0].start = PROGRAM_BASE_ADDRESS;
    free_list[0].size  = MAX_SPACE; // GIGABYTE * 8
    used_nodes = 1;                     // There will always be one big chunk of free memory at first, so there will only be one node.
    first = false;                     // Faster way to make first = 0
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

uint8_t pfree(void* allocated_memory, size_t size)
{
  if(size > MAX_SPACE) return MEMORY_FREE_REQUEST_SIZE_TOO_LARGE;
  if(pfree_is_already_freed(allocated_memory)) return DOUBLE_FREE;
  for(unsigned int i = 0; i < used_nodes; i++)
  {
    // Move along nodes until we reach an adjacent free block
    if((size_t)allocated_memory > (size_t)free_list[i].start)
      continue;

    if(i == 0)
    {
      if ((allocated_memory >= PROGRAM_BASE_ADDRESS) && (size < ((size_t)free_list[i].start - (size_t)allocated_memory)))
      {
        // Shift nodes
        for(int j = 0; j >= (int)i; j--)
        {
          free_list_node tmp = free_list[j];
          free_list[j + 1] = tmp;
        }
        free_list[i].start = allocated_memory;
        free_list[i].size  = size;
        return EXIT_SUCCESS;
      }
      else if ((allocated_memory >= PROGRAM_BASE_ADDRESS) && (size == ((size_t)free_list[i].start - (size_t)allocated_memory)))
      {
        free_list[i].size += size;
        free_list[i].start = allocated_memory;
        return EXIT_SUCCESS;
      }
    }

    // If we have a previous node
    if(i > 0)
    {
      if(PFREE_ALIGNED_PERFECT(free_list[i - 1], allocated_memory, size, free_list[i]))
      {
        free_list[i - 1].size += size;
        free_list[i - 1].size += free_list[i].size;

        // We don't clear the next one because all the nodes are preallocated in an array for us, and clearing it would be pointless

        used_nodes--;

        return EXIT_SUCCESS;

      } 
      else if(PFREE_ALIGNED_LEFT(free_list[i - 1], allocated_memory, size, free_list[i]))
      {
        free_list[i - 1].size += size;

        return EXIT_SUCCESS;
      } 
      else if(PFREE_ALIGNED_RIGHT(free_list[i - 1], allocated_memory, size, free_list[i]))
      {
        free_list[i].start = allocated_memory;
        free_list[i].size += size;

        return EXIT_SUCCESS;
      }
      else if(PFREE_ALIGNED_MIDDLE(free_list[i - 1], allocated_memory, size, free_list[i]))
      {
        // Shift the array from the next node one to the right to make room for our new node
        for(unsigned int j = used_nodes - 1; j >= i; j--)
        {
          free_list_node tmp = free_list[j];
          free_list[j + 1] = tmp;
        }

        free_list[i].start = allocated_memory;
        free_list[i].size  = size;

        used_nodes++;
        return EXIT_SUCCESS;
      }
    }
  }
  return EXIT_FAILURE;
}

bool pfree_is_already_freed(void* address)
{
  for(unsigned int i = 0; i < used_nodes; i++)
    if(((size_t)free_list[i].start <= (size_t)address) && ((size_t)address <= ((size_t)free_list[i].start + ((size_t)free_list[i].start + (free_list[i].size << 12))))) return true;
  return false;
}

