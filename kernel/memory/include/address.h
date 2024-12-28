#ifndef KERNEL_MEMORY_ADDRESS_H
#define KERNEL_MEMORY_ADDRESS_H

#include "../../include/utils.h"

namespace kernel::memory {

struct virtual_address {
  constexpr virtual_address(usize addr)
      : offset_((addr) & (0xFFF)),
        pt_index_((addr >> 12) & (0x1FF)),
        pd_index_((addr >> 21) & (0x1FF)),
        pdp_index_((addr >> 30) & (0x3)) {}

  inline usize pt_index() const { return pt_index_; }
  inline usize pd_index() const { return pd_index_; }
  inline usize pdp_index() const { return pdp_index_; }

 private:
  [[maybe_unused]] usize offset_ : 12;

  usize pt_index_ : 9;
  usize pd_index_ : 9;
  usize pdp_index_ : 2;
} __attribute__((packed));

}  // namespace kernel::memory

#endif  // (c) by andrew.la
