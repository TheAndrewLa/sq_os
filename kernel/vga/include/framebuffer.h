#ifndef KERNEL_VGA_FRAMEBUFFER_H
#define KERNEL_VGA_FRAMEBUFFER_H

#include "../../include/utils.h"
#include "context.h"

namespace kernel::graphics {

enum class border_styles : usize {
  none = 0,

  line = 1,
  double_line = 2,

  solid = 3,
};

struct frame {
  frame();
  frame(usize x, usize y);
  frame(usize x, usize y, usize width, usize height);

  usize x;
  usize y;
  usize width;
  usize height;
};

// TODO: add background color parameter to framebuffer

struct framebuffer {

  framebuffer() = delete;

  framebuffer(context& ctx, const frame& frame,
              colors text_color = colors::white);

  void print_char(uint8 symbol);

  void add_border(border_styles style);
  void add_border(border_styles style, colors color);

  void move_cursor_left();
  void move_cursor_down();

  inline void reset_cursor_x() { x_ = 0; }

  inline void reset_cursor_y() { y_ = 0; }

  inline colors& text_color() { return text_color_; }
  inline const colors& text_color() const { return text_color_; }

  void clear_buffer();

 private:
  static constexpr uint8 horizontal_borders[4] = {0x0, 0xC4, 0xCD, 0xB1};
  static constexpr uint8 vertical_borders[4] = {0x0, 0xB3, 0xBA, 0xB1};
  static constexpr uint8 top_left_corners[4] = {0x0, 0xDA, 0xC9, 0xB1};
  static constexpr uint8 top_right_corners[4] = {0x0, 0xBF, 0xBB, 0xB1};
  static constexpr uint8 bottom_left_corners[4] = {0x0, 0xC0, 0xC8, 0xB1};
  static constexpr uint8 bottom_right_corners[4] = {0x0, 0xD9, 0xBC, 0xB2};

  void draw_border();

  inline color_pair get_text_color() const {
    return color_pair(text_color_, colors::black);
  }

  inline color_pair get_border_color() const {
    return color_pair(border_color_, colors::black);
  }

  context& ctx_;

  usize x_;
  usize y_;

  usize offset_x_;
  usize offset_y_;

  usize width_;
  usize height_;

  colors text_color_;

  colors border_color_;
  usize border_style_index_;
};

}  // namespace kernel::graphics

#endif  // (c) by andrew.la
