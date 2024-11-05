// printer.cpp
#include "include/printer.h"
#include "include/prefix_printer.h"

namespace boot::graphics {

printer::printer(context& ctx, colors color)
    : base_printer<printer>(ctx), color_(color) {}

void printer::before_print() {
  prev_color_ = ctx_.get_text_color();
  ctx_.set_text_color(color_);
}

void printer::after_print() {
  ctx_.set_text_color(prev_color_);
}

}  // namespace boot::graphics
