#ifndef BOOTLOADER_DEFS_H
#define BOOTLOADER_DEFS_H

#include <cstddef>
#include <cstdint>

#include <functional>

#define ASM_ONELINE(str) __asm__ volatile(str)

#define ASM_CLI() ASM_ONELINE("cli")
#define ASM_STI() ASM_ONELINE("sti")

namespace boot {

using uint8 = ::std::uint8_t;
using int8 = ::std::int8_t;

using uint16 = ::std::uint16_t;
using int16 = ::std::int16_t;

using uint32 = ::std::uint32_t;
using int32 = ::std::int32_t;

using uint64 = ::std::uint64_t;
using int64 = ::std::int64_t;

using usize = ::std::size_t;
using isize = ::std::ptrdiff_t;

using assert_fn = ::std::function<void(bool, const char*)>;

}  // namespace boot

#endif  // (c) by anrdew.la
