// pde.cpp
#include "include/pde.h"

namespace kernel::memory {

pd_entry::pd_entry()
    : present_(0x0),
      read_write_(0x1),
      user_supervisor_(0x0),
      pwt_(0x0),
      pcd_(0x0),
      access_bit_(0x0),
      dirty_bit_(0x0),
      global_(0x0),
      available_(0x0),
      pat_(0x0),
      address_(0x0),
      nx_bit_(0x0) {}

}  // namespace kernel::memory
