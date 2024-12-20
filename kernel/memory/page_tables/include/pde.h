#ifndef KERNEL_MEMORY_PAGE_TABLES_PDE_H
#define KERNEL_MEMORY_PAGE_TABLES_PDE_H

#include "../../../include/utils.h"

namespace kernel::memory {

enum class page_privilege : uint8 {
  supervisor = 0x0,
  user = 0x1,
};

enum class page_mode : uint8 {
  read = 0x0,
  read_write = 0x1,
};

struct pd_entry {
  pd_entry();
  ~pd_entry() = default;

  inline bool is_present() const { return static_cast<bool>(present_); }

  inline void make_present() { present_ = 0x1; }
  inline void make_absent() { present_ = 0x0; }

  inline void set_privilige(page_privilege privilege) {
    user_supervisor_ = static_cast<uint8>(privilege);
  }

  inline void set_mode(page_mode mode) {
    read_write_ = static_cast<uint8>(mode);
  }

  inline void make_global() { global_ = 0x1; }
  inline void make_local() { global_ = 0x0; }

  inline void make_executable() { nx_bit_ = 0x0; }
  inline void make_non_executable() { nx_bit_ = 0x1; }

  inline void clear_dirty_bit() { dirty_bit_ = 0x0; }
  inline void clear_access_bit() { access_bit_ = 0x0; }

  inline uint8 get_dirty_bit() const { return dirty_bit_; }
  inline uint8 get_access_bit() const { return access_bit_; }

  inline void set_address(wide_ptr address) { address_ = address >> 20; }
  inline wide_ptr get_address() const { return address_ << 20; }

 private:
  [[maybe_unused]] uint8 present_ : 1;
  [[maybe_unused]] uint8 read_write_ : 1;
  [[maybe_unused]] uint8 user_supervisor_ : 1;
  [[maybe_unused]] uint8 pwt_ : 1;
  [[maybe_unused]] uint8 pcd_ : 1;
  [[maybe_unused]] uint8 access_bit_ : 1;
  [[maybe_unused]] uint8 dirty_bit_ : 1;
  [[maybe_unused]] const uint8 __reserved0 : 1 = 0x1;
  [[maybe_unused]] uint8 global_ : 1;
  [[maybe_unused]] uint8 available_ : 2;
  [[maybe_unused]] uint8 pat_ : 1;
  [[maybe_unused]] const uint64 __reserved1 : 8 = 0x0;
  [[maybe_unused]] uint64 address_ : 32;
  [[maybe_unused]] const uint64 __reserved2 : 11 = 0x0;
  [[maybe_unused]] uint8 nx_bit_ : 1;
} __attribute__((packed));

}  // namespace kernel::memory

#endif
