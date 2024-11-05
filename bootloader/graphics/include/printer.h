// printer.h
#ifndef BOOTLOADER_GRAPHICS_PRINTER_H
#define BOOTLOADER_GRAPHICS_PRINTER_H

#include "../../include/defs.h"
#include "context.h"

#include <concepts>

namespace boot::graphics {

template <typename printer_class>  // TODO: add printer_class requirements
struct base_printer {
  base_printer(context& ctx) : ctx_(ctx) {}

  printer_class& operator=(const printer_class&) = delete;
  printer_class& operator=(printer_class&&) = delete;

  printer_class& operator<<(char symbol) {
    static_cast<printer_class*>(this)->before_print();
    display_char(symbol);
    static_cast<printer_class*>(this)->after_print();

    return static_cast<printer_class&>(*this);
  }

  printer_class& operator<<(const char* str) {
    static_cast<printer_class*>(this)->before_print();

    usize index = 0;
    while (str[index] != 0x0) {
      display_char(str[index++]);
    }

    static_cast<printer_class*>(this)->after_print();

    return static_cast<printer_class&>(*this);
  }

  template <typename T>
  printer_class& operator<<(T value) requires(std::is_arithmetic_v<T>) {
    static_cast<printer_class*>(this)->before_print();

    display_char('0');
    display_char('x');

    isize initial_shift = (8 * sizeof(T)) - 4;

    for (isize i = initial_shift; i >= 0; i -= 4) {
      char digit = (value >> i) & 0b1111;

      if (digit < 0xA) {
        display_char(digit + '0');
      } else {
        display_char(digit - 0xA + 'A');
      }
    }

    static_cast<printer_class*>(this)->after_print();

    return static_cast<printer_class&>(*this);
  }

 protected:
  void display_char(char symbol) {
    if (symbol >= 0x20) {
      ctx_.set_char(symbol);
      ctx_.move_cursor_left();
    } else {
      np_character(symbol);
    }
  }

  void np_character(char symbol) {
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

      case '\v': {
        ctx_.move_cursor_down();
        ctx_.move_cursor_down();
      } break;

      case ' ': {
        ctx_.move_cursor_left();
      } break;

      default: {
      } break;
    }
  }

  context& ctx_;
};

struct printer : base_printer<printer> {
  printer(context& ctx, colors color);

  printer(const printer&) = delete;
  printer(printer&&) = delete;

  void before_print();
  void after_print();

 private:
  colors color_;
  colors prev_color_{};
};

};  // namespace boot::graphics

#endif  // (c) by andrew.la
