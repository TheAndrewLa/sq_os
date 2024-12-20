// context.h
#ifndef BOOTLOADER_GRAPHICS_CONTEXT_H
#define BOOTLOADER_GRAPHICS_CONTEXT_H

// +======================+
// |       CONTEXT        |
// +======================+

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

inline uint16 make_symbol(char code, colors back_color, colors text_color) {
  auto character = static_cast<uint16>(code);
  auto color1 = static_cast<uint16>(text_color) & 0b1111;
  auto color2 = static_cast<uint16>(back_color) & 0b111;
  return (character) | (color1 << 8) | (color2 << 12);
}

struct context {
  context();
  context(const context&) = delete;
  context(context&&) = delete;

  context& operator=(const context&) = delete;
  context& operator=(context&&) = delete;

  /*
    TODO: create context's stack (push_color/pop_color)
  */

  inline colors& back_color() { return back_color_; }
  inline const colors& back_color() const { return back_color_; }

  inline colors& text_color() { return text_color_; }
  inline const colors& text_color() const { return text_color_; }

  void set_char(char code);

  void move_cursor_left();
  void move_cursor_down();

  inline void reset_cursor_x() { x_ = 0; }
  inline void reset_cursor_y() { y_ = 0; }

  void clear_screen();

 private:
  static constexpr usize VGA_MEMORY_START = 0xB8000;
  static constexpr usize VGA_MEMORY_END = 0xB8FA0;

  static constexpr usize ROW_SIZE = 80;
  static constexpr usize COLUMN_SIZE = 25;

  static constexpr usize STACK_SIZE = 32;

  inline uint16* get_cursor() {
    static_assert(VGA_MEMORY_START < VGA_MEMORY_END);
    auto ptr = reinterpret_cast<uint16*>(VGA_MEMORY_START);

    return ptr + x_ + (y_ * ROW_SIZE);
  }

  // TODO: remove it and work with stack

  colors back_color_;
  colors text_color_;

  usize x_;
  usize y_;
};

};  // namespace kernel::graphics

#endif  // (c) by andrew.la
