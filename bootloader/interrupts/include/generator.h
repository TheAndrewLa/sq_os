// generator.h
#ifndef BOOTLOADER_INTERRUPTS_GENERATOR_H
#define BOOTLOADER_INTERRUPTS_GENERATOR_H

#include <functional>
#include <type_traits>

#include "gate_descriptor.h"
#include "idt_descriptor.h"
#include "tramplin.h"

namespace boot::interrupts {

struct generator {
  struct desc {
    usize gates_address;
    usize tramplins_address;
    usize universal_dispatcher_address;

    usize syscalls_count;
  };

  template <typename Fn>
  generator(const Fn& assert_fn, const desc& description)
    requires(std::is_nothrow_invocable_v<Fn, bool, const char*>)
  {
    // Size of tramplin body has to be power of 2
    static_assert((sizeof(tramplin) & (sizeof(tramplin) - 1)) == 0);

    static_assert(sizeof(idt_descriptor) == 6);
    static_assert(sizeof(gate_descriptor) == 8);

    syscall_count_ = description.syscalls_count;
    total_count_ = description.syscalls_count + 0x20 + 0xF;

    // TODO: check that addresses aren't overlapping

    std::invoke(assert_fn, total_count_ <= 0xFF, "Too many syscalls!");

    gates_pointer_ =
        reinterpret_cast<gate_descriptor*>(description.gates_address);

    tramplins_pointer_ =
        reinterpret_cast<tramplin*>(description.tramplins_address);

    dispatcher_ =
        reinterpret_cast<uint32>(description.universal_dispatcher_address);
  }

  generator(const generator&) = delete;
  generator(generator&&) = delete;

  void fill_gates() const;
  void fill_tramplins() const;

  void load_idt() const;

 private:
  usize total_count_;
  usize syscall_count_;

  gate_descriptor* gates_pointer_;
  tramplin* tramplins_pointer_;

  uint32 dispatcher_;
};

}  // namespace boot::interrupts

#endif  // (c) by andrew.la
