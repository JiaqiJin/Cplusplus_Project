#ifndef HANDMADE_PLATFORM_H
#define HANDMADE_PLATFORM_H

#include <stdint.h>
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int32 bool32;
typedef float real32;
typedef double real64;

#define global_variable static
#define local_persist static
#define internal static

#define ArrayCount(arr) (sizeof((arr)) / (sizeof((arr)[0])))

#define Pi32 3.14159265359

inline uint32
SafeTruncateUInt64(uint64 Value)
{
	Assert(Value <= 0xFFFFFFFF);
	uint32 Result = (uint32)Value;
	return Result;
}

#endif
