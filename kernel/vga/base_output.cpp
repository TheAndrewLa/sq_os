// base_output.cpp
#include "include/base_output.h"

namespace kernel::graphics {

base_output::base_output(framebuffer& fb) : fb_(fb) {}

void base_output::putc(char symbol) {
  uint8 unsigned_symbol = static_cast<uint8>(symbol);

  if (unsigned_symbol >= '\x20') {
    fb_.print_char(unsigned_symbol);
  } else {
    np_char(unsigned_symbol);
  }
}

void base_output::puts(const char* str) {
  if (str == nullptr) {
    return;
  }

  usize i = 0;

  while (str[i] != '\0') {
    putc(str[i++]);
  }
}

void base_output::np_char(char symbol) {
  switch (symbol) {
    case '\r': {
      fb_.reset_cursor_x();
    } break;

    case '\n': {
      fb_.move_cursor_down();
      fb_.reset_cursor_x();
    } break;

    case '\t': {
      fb_.move_cursor_left();
      fb_.move_cursor_left();
    } break;
  }
}

}  // namespace kernel::graphics
