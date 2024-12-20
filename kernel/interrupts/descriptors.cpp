// descriptors.cpp
#include "include/descriptors.h"

namespace kernel::interrupts {

gate_descriptor::gate_descriptor(uint32 tramplin_address, gate_info info)
    : segment_selector_(0x8), info_(info) {
  static_assert(sizeof(gate_info) == 2);
  tramplin_address_low_ = tramplin_address & 0xFFFF;
  tramplin_address_high_ = (tramplin_address >> 0x10) & 0xFFFF;
  segment_selector_ = 0x8;
}

gate_descriptor::gate_descriptor(uint32 tramplin_address, gate_type type,
                                 gate_dpl dpl)
    : segment_selector_(0x8), info_(gate_info(type, dpl)) {
  static_assert(sizeof(gate_info) == 2);
  tramplin_address_low_ = tramplin_address & 0xFFFF;
  tramplin_address_high_ = (tramplin_address >> 0x10) & 0xFFFF;
}

idt_descriptor::idt_descriptor(usize gates_count, gate_descriptor* address)
    : size_(static_cast<uint16>(sizeof(gate_descriptor) * gates_count)),
      address_(reinterpret_cast<uint32>(address)) {}

}  // namespace kernel::interrupts
