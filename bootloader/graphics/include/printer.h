// printer.h
#ifndef BOOTLOADER_GRAPHICS_PRINTER_H
#define BOOTLOADER_GRAPHICS_PRINTER_H

#include "../../include/defs.h"
#include "context.h"

namespace boot::graphics {

struct printer {
  printer(context& ctx);

 private:
  context& ctx_;
};

};  // namespace boot::graphics

#endif  // (c) by andrew.la
