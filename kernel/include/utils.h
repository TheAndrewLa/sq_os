#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <cstddef>
#include <cstdint>

#include <concepts>
#include <type_traits>

#define ASM_ONELINE(str) __asm__ volatile(str)

#define ASM_CLI() ASM_ONELINE("cli")
#define ASM_STI() ASM_ONELINE("sti")

namespace kernel {

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

using wide_ptr = ::std::uint64_t;

template <usize N>
struct is_power_of_two
    : std::conditional_t<(N & (N - 1)) == 0, std::true_type, std::false_type> {
};

template <usize N>
constexpr bool is_power_of_two_v = is_power_of_two<N>::value;

namespace memory {

constexpr usize kilobytes(usize n) {
  return static_cast<usize>(1024) * n;
}

constexpr usize megabytes(usize n) {
  return kilobytes(static_cast<usize>(1024) * n);
}

constexpr usize gigabytes(usize n) {
  return megabytes(static_cast<usize>(1024) * n);
}

template <typename T, usize N = 1>
inline bool is_aligned(T* ptr) {
  usize pointer = reinterpret_cast<usize>(ptr);
  return !(pointer % (sizeof(T) * N));
}

template <std::copyable T>
void memory_copy(const T* src, T* dst, usize size) {
  if (src == nullptr || dst == nullptr) {
    return;
  }

  usize i = 0;
  while (i < size) {
    *(dst + i) = *(src + i);
    i++;
  }
}

template <std::copyable T>
void memory_set(T* mem, const T& element, usize size) {
  if (mem == nullptr) {
    return;
  }

  usize i = 0;

  while (i < size) {
    *(mem + i) = element;
    i++;
  }
}

template <typename T>
void memory_zero(T* mem, usize size) {
  memory_set<uint8>(reinterpret_cast<uint8*>(mem), 0, sizeof(T) * size);
}

}  // namespace memory

namespace ports {

extern "C" uint8 inb(uint16 port);
extern "C" uint16 inw(uint16 port);

extern "C" void outb(uint16 port, uint8 byte);
extern "C" void outw(uint16 port, uint16 word);

inline void io_delay() {
  outw(0x80, 0x0);
}

}  // namespace ports

namespace registers {

extern "C" uint32 read_cr0();
extern "C" void write_cr0(uint32 address);

extern "C" uint32 read_cr2();

extern "C" uint32 read_cr3();
extern "C" void write_cr3(void* address);

extern "C" uint32 read_cr4();
extern "C" void write_cr4(uint32 address);

}  // namespace registers

}  // namespace kernel

#endif  // (c) by anrdew.la
