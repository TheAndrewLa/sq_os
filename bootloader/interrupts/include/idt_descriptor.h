// idt_descriptor.h
#ifndef BOOTLOADER_INTERRUPTS_IDT_DESCRIPTOR_H
#define BOOTLOADER_INTERRUPTS_IDT_DESCRIPTOR_H

#include "../../include/defs.h"
#include "gate_descriptor.h"

namespace boot::interrupts {

struct idt_descriptor {
  idt_descriptor(usize gates_count, gate_descriptor* address);

  idt_descriptor(const idt_descriptor&) = default;
  idt_descriptor(idt_descriptor&&) = default;

  inline void fill_idtr() const { ASM_ONELINE("lidt %0" ::"m"(*this)); }

 private:
  [[maybe_unused]] uint16 size_;
  [[maybe_unused]] uint32 address_;
} __attribute__((packed));

}  // namespace boot::interrupts

#endif  // (c) by andrew.la
