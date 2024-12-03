// generator.cpp
#include "include/generator.h"
#include "include/idt_descriptor.h"

namespace boot::interrupts {

void generator::fill_gates() const {
  // Exceptions
  for (usize i = 0; i < 0x20; i++) {
    new (&gates_pointer_[i])(gate_descriptor)(
        reinterpret_cast<uint32>(&tramplins_pointer_[i]), EXCEPTION_GATE);
  }

  // IO
  for (usize i = 0x20; i < 0x2F; i++) {
    new (&gates_pointer_[i])(gate_descriptor)(
        reinterpret_cast<uint32>(&tramplins_pointer_[i]), IO_GATE);
  }

  // Syscalls
  for (usize i = 0x2F; i < 0x2F + syscall_count_; i++) {
    new (&gates_pointer_[i])(gate_descriptor)(
        reinterpret_cast<uint32>(&tramplins_pointer_[i]), SYSCALL_GATE);
  }
}

void generator::fill_tramplins() const {
  for (usize i = 0; i < total_count_; i++) {
    new (&tramplins_pointer_[i])(tramplin)(dispatcher_, static_cast<uint8>(i));
  }

  usize special_exceptions[] = {0x8, 0xA,  0xB,  0xC,  0xD,
                                0xE, 0x11, 0x15, 0x1D, 0x1E};

  for (const auto& i : special_exceptions) {
    tramplins_pointer_[i].remove_fake_error_code();
  }
}

void generator::load_idt() const {
  idt_descriptor desc(total_count_, gates_pointer_);
  desc.fill_idtr();
}

}  // namespace boot::interrupts
