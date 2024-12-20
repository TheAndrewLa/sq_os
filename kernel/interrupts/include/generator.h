// generator.h
#ifndef KERNEL_INTERRUPTS_GENERATOR_H
#define KERNEL_INTERRUPTS_GENERATOR_H

#include <functional>
#include <type_traits>

#include "descriptors.h"
#include "tramplin.h"

namespace kernel::interrupts {

struct generator {
  struct desc {
    usize gates_address;
    usize tramplins_address;
    usize dispatcher_address;

    usize syscalls_count;
  };

  generator() = delete;

  generator(const desc& description);

  generator(const generator&) = delete;
  generator(generator&&) = delete;

  ~generator() = default;

  generator& operator=(const generator&) = delete;
  generator& operator=(generator&&) = delete;

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

}  // namespace kernel::interrupts

#endif  // (c) by andrew.la
