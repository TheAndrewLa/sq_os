// printer.cpp
#include "include/printer.h"

namespace kernel::graphics {

printer::printer(framebuffer& fb)
    : base_output(fb), text_color_(colors::white) {}

printer::printer(framebuffer& fb, colors text_color, colors back_color)
    : base_output(fb), text_color_(text_color) {}

printer& printer::operator<<(const char* str) {
  pre_stage();
  puts(str);
  post_stage();

  return *this;
}

printer& printer::operator<<(char symbol) {
  pre_stage();
  putc(static_cast<uint8>(symbol));
  post_stage();

  return *this;
}

void printer::pre_stage() {
  save_text_color();
  set_text_color(text_color_);
}

void printer::post_stage() {
  restore_text_color();
}

}  // namespace kernel::graphics
