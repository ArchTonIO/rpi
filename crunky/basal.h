#pragma once

#include <inttypes.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

//u64 __aeabi_uidivmod(u32 value, u32 divisor);
//u32 __aeabi_uidiv(u32 value, u32 divisor);

#define PERIPHERAL_BASE 0x3F000000 /* RPi 2/3 */

#define GPU_MEM_BASE  0xC0000000 // L2 cache disabled, depending on Pi

#if RPI == 3
	#define PBASE 0x3F000000
#elif RPI == 4
	#define PBASE 0xFE000000
#else
	#warning RPI definition not set
#endif

