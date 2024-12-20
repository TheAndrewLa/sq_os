// generator.cpp
#include "include/generator.h"

namespace kernel::interrupts {

generator::generator(const desc& description) {
  static_assert(sizeof(idt_descriptor) == 6);
  static_assert(sizeof(gate_descriptor) == 8);
  static_assert(is_power_of_two<sizeof(tramplin)>::value);

  syscall_count_ = description.syscalls_count;
  total_count_ = description.syscalls_count + 0x20 + 0xF;

  gates_pointer_ =
      reinterpret_cast<gate_descriptor*>(description.gates_address);

  tramplins_pointer_ =
      reinterpret_cast<tramplin*>(description.tramplins_address);

  dispatcher_ = reinterpret_cast<uint32>(description.dispatcher_address);
}

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

  // Exceptions with real error are listed here:
  // https://osdev.wiki/wiki/Exceptions

  tramplins_pointer_[0x8].add_error_code();
  tramplins_pointer_[0xA].add_error_code();
  tramplins_pointer_[0xB].add_error_code();
  tramplins_pointer_[0xC].add_error_code();
  tramplins_pointer_[0xD].add_error_code();
  tramplins_pointer_[0xE].add_error_code();

  tramplins_pointer_[0x11].add_error_code();
  tramplins_pointer_[0x15].add_error_code();
  tramplins_pointer_[0x1D].add_error_code();
  tramplins_pointer_[0x1E].add_error_code();
}

void generator::load_idt() const {
  idt_descriptor desc(total_count_, gates_pointer_);
  desc.fill_idtr();
}

}  // namespace kernel::interrupts
