// base_output.h
#ifndef BOOTLOADER_GRAPHICS_BASIC_OUTPUT_H
#define BOOTLOADER_GRAPHICS_BASIC_OUTPUT_H

// +=================================+
// |           BASE_OUTPUT           |
// +=================================+

#include "../../include/utils.h"
#include "context.h"

namespace kernel::graphics {

struct base_output {
  explicit base_output(context& ctx);

  /*
    Basic IO functions
    Will be used by different printers to do their stuff
  */

  void putc(char symbol);
  void puts(const char* str);

 protected:
  /*
    Functions for saving some context information (will be used by derived class)
  */

  // TODO: use context's stack for saving color

  inline void set_text_color(colors color) { ctx_.text_color() = color; }
  inline void save_text_color() { saved_text_color_ = ctx_.text_color(); }
  inline void restore_text_color() { ctx_.text_color() = saved_text_color_; }

  inline void set_back_color(colors color) { ctx_.back_color() = color; }
  inline void save_back_color() { saved_back_color_ = ctx_.back_color(); }
  inline void restore_back_color() { ctx_.back_color() = saved_back_color_; }

  // TODO: think about encapsulation of context

 protected:
  context& ctx_;

 private:
  void np_char(char symbol);

  colors saved_text_color_ = colors::white;
  colors saved_back_color_ = colors::black;
};

}  // namespace kernel::graphics

#endif
