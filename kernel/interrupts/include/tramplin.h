// dispatcher.h
#ifndef BOOTLOADER_INTERRUPTS_DISPATCHER_H
#define BOOTLOADER_INTERRUPTS_DISPATCHER_H

#include "../../include/utils.h"

namespace kernel::interrupts {

struct tramplin {
  tramplin(uint32 universal_handler_address, uint8 interrupt_vector);

  tramplin(const tramplin&) = default;
  tramplin(tramplin&&) = default;

  inline void add_error_code() {
    push_error_code_[0] = NOP;
    push_error_code_[1] = NOP;
  }

 private:
  static constexpr uint8 NOP = 0x90;

  static constexpr uint8 PUSH_BYTE = 0x6A;
  static constexpr uint8 PUSH_DWORD = 0x68;

  static constexpr uint8 RET = 0xC3;

  // Check that everything is good
  static constexpr uint8 CLI = 0xFA;

  uint8 clear_if_[1];
  uint8 push_error_code_[2];
  uint8 push_vector_[2];
  uint8 jump_procedure_[6];
  uint8 padding_[5];

} __attribute__((packed));

}  // namespace kernel::interrupts

#endif  // (c) by andrew.la
