// tramplin.cpp
#include "include/tramplin.h"

namespace kernel::interrupts {

tramplin::tramplin(uint32 universal_dispatcher_addr, uint8 interrupt_vector) {

  clear_if_[0] = CLI;

  push_error_code_[0] = PUSH_BYTE;
  push_error_code_[1] = 0x42;

  push_vector_[0] = PUSH_BYTE;
  push_vector_[1] = interrupt_vector;

  jump_procedure_[0] = PUSH_DWORD;
  jump_procedure_[1] = (universal_dispatcher_addr) & 0xFF;
  jump_procedure_[2] = (universal_dispatcher_addr >> 8) & 0xFF;
  jump_procedure_[3] = (universal_dispatcher_addr >> 16) & 0xFF;
  jump_procedure_[4] = (universal_dispatcher_addr >> 24) & 0xFF;

  jump_procedure_[5] = RET;

  padding_[0] = NOP;
  padding_[1] = NOP;
  padding_[2] = NOP;
}

}  // namespace kernel::interrupts
