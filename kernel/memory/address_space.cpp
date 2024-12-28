// address_space.cpp
#include "include/address_space.h"

namespace kernel::memory {

virtual_address_space::virtual_address_space(
    pool_allocator<pdp_entry, 0x4>& pdp_alloc,
    pool_allocator<pd_entry, 0x200>& pd_alloc)
    : pdp_allocator_(pdp_alloc), pd_allocator_(pd_alloc) {
  pdp_table_ = pdp_allocator_.construct();
}

void virtual_address_space::apply() const {
  registers::write_cr3(pdp_table_);
}

void virtual_address_space::release() {
  pd_allocator_.deallocate(pdp_table_[0].disable());
  pd_allocator_.deallocate(pdp_table_[1].disable());
  pd_allocator_.deallocate(pdp_table_[2].disable());
  pd_allocator_.deallocate(pdp_table_[3].disable());

  pdp_allocator_.deallocate(pdp_table_);
}

pd_entry& virtual_address_space::find_pde(usize pdp_index, usize pd_index) {
  while (pdp_index < 0x4) {

    pdp_entry& pdp = pdp_table_[pdp_index];

    if (!pdp.is_present()) {
      pdp.enable(pd_allocator_.construct());
      return pdp.get_directories()[0];
    }

    pd_entry* pd_tables = pdp.get_directories();

    while (pd_index < 0x200 && pd_tables[pd_index].is_present()) {
      pd_index++;
    }

    if (pd_index < 0x200) {
      return pd_tables[pd_index];
    } else {
      pdp_index++;
      pd_index = 0;
    }
  }

  ASM_CLI();
  ASM_ONELINE("jmp .");
}

void virtual_address_space::map_address(const mapping_args& args,
                                        usize virtual_address,
                                        wide_ptr physical_address) {
  auto pdp_index = (virtual_address >> 29) & (0x3);
  auto pd_index = (virtual_address >> 21) & (0x1FF);

  pd_entry& pde = find_pde(pdp_index, pd_index);

  pde.set_address(physical_address);

  pde.set_privilige((args.user_mode) ? page_privilege::user
                                     : page_privilege::supervisor);

  pde.set_mode((args.is_writable) ? page_mode::read_write : page_mode::read);

  pde.set_nx_bit(!args.is_executable);

  pde.make_present();
}

}  // namespace kernel::memory
