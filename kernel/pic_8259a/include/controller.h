#ifndef KERNEL_PIC_8259A_CONTROLLER_H
#define KERNEL_PIC_8259A_CONTROLLER_H

#include "../../include/utils.h"

namespace kernel::pic_8259a {

enum class types : uint16 {
  master = 0x20,
  slave = 0xA0,
};

enum class devices : uint16 {
  timer = 0x0001,

  keyboard = 0x0002,
  mouse = 0x1000,

  com1 = 0x0010,
  com2 = 0x0008,

  lpt1 = 0x0080,
  lpt2 = 0x0020,

  floppy_disk = 0x0040,
  primary_ata_disk = 0x4000,
  secondary_ata_disk = 0x8000,

  cmos_timer = 0x0100,

  coprocessor = 0x2000,
};

struct controller {
  constexpr controller(types type)
      : command_port_(static_cast<uint16>(type)),
        data_port_(static_cast<uint16>(type) + 1) {}

  void send_command(uint8 command);
  void send_data(uint8 data);

 private:
  uint16 command_port_;
  uint16 data_port_;
};

}  // namespace kernel::pic_8259a

#endif  // (c) by andrew.la
