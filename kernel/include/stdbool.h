#ifndef __MY_OS_STDBOOL__
#define __MY_OS_STDBOOL__

#ifndef __cplusplus // If we aren't using C++, redeclare bool
typedef char bool;

#define true  0x01
#define false 0x00
#endif

#endif
