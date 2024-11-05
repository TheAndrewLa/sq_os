// context.cpp
#include "include/context.h"
#include <concepts>

template <std::copyable T>
static void memory_copy(const T* src, T* ptr, ::boot::usize size) {
  if (src == nullptr || ptr == nullptr) {
    return;
  }

  ::boot::usize i = 0;

  while (i < size) {
    *(ptr + i) = *(src + i);
    i++;
  }
}

template <std::copyable T>
static void memory_set(T* mem, const T& element, ::boot::usize size) {
  if (mem == nullptr) {
    return;
  }

  ::boot::usize i = 0;

  while (i < size) {
    *(mem + i) = element;
    i++;
  }
}

namespace boot::graphics {

context::context()
    : background_color_(colors::black), text_color_(colors::white) {
  clear_screen();
}

void context::set_char(char code) {
  auto symbol = make_symbol(code, background_color_, text_color_);
  *(get_cursor()) = symbol;
}

void context::move_cursor_left() {
  if ((x_ += 1) == ROW_SIZE) {
    move_cursor_down();
    x_ = 0;
  }
}

void context::move_cursor_down() {
  if ((y_ += 1) == COLUMN_SIZE) {
    usize end = ROW_SIZE * (COLUMN_SIZE - 1);

    for (usize i = 0; i < end; i += ROW_SIZE) {
      auto ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START) + i;
      ::memory_copy(ptr + ROW_SIZE, ptr, ROW_SIZE);
    }

    auto ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START) + end;
    auto symbol = make_symbol(0x0, background_color_, text_color_);

    ::memory_set(ptr, symbol, ROW_SIZE);

    y_ = COLUMN_SIZE - 1;
  }
}

void context::clear_screen() {
  static_assert(VGA_MEMORY_START < VGA_MEMORY_END);

  auto ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START);
  auto symbol = make_symbol(0x0, background_color_, text_color_);

  ::memory_set(ptr, symbol, ROW_SIZE * COLUMN_SIZE);

  reset_cursor_x();
  reset_cursor_y();
}

}  // namespace boot::graphics
