#ifndef KERNEL_MULTITASKING_TASK_CONTEXT_H
#define KERNEL_MULTITASKING_TASK_CONTEXT_H

#include "../../include/utils.h"
#include "../../memory/include/address.h"
#include "../../memory/include/address_space.h"

#include "../../vga/include/base_output.h"
#include "../../vga/include/blue_screen.h"
#include "../../vga/include/framebuffer.h"

namespace kernel::multitasking {

struct task_context {
  uint32 edi;
  uint32 esi;
  uint32 ebp;
  uint32 esp;
  uint32 ebx;
  uint32 edx;
  uint32 ecx;
  uint32 eax;

  uint16 gs alignas(uint32);
  uint16 fs alignas(uint32);
  uint16 es alignas(uint32);
  uint16 ds alignas(uint32);

  uint8 vector alignas(uint32);

  uint32 error_code;

  // TSS here

  uint32 eip;
  uint16 cs alignas(uint32);
  uint32 eflags;
};

extern "C" void restore_context(task_context* ctx);

struct task {
  task() = delete;
  task(graphics::framebuffer& fb, void* stack, void* code);

  void continue_task();

  void set_context(task_context* ctx);
  task_context* get_context();

  void write(const char* str);

 private:
  graphics::base_output stdout_;
  task_context context_;
};

}  // namespace kernel::multitasking

#endif  // (c) by andrew.la
