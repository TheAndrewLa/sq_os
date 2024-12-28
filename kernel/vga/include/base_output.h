// base_output.h
#ifndef KERNEL_VGA_BASE_OUTPUT_H
#define KERNEL_VGA_BASE_OUTPUT_H

// +=================================+
// |           BASE_OUTPUT           |
// +=================================+

#include "../../include/utils.h"
#include "framebuffer.h"

namespace kernel::graphics {

struct base_output {
  explicit base_output(framebuffer& fb);

  void putc(char symbol);
  void puts(const char* str);

 protected:
  inline void set_text_color(colors color) { fb_.text_color() = color; }
  inline void save_text_color() { saved_text_color_ = fb_.text_color(); }
  inline void restore_text_color() { fb_.text_color() = saved_text_color_; }

 protected:
  framebuffer& fb_;

 private:
  void np_char(char symbol);

  colors saved_text_color_ = colors::white;
};

}  // namespace kernel::graphics

#endif
