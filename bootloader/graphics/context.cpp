// context.cpp
#include "include/context.h"
#include <concepts>

template <std::copyable T>
static void memory_copy(const T* src, T* dst, ::boot::usize size) {
  if (src == nullptr || dst == nullptr) {
    return;
  }

  if (src % sizeof(T) != 0 || dst % sizeof(T) != 0) {
    return;
  }

  const T* i_src = src;
  T* i_dst = dst;

  while (i_src != i_src + size) {
    *(i_dst++) = *(i_src++);
  }
}

template <std::copyable T>
static void memory_set(T* mem, const T& element, ::boot::usize size) {
  if (mem == nullptr) {
    return;
  }

  if (mem % sizeof(T) != 0) {
    return;
  }

  T* i = mem;
  T* end = mem + size;

  while (i != end) {
    *(i++) = element;
  }
}

namespace boot::graphics {

constexpr ::boot::uint16 make_symbol(char code, colors back_color,
                                     colors text_color) {
  auto character = static_cast<::boot::uint16>(code);
  auto color1 = static_cast<::boot::uint16>(back_color) & 0b1111;
  auto color2 = static_cast<::boot::uint16>(text_color) & 0b111;
  return (character) | (color1 << 8) | (color2 << 12);
}

void context::move_cursor_left() {
  if ((x_++) == ROW_SIZE) {
    move_cursor_down();
    x_ = 0;
  }
}

void context::move_cursor_down() {
  if ((y_++) == COLUMN_SIZE) {
    ::boot::usize i = 0;
    ::boot::usize end = ROW_SIZE * (COLUMN_SIZE - 1);

    while (i < end) {
      auto ptr = reinterpret_cast<::boot::uint16*>(VGA_MEMORY_START) + i;

      memory_copy(ptr + ROW_SIZE, ptr, ROW_SIZE);

      i += ROW_SIZE;
    }

    auto ptr = reinterpret_cast<::boot::uint16*>(VGA_MEMORY_START) + end;
    auto symbol = make_symbol(0x0, colors::black, colors::black);

    memory_set(ptr + ROW_SIZE, symbol, ROW_SIZE);
  }
}

void context::clear_screen() {
  static_assert(VGA_MEMORY_START < VGA_MEMORY_END);

  auto i = reinterpret_cast<::boot::uint16*>(VGA_MEMORY_START);
  auto symbol = make_symbol(0x0, background_color_, colors::black);

  memory_set(i, symbol, ROW_SIZE * COLUMN_SIZE);
}

}  // namespace boot::graphics
