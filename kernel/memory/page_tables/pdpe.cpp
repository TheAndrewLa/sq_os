// pdpde.cpp
#include "include/pdpe.h"

namespace kernel::memory {

pdp_entry::pdp_entry()
    : present_(0x0), pwt_(0x0), pcd_(0x0), available_(0x0), address_(0x0) {}

pdp_entry::pdp_entry(pd_entry* directory_address) : pdp_entry() {
  enable(directory_address);
}

void pdp_entry::enable(pd_entry* directory_address) {
  address_ = (reinterpret_cast<uint64>(directory_address) >> 12);
  present_ = 0x1;
}

pd_entry* pdp_entry::disable() {
  auto* ret = reinterpret_cast<pd_entry*>(address_ << 12);

  present_ = 0x0;
  address_ = 0x0;

  return ret;
}

}  // namespace kernel::memory
