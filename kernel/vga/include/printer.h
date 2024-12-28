// printer.h
#ifndef KERNEL_VGA_PRINTER_H
#define KERNEL_VGA_PRINTER_H

#include "../../include/utils.h"
#include "base_output.h"

namespace kernel::graphics {

struct printer : base_output {
  explicit printer(framebuffer& fb);
  printer(framebuffer& fb, colors text_color, colors back_color);

  printer& operator<<(char symbol);
  printer& operator<<(const char* string);

  template <typename T>
  printer& operator<<(T value)
    requires(std::is_arithmetic_v<T>)
  {
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

    return *this;
  }

  template <typename T>
  printer& operator<<(T* pointer) {
    return this->operator<<(reinterpret_cast<usize>(pointer));
  }

 private:
  void pre_stage();
  void post_stage();

  colors text_color_;
};

};  // namespace kernel::graphics

#endif  // (c) by andrew.la
