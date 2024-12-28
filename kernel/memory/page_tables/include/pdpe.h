#ifndef KERNEL_MEMORY_PAGE_TABLES_PDPE_H
#define KERNEL_MEMORY_PAGE_TABLES_PDPE_H

#include <variant>

#include "../../../include/utils.h"
#include "pde.h"

namespace kernel::memory {

struct pdp_entry {
  pdp_entry();
  explicit pdp_entry(pd_entry* directory_address);

  ~pdp_entry() = default;

  inline bool is_present() const { return static_cast<bool>(present_); }

  inline pd_entry* get_directories() const {
    return reinterpret_cast<pd_entry*>(address_ << 12);
  }

  void enable(pd_entry* directory_address);

  [[nodiscard]]
  pd_entry* disable();

 private:
  [[maybe_unused]] uint8 present_ : 1;
  [[maybe_unused]] const uint8 __reserved0 : 2 = 0x0;
  [[maybe_unused]] uint8 pwt_ : 1;
  [[maybe_unused]] uint8 pcd_ : 1;
  [[maybe_unused]] const uint8 __reserved1 : 4 = 0x0;
  [[maybe_unused]] uint8 available_ : 3;
  [[maybe_unused]] uint64 address_ : 40;
  [[maybe_unused]] const uint64 __reserved2 : 12 = 0x0;
} __attribute__((packed));

}  // namespace kernel::memory

#endif  // (c) by andrew.la
