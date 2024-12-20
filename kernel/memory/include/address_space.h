#ifndef KERNEL_MEMORY_ADDRESS_SPACE_H
#define KERNEL_MEMORY_ADDRESS_SPACE_H

#include "../../include/utils.h"

#include "../page_tables/include/pde.h"
#include "../page_tables/include/pdpe.h"

#include "../allocators/include/pool_allocator.h"

namespace kernel::memory {

struct mapping_args {
  bool is_writable;
  bool is_executable;

  bool user_mode;
};

struct virtual_address_space {
  virtual_address_space() = delete;

  virtual_address_space(pool_allocator<pdp_entry, 0x4>* pdp_allocator,
                        pool_allocator<pd_entry, 0x200>* pd_allocator);

  inline pdp_entry* get_pdp() { return pdp_table_; }
  inline const pdp_entry* get_pdp() const { return pdp_table_; }

  void map_address(const mapping_args& args, void* virtual_address,
                   wide_ptr physical_address);

  void apply() const;
  void release();

 private:
  pd_entry* find_pde(usize pdp_index, usize pd_index);

  pool_allocator<pdp_entry, 0x4>* pdp_allocator_;
  pool_allocator<pd_entry, 0x200>* pd_allocator_;

  pdp_entry* pdp_table_;
};

}  // namespace kernel::memory

#endif
