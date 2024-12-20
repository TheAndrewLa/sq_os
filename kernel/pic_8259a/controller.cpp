// controller.cpp
#include "include/controller.h"

namespace kernel::pic_8259a {

void controller::send_command(uint8 command) {
  ports::outb(command_port_, command);
  ports::io_delay();
}

void controller::send_data(uint8 data) {
  ports::outb(data_port_, data);
  ports::io_delay();
}

}  // namespace kernel::pic_8259a
