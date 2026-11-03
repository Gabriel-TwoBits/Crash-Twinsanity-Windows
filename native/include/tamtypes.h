#pragma once

// The native build's PS2SDK tamtypes.h: the game's integer types at the widths the PS2's have. Pointer-sized integers are the
// host's (64 bit, or 32 on 32 bit x86), which is the one place the native build deliberately differs from the PS2's n32 layout
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
#ifdef __SIZEOF_INT128__
typedef unsigned __int128 u128;
typedef __int128 s128;
#else
// 32 bit x86 has no 128 bit integer: a quadword's storage, at its size and alignment (nothing does arithmetic on one)
typedef struct alignas(16)
{
    u64 low;
    u64 high;
} u128;
typedef u128 s128;
#endif

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile u128 vu128;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
typedef volatile s128 vs128;

typedef uintptr_t uiptr;
typedef intptr_t siptr;
typedef volatile uiptr vuiptr;
typedef volatile siptr vsiptr;
