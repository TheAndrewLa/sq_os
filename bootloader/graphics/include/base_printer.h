// base_printer.h
#ifndef BOOTLOADER_GRAPHICS_BASE_PRINTER_H
#define BOOTLOADER_GRAPHICS_BASE_PRINTER_H

// +=================================+
// |           BASE PRINTER          |
// +=================================+

#include "../../include/defs.h"
#include "base_output.h"
#include "context.h"

#include <type_traits>

namespace boot::graphics {

template <typename derived_printer, typename return_printer = derived_printer>
struct base_printer : base_output {
  base_printer() = delete;

  return_printer& operator<<(char symbol) {
    static_cast<derived_printer*>(this)->pre_stage();
    putc(symbol);
    static_cast<derived_printer*>(this)->post_stage();

    return reinterpret_cast<return_printer&>(*this);
  }

  return_printer& operator<<(const char* string) {
    static_cast<derived_printer*>(this)->pre_stage();
    puts(string);
    static_cast<derived_printer*>(this)->post_stage();

    return reinterpret_cast<return_printer&>(*this);
  }

  template <typename T>
  return_printer& operator<<(T value)
    requires(std::is_arithmetic_v<T>)
  {
    static_cast<derived_printer*>(this)->pre_stage();

    puts("0x");

    constexpr isize initial_shift = (8 * sizeof(T)) - 4;

    for (isize i = initial_shift; i >= 0; i -= 4) {
      char digit = (value >> i) & 0b1111;

      if (digit < 0xA) {
        putc(digit + '0');
      } else {
        putc(digit - 0xA + 'A');
      }
    }

    static_cast<derived_printer*>(this)->post_stage();

    return reinterpret_cast<return_printer&>(*this);
  }

  template <typename T>
  return_printer& operator<<(T* pointer) {
    return this->operator<<(reinterpret_cast<usize>(pointer));
  }

 protected:
  base_printer(context& ctx) : base_output(ctx) {}
  base_printer(const base_printer&) = default;
  base_printer(base_printer&&) = default;

  ~base_printer() = default;
};

}  // namespace boot::graphics

#endif  // (c) by andrew.la
