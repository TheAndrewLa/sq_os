// context.h
#ifndef BOOTLOADER_GRAPHICS_CONTEXT_H
#define BOOTLOADER_GRAPHICS_CONTEXT_H

#include "../../include/defs.h"

namespace boot::graphics {

enum class colors : ::boot::uint16 {
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

constexpr ::boot::uint16 make_symbol(char code, colors back_color,
                                     colors text_color);

struct context {
  context() = default;
  context(const context&) = delete;
  context(context&&) = delete;

  context& operator=(const context&) = delete;
  context& operator=(context&&) = delete;

  inline colors& background_color() { return background_color_; }
  inline colors& text_color() { return text_color_; }

  inline const colors& background_color() const { return background_color_; }
  inline const colors& text_color() const { return text_color_; }

  inline uint16* get_cursor() {
    static_assert(VGA_MEMORY_START < VGA_MEMORY_END);

    auto offset = x_ + (y_ * ROW_SIZE);
    auto ptr = reinterpret_cast<::boot::uint16*>(VGA_MEMORY_START);

    return ptr + offset;
  }

  void move_cursor_left();
  void move_cursor_down();

  void clear_screen();

 private:
  colors background_color_ = colors::black;
  colors text_color_ = colors::white;

  ::boot::usize x_ = 0;
  ::boot::usize y_ = 0;

  static constexpr ::boot::usize VGA_MEMORY_START = 0xB8000;
  static constexpr ::boot::usize VGA_MEMORY_END = 0xB8FA0;

  static constexpr ::boot::usize ROW_SIZE = 80;
  static constexpr ::boot::usize COLUMN_SIZE = 25;
};

};  // namespace boot::graphics

#endif  // (c) by andrew.la
