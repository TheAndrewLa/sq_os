// context.h
#ifndef KERNEL_VGA_CONTEXT_H
#define KERNEL_VGA_CONTEXT_H

#include <utility>
#include "../../include/utils.h"

namespace kernel::graphics {

enum class colors : uint16 {
  black = 0x0,
  blue = 0x1,
  green = 0x2,
  cyan = 0x3,
  red = 0x4,
  purple = 0x5,
  brown = 0x6,
  gray = 0x7,

  dark_gary = 0x8,
  light_blue = 0x9,
  light_green = 0xA,
  light_cyan = 0xB,
  light_red = 0xC,
  light_purple = 0xD,
  yellow = 0xE,
  white = 0xF,
};

using color_pair = std::pair<colors, colors>;

inline uint16 make_symbol(uint8 code, colors text_color, colors back_color) {
  auto character = static_cast<uint16>(code);
  auto text = static_cast<uint16>(text_color) & 0b1111;
  auto back = static_cast<uint16>(back_color) & 0b111;
  return (character) | (text << 8) | (back << 12);
}

struct context final {
  context();
  context(const context&) = delete;
  context(context&&) = delete;

  context& operator=(const context&) = delete;
  context& operator=(context&&) = delete;

  inline void set_cursor(usize x, usize y) {
    x_ = x;
    y_ = y;
  }

  inline void move_cursor(isize delta_x, isize delta_y) {
    x_ += delta_x;
    y_ += delta_y;
  }

  inline uint16 get_cursor_value() {
    static_assert(VGA_MEMORY_START < VGA_MEMORY_END);
    auto* ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START);

    return *(ptr + x_ + (y_ * MAX_ROW_SIZE));
  }

  inline void set_cursor_value(uint16 value) {
    static_assert(VGA_MEMORY_START < VGA_MEMORY_END);
    auto* ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START);

    *(ptr + x_ + (y_ * MAX_ROW_SIZE)) = value;
  }

  void set_char(uint8 code, const color_pair& color);

  void clear_screen();
  void clear_screen(colors color);

  static constexpr usize MAX_ROW_SIZE = 80;
  static constexpr usize MAX_COLUMN_SIZE = 25;

 private:
  static constexpr usize VGA_MEMORY_START = 0xB8000;
  static constexpr usize VGA_MEMORY_END = 0xB8FA0;

  usize x_;
  usize y_;
};

};  // namespace kernel::graphics

#endif  // (c) by andrew.la
