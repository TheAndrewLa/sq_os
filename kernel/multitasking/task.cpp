// task.cpp
#include "include/task.h"

namespace kernel::multitasking {

task::task(graphics::framebuffer& fb, void* stack, void* code) : stdout_(fb) {
  context_.cs = 0x8;
  context_.ds = 0x10;
  context_.es = 0x10;
  context_.gs = 0x10;
  context_.fs = 0x10;

  context_.eax = 0x0;
  context_.ebx = 0x0;
  context_.ecx = 0x0;
  context_.edx = 0x0;

  context_.esi = 0x0;
  context_.edi = 0x0;

  // Enable only interrupt flag & reserved one

  context_.eflags = (0x0200) | (0x0002);

  context_.error_code = 0x42;

  context_.eip = reinterpret_cast<uint32>(code);
  context_.esp = reinterpret_cast<uint32>(stack);

  context_.ebp = 0x0;
}

void task::set_context(task_context* ctx) {
  context_.cs = ctx->cs;
  context_.ds = ctx->ds;
  context_.es = ctx->es;
  context_.gs = ctx->gs;
  context_.fs = ctx->fs;

  context_.eax = ctx->eax;
  context_.ebx = ctx->ebx;
  context_.ecx = ctx->ecx;
  context_.edx = ctx->edx;

  context_.esi = ctx->esi;
  context_.edi = ctx->edi;

  context_.eflags = ctx->eflags;

  context_.error_code = ctx->error_code;

  context_.eip = ctx->eip;
  context_.esp = ctx->esp;

  context_.ebp = ctx->ebp;
}

task_context* task::get_context() {
  return &context_;
}

void task::continue_task() {
  context_.eflags |= 0x0200;
  restore_context(&context_);
}

void task::write(const char* str) {
  stdout_.puts(str);
}

}  // namespace kernel::multitasking
