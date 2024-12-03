// ports.h
#ifndef BOOTLOADER_PORTS_H
#define BOOTLOADER_PORTS_H

// +==================+
// |      PORTS       |
// +==================+

#include "defs.h"

namespace boot::ports {

extern "C" uint8 inb(uint16 port);
extern "C" uint16 inw(uint16 port);

extern "C" void outb(uint16 port, uint8 byte);
extern "C" void outw(uint16 port, uint16 word);

inline void io_delay() {
  outw(0x80, 0x0);
}

}  // namespace boot::ports

#endif  // (c) by andrew.la
