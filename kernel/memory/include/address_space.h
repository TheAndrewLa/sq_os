#ifndef KERNEL_MEMORY_ADDRESS_SPACE_H
#define KERNEL_MEMORY_ADDRESS_SPACE_H

#include "../../include/utils.h"

#include "../page_tables/include/pde.h"
#include "../page_tables/include/pdpe.h"

#include "allocators/pool_allocator.h"

namespace kernel::memory {

struct mapping_args {
  bool is_writable;
  bool is_executable;

  bool user_mode;
};

struct virtual_address_space {
  using pdp_allocator = pool_allocator<pdp_entry, 0x4>;
  using pd_allocator = pool_allocator<pd_entry, 0x200>;

  virtual_address_space(pdp_allocator& pdp_alloc, pd_allocator& pd_alloc);

  void map_address(const mapping_args& args, usize virtual_address,
                   wide_ptr physical_address);

  inline const pdp_entry* pdp_table() const { return pdp_table_; }
  inline pdp_entry* pdp_table() { return pdp_table_; }

  void apply() const;
  void release();

 private:
  pd_entry& find_pde(usize pdp_index, usize pd_index);

  pdp_allocator& pdp_allocator_;
  pd_allocator& pd_allocator_;

  pdp_entry* pdp_table_;
};

}  // namespace kernel::memory

#endif  // (c) by andrew.la
