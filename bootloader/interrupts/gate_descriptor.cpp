// gate_descriptor.cpp
#include "include/gate_descriptor.h"

namespace boot::interrupts {

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

}  // namespace boot::interrupts
