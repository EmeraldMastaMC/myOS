#ifndef __MY_OS_SYS__
#define __MY_OS_SYS__

#define CODE_SEG        0x08
#define DATA_SEG        0x10

#define KERNEL_CODE_SEG CODE_SEG
#define KERNEL_DATA_SEG DATA_SEG
#define USER_CODE_SEG   CODE_SEG + 0x10
#define USER_DATA_SEG   DATA_SEG + 0x10

#endif

