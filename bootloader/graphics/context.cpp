// context.cpp
#include "include/context.h"

namespace boot::graphics {

context::context() : back_color_(colors::black), text_color_(colors::white) {
  ports::outb(0x3D4, 0xA);
  ports::outb(0x3D5, 0x20);

  clear_screen();
}

void context::set_char(char code) {
  auto symbol = make_symbol(code, back_color_, text_color_);
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
      ::boot::memory::memory_copy(ptr + ROW_SIZE, ptr, ROW_SIZE);
    }

    auto ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START) + end;
    auto symbol = make_symbol(0x0, back_color_, text_color_);

    ::boot::memory::memory_set(ptr, symbol, ROW_SIZE);

    y_ = COLUMN_SIZE - 1;
  }
}

void context::clear_screen() {
  static_assert(VGA_MEMORY_START < VGA_MEMORY_END);

  auto ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START);
  auto symbol = make_symbol(0x0, back_color_, text_color_);

  ::boot::memory::memory_set(ptr, symbol, ROW_SIZE * COLUMN_SIZE);

  reset_cursor_x();
  reset_cursor_y();
}

}  // namespace boot::graphics
