// framebuffer.cpp
#include "include/framebuffer.h"

static constexpr kernel::usize min(kernel::usize a, kernel::usize b) {
  return (a < b) ? a : b;
}

namespace kernel::graphics {

frame::frame()
    : x(0),
      y(0),
      width(context::MAX_ROW_SIZE),
      height(context::MAX_COLUMN_SIZE) {}

frame::frame(usize x, usize y) {
  if ((x >= context::MAX_ROW_SIZE) || (y >= context::MAX_COLUMN_SIZE)) {
    frame();
  } else {
    this->x = x;
    this->y = y;
    this->width = context::MAX_ROW_SIZE - x;
    this->height = context::MAX_COLUMN_SIZE - y;
  }
}

frame::frame(usize x, usize y, usize width, usize height) {
  if ((x >= context::MAX_ROW_SIZE) || (y >= context::MAX_COLUMN_SIZE)) {
    frame();
  } else {
    this->x = x;
    this->y = y;
    this->width = min(80 - x, width);
    this->height = min(25 - y, height);
  }
}

framebuffer::framebuffer(context& ctx, const frame& frame, colors text_color)
    : ctx_(ctx),
      offset_x_(frame.x),
      offset_y_(frame.y),
      width_(frame.width),
      height_(frame.height),
      text_color_(text_color),
      border_color_(colors::black),
      border_style_index_(0x0) {}

void framebuffer::print_char(uint8 symbol) {
  ctx_.set_cursor(offset_x_ + x_, offset_y_ + y_);
  ctx_.set_char(symbol, get_text_color());

  move_cursor_left();
}

void framebuffer::move_cursor_left() {
  if ((x_ += 1) == width_) {
    move_cursor_down();
    reset_cursor_x();
  }
}

void framebuffer::move_cursor_down() {
  if ((y_ += 1) == height_) {

    usize x_start = offset_x_;
    usize y_start = offset_y_;

    usize y_end = offset_y_ + height_ - 1;
    usize x_end = offset_x_ + width_;

    for (usize y = y_start; y < y_end; y++) {
      for (usize x = x_start; x < x_end; x++) {
        ctx_.set_cursor(x, y + 1);
        uint16 pixel = ctx_.get_cursor_value();

        ctx_.set_cursor(x, y);
        ctx_.set_cursor_value(pixel);
      }
    }

    y_ = height_ - 1;
    x_ = 1;
  }
}

void framebuffer::clear_buffer() {
  auto symbol = make_symbol(0x0, colors::black, colors::black);

  for (usize i = 0; i < height_; i++) {
    for (usize j = 0; j < width_; j++) {
      ctx_.set_cursor(offset_x_ + j, offset_y_ + i);
      ctx_.set_cursor_value(symbol);
    }
  }
}

void framebuffer::add_border(border_styles style) {
  border_color_ = text_color_;
  border_style_index_ = static_cast<usize>(style);
  draw_border();
}

void framebuffer::add_border(border_styles style, colors color) {
  if (width_ < 3 || height_ < 3) [[unlikely]] {
    return;
  }

  border_color_ = color;
  border_style_index_ = static_cast<usize>(style);
  draw_border();
}

void framebuffer::draw_border() {
  auto border = get_border_color();

  ctx_.set_cursor(offset_x_, offset_y_);
  ctx_.set_char(top_left_corners[border_style_index_], border);

  ctx_.set_cursor(offset_x_ + width_ - 1, offset_y_);
  ctx_.set_char(top_right_corners[border_style_index_], border);

  ctx_.set_cursor(offset_x_, offset_y_ + height_ - 1);
  ctx_.set_char(bottom_left_corners[border_style_index_], border);

  ctx_.set_cursor(offset_x_ + width_ - 1, offset_y_ + height_ - 1);
  ctx_.set_char(bottom_right_corners[border_style_index_], border);

  for (usize i = 1; i < width_ - 1; i++) {
    ctx_.set_cursor(offset_x_ + i, offset_y_);
    ctx_.set_char(horizontal_borders[border_style_index_], border);

    ctx_.set_cursor(offset_x_ + i, offset_y_ + height_ - 1);
    ctx_.set_char(horizontal_borders[border_style_index_], border);
  }

  for (usize i = 1; i < height_ - 1; i++) {
    ctx_.set_cursor(offset_x_, offset_y_ + i);
    ctx_.set_char(vertical_borders[border_style_index_], border);

    ctx_.set_cursor(offset_x_ + width_ - 1, offset_y_ + i);
    ctx_.set_char(vertical_borders[border_style_index_], border);
  }

  reset_cursor_x();
  reset_cursor_y();

  offset_x_ += 1;
  offset_y_ += 1;

  width_ -= 2;
  height_ -= 2;
}

}  // namespace kernel::graphics
