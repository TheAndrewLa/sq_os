// context.cpp
#include "include/context.h"

namespace kernel::graphics {

context::context() : x_(0), y_(0) {
  ports::outb(0x3D4, 0xA);
  ports::outb(0x3D5, 0x20);

  clear_screen();
}

void context::set_char(uint8 code, const color_pair& color) {
  auto symbol = make_symbol(code, color.first, color.second);
  set_cursor_value(symbol);
}

void context::clear_screen() {
  clear_screen(colors::black);
}

void context::clear_screen(colors color) {
  static_assert(VGA_MEMORY_START < VGA_MEMORY_END);

  auto ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START);
  auto symbol = make_symbol(0x0, color, color);

  memory::memory_set(ptr, symbol, MAX_ROW_SIZE * MAX_COLUMN_SIZE);
}

}  // namespace kernel::graphics
