// printer.h
#ifndef BOOTLOADER_GRAPHICS_PRINTER_H
#define BOOTLOADER_GRAPHICS_PRINTER_H

// +==================================+
// |     PRINTER & PREFIX_PRINTER     |
// +==================================+

#include "../../include/defs.h"
#include "base_output.h"
#include "base_printer.h"

namespace boot::graphics {

struct printer final : base_printer<printer> {
  explicit printer(context& ctx);
  printer(context& ctx, colors text_color, colors back_color);

  void pre_stage();
  void post_stage();

 private:
  colors text_color_;
  colors back_color_;
};

struct prefix_printer final : base_printer<prefix_printer, printer> {
  explicit prefix_printer(context& ctx);
  prefix_printer(context& ctx, colors text_color, colors back_color);

  prefix_printer(context& ctx, colors text_color, colors back_color,
                 const char* prefix, colors prefix_text_color,
                 colors prefix_back_color);

  void pre_stage();
  void post_stage();

 private:
  static constexpr usize GLOBAL_PREFIX_LENGTH = 10;

  // These fields has to be first fields of prefix_printer
  // Because of converting to printer

  // ========================================

  colors text_color_;
  colors back_color_;

  // ========================================

  const char* prefix_text_;
  usize prefix_length_;

  colors prefix_text_color_;
  colors prefix_back_color_;
};

};  // namespace boot::graphics

#endif  // (c) by andrew.la
