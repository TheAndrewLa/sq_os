// blue_screen.cpp
#include "include/blue_screen.h"

namespace kernel::graphics {

blue_screen::blue_screen(context& ctx) : ctx_(ctx) {}

void blue_screen::invoke(const char* msg) {
  ctx_.clear_screen(BACK_COLOR);
  ctx_.set_cursor(0, 0);

  for (usize i = 0; i < TITLE_HEIGHT; i++) {
    ctx_.set_cursor(PREFIX_LENGTH, i);
    put_string(TITLE[i], TITLE_WIDTH);
  }

  ctx_.set_cursor(PREFIX_LENGTH, TITLE_HEIGHT + 2);

  put_string(msg);

  ASM_CLI();
  ASM_ONELINE("jmp .");
}

void blue_screen::put_string(const char* msg) {
  usize i = 0;
  while (msg[i] != '\0') {
    put_char(msg[i++]);
  }
}

void blue_screen::put_string(const char* msg, usize number) {
  for (usize i = 0; i < number; i++) {
    put_char(msg[i]);
  }
}

}  // namespace kernel::graphics
