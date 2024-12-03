// dispatcher.h
#ifndef BOOTLOADER_INTERRUPTS_DISPATCHER_H
#define BOOTLOADER_INTERRUPTS_DISPATCHER_H

#include "../../include/defs.h"

namespace boot::interrupts {

struct tramplin {
  tramplin(uint32 universal_handler_address, uint8 interrupt_vector);

  tramplin(const tramplin&) = default;
  tramplin(tramplin&&) = default;

  inline void remove_fake_error_code() {
    push_error_code_[0] = NOP;
    push_error_code_[1] = NOP;
  }

 private:
  static constexpr uint8 NOP = 0x90;

  static constexpr uint8 PUSH_BYTE = 0x6A;
  static constexpr uint8 PUSH_DWORD = 0x68;

  static constexpr uint8 RET = 0xC2;

  static constexpr uint8 CLI = 0xFA;

  uint8 clear_if_[1];
  uint8 push_error_code_[2];
  uint8 push_vector_[2];
  uint8 jump_procedure_[8];
  uint8 padding_[3];

} __attribute__((packed));

}  // namespace boot::interrupts

#endif  // (c) by andrew.la
