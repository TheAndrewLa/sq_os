// idt_descriptor.cpp
#include "include/idt_descriptor.h"
#include "include/gate_descriptor.h"

namespace boot::interrupts {

idt_descriptor::idt_descriptor(usize gates_count, gate_descriptor* address)
    : size_(static_cast<uint16>(sizeof(gate_descriptor) * gates_count)),
      address_(reinterpret_cast<uint32>(address)) {}

}  // namespace boot::interrupts
