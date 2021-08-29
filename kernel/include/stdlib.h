#ifndef __MY_OS_STDLIB__
#define __MY_OS_STDLIB__

#include "stdint.h"

#define nullptr (void*)0x0000000000000000
#define EXIT_FAILURE 0x01
#define EXIT_SUCCESS 0x00

typedef uint64_t size_t;

#define KILOBYTE  (uint64_t)0x400
#define MEGABYTE  (uint64_t)(KILOBYTE << 10)
#define GIGABYTE  (uint64_t)(MEGABYTE << 10)
#define TERABYTE  (uint64_t)(GIGABYTE << 10)
#define PETABYTE  (uint64_t)(TERABYTE << 10)
#define  EXABYTE  (uint64_t)(PETABYTE << 10)

#endif
