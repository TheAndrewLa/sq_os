// address_space.cpp
#include "include/address_space.h"
#include "../vga/include/printer.h"

extern kernel::graphics::printer* global_printer;

namespace kernel::memory {

virtual_address_space::virtual_address_space(
    pool_allocator<pdp_entry, 0x4>* pdp_allocator,
    pool_allocator<pd_entry, 0x200>* pd_allocator)
    : pdp_allocator_(pdp_allocator), pd_allocator_(pd_allocator) {
  pdp_table_ = pdp_allocator_->construct();
}

void virtual_address_space::apply() const {
  registers::write_cr3(reinterpret_cast<uint32>(pdp_table_));
}

void virtual_address_space::release() {
  pd_allocator_->deallocate(pdp_table_[0].disable());
  pd_allocator_->deallocate(pdp_table_[1].disable());
  pd_allocator_->deallocate(pdp_table_[2].disable());
  pd_allocator_->deallocate(pdp_table_[3].disable());

  pdp_allocator_->deallocate(pdp_table_);
}

pd_entry* virtual_address_space::find_pde(usize pdp_index, usize pd_index) {
  if (!pdp_table_[pdp_index].is_present()) {
    pdp_table_[pdp_index].enable(pd_allocator_->construct());
  }

  while (pdp_index < 4) {
    pdp_entry& pdp = pdp_table_[pdp_index];

    if (!pdp.is_present()) {
      pdp.enable(pd_allocator_->construct());
    }

    pd_entry* pd_tables = pdp.get_directories();

    while (pd_index < 512 && pd_tables[pd_index].is_present()) {
      pd_index++;
    }

    if (pd_index < 512) {
      return &pd_tables[pd_index];
    } else {
      pdp_index++;
      pd_index = 0;
    }
  }

  // blue screen actually (stopping work)

  global_printer->operator<<(
      "A limit of virtual address space has been reached!\n");

  ASM_CLI();
  ASM_ONELINE("jmp .");

  return nullptr;
}

void virtual_address_space::map_address(const mapping_args& args,
                                        void* virtual_address,
                                        wide_ptr physical_address) {
  auto addr = reinterpret_cast<usize>(virtual_address);

  auto pdp_index = (addr >> 29) & (0x3);
  auto pd_index = (addr >> 20) & (0x1FF);

  pd_entry* pde = find_pde(pdp_index, pd_index);

  pde->set_address(physical_address);

  pde->set_privilige((args.user_mode) ? page_privilege::user
                                      : page_privilege::supervisor);

  pde->set_mode((args.is_writable) ? page_mode::read_write : page_mode::read);

  pde->make_present();

  if (args.is_executable) {
    pde->make_executable();
  } else {
    pde->make_non_executable();
  }
}

}  // namespace kernel::memory
