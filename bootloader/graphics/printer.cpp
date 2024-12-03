// printer.cpp
#include "include/printer.h"

static boot::usize string_length(const char* str) {
  if (str == nullptr) {
    return 0;
  }

  boot::usize len = 0;
  while (str[len++] != 0x0) {}

  return len - 1;
}

namespace boot::graphics {

printer::printer(context& ctx)
    : base_printer<printer>(ctx),
      text_color_(colors::white),
      back_color_(colors::black) {}

printer::printer(context& ctx, colors text_color, colors back_color)
    : base_printer<printer>(ctx),
      text_color_(text_color),
      back_color_(back_color) {}

void printer::pre_stage() {
  save_text_color();
  save_back_color();

  set_text_color(text_color_);
  set_back_color(back_color_);
}

void printer::post_stage() {
  restore_text_color();
  restore_back_color();
}

prefix_printer::prefix_printer(context& ctx, colors text_color,
                               colors back_color, const char* prefix_text,
                               colors prefix_text_color,
                               colors prefix_back_color)
    : base_printer<prefix_printer, printer>(ctx),
      text_color_(text_color),
      back_color_(back_color),
      prefix_text_(prefix_text),
      prefix_length_(string_length(prefix_text)),
      prefix_text_color_(prefix_text_color),
      prefix_back_color_(prefix_back_color) {}

prefix_printer::prefix_printer(context& ctx)
    : prefix_printer(ctx, colors::white, colors::black, nullptr, colors::black,
                     colors::black) {}

prefix_printer::prefix_printer(context& ctx, colors text_color,
                               colors back_color)
    : prefix_printer(ctx, text_color, back_color, nullptr, colors::black,
                     colors::black) {}

void prefix_printer::pre_stage() {
  save_text_color();
  save_back_color();

  set_text_color(prefix_text_color_);
  set_back_color(prefix_back_color_);

  putc(' ');
  puts(prefix_text_);

  for (usize i = prefix_length_; i < GLOBAL_PREFIX_LENGTH; i++) {
    putc(' ');
  }

  set_text_color(text_color_);
  set_back_color(back_color_);

  putc(' ');
}

void prefix_printer::post_stage() {
  restore_text_color();
  restore_back_color();
}

}  // namespace boot::graphics
