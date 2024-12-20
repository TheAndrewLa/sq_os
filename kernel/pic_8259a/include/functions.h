#ifndef KERNEL_PIC_8259A_CONFIG_H
#define KERNEL_PIC_8259A_CONFIG_H

#include "../../include/utils.h"
#include "controller.h"

namespace kernel::pic_8259a {

struct config {
  uint8 icw1;
  uint8 icw2;
  uint8 icw3;
  uint8 icw4;
};

constexpr controller PIC_MASTER_CONTROLLER = controller(types::master);
constexpr controller PIC_SLAVE_CONTROLLER = controller(types::slave);

}  // namespace kernel::pic_8259a

#endif  // (c) by andrew.la
