// defs.h
#ifndef DEFS_H
#define DEFS_H

#include <cstddef>
#include <cstdint>

#define ASM_ONELINE(str) __asm__ volatile(str)

// TODO: more complicated inline asm macroses

#define CLI() ASM_ONELINE("cli")
#define STI() ASM_ONELINE("sti")

using u8 = std::uint8_t;
using i8 = std::int8_t;

using u16 = std::uint16_t;
using i16 = std::int64_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

using u64 = std::uint64_t;
using i64 = std::int64_t;

using usize = std::size_t;
using isize = std::ptrdiff_t;

#endif  // (c) by andrew.la
