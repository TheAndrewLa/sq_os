// descriptors.h
#ifndef KERNEL_INTERRUPTS_DESCRIPTORS_H
#define KERNEL_INTERRUPTS_DESCRIPTORS_H

#include "../../include/utils.h"

namespace kernel::interrupts {

enum class gate_type : uint8 {
  trap = 0xE,
  interrupt = 0xF,
};

enum class gate_dpl : uint8 {
  kernel = 0x0,
  user = 0x3,
};

struct gate_info {
  constexpr gate_info(gate_type type, gate_dpl dpl)
      : type_(static_cast<uint8>(type) & 0xF),
        dpl_(static_cast<uint8>(dpl) & 0x3) {}

  gate_info(const gate_info&) = default;
  gate_info(gate_info&&) = default;

  friend struct gate_descriptor;

 private:
  [[maybe_unused]] const uint8 __reserved0 = 0x0;
  [[maybe_unused]] uint8 type_ : 0x4;
  [[maybe_unused]] const uint8 __reserved1 : 0x1 = 0x0;
  [[maybe_unused]] uint8 dpl_ : 0x2;
  [[maybe_unused]] const uint8 __reserved2 : 0x1 = 0x1;
} __attribute__((packed));

constexpr gate_info EXCEPTION_GATE =
    gate_info(gate_type::trap, gate_dpl::kernel);

constexpr gate_info IO_GATE = gate_info(gate_type::interrupt, gate_dpl::kernel);

constexpr gate_info SYSCALL_GATE =
    gate_info(gate_type::interrupt, gate_dpl::user);

struct gate_descriptor {
  gate_descriptor(uint32 tramplin_address, gate_info info);
  gate_descriptor(uint32 tramplin_address, gate_type type, gate_dpl dpl);

  gate_descriptor(const gate_descriptor&) = default;
  gate_descriptor(gate_descriptor&&) = default;

 private:
  uint16 tramplin_address_low_;
  uint16 segment_selector_;
  gate_info info_;
  uint16 tramplin_address_high_;
} __attribute__((packed));

struct idt_descriptor {
  idt_descriptor(usize gates_count, gate_descriptor* address);

  idt_descriptor(const idt_descriptor&) = default;
  idt_descriptor(idt_descriptor&&) = default;

  inline void fill_idtr() const { ASM_ONELINE("lidt %0" ::"m"(*this)); }

 private:
  [[maybe_unused]] uint16 size_;
  [[maybe_unused]] uint32 address_;
} __attribute__((packed));

}  // namespace kernel::interrupts

#endif  // (c) by andrew.la