// base_output.cpp
#include "include/base_output.h"

namespace boot::graphics {

base_output::base_output(context& ctx) : ctx_(ctx) {}

void base_output::putc(char symbol) {
  if (symbol >= '\x20') {
    ctx_.set_char(symbol);
    ctx_.move_cursor_left();
  } else {
    np_char(symbol);
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
      ctx_.reset_cursor_x();
    } break;

    case '\n': {
      ctx_.move_cursor_down();
      ctx_.reset_cursor_x();
    } break;

    case '\t': {
      ctx_.move_cursor_left();
      ctx_.move_cursor_left();
    } break;
  }
}

}  // namespace boot::graphics
