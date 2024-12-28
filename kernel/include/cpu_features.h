#ifndef KERNEL_CPU_FEATURES_H
#define KERNEL_CPU_FEATURES_H

#include "utils.h"

namespace kernel::cpu_features {

inline void enable_pse() {
  uint32 cr4 = registers::read_cr4();
  registers::write_cr4(cr4 | (1 << 4));
}

inline void enable_pae() {
  uint32 cr4 = registers::read_cr4();
  registers::write_cr4(cr4 | (1 << 5));
}

inline void enable_paging() {
  uint32 cr0 = registers::read_cr0();
  registers::write_cr0(cr0 | (1 << 31));
}

}  // namespace kernel::cpu_features

#endif
