#ifndef __MY_OS_STDLIB__
#define __MY_OS_STDLIB__

#include "stdint.h"

void* pmalloc(uint32_t amount);

void  pfree(void* page_start, uint32_t size);

#endif
