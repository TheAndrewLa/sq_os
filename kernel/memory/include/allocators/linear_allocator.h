#ifndef KERNEL_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_H
#define KERNEL_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_H

#include <concepts>
#include <type_traits>

#include "../../../include/utils.h"
#include "../../../vga/include/blue_screen.h"

extern kernel::graphics::blue_screen* g_blue_screen;

namespace kernel::memory {

template <typename T, usize N = 1>
struct linear_allocator {
  linear_allocator() = delete;

  linear_allocator(T* address, usize capacity) {
    start_ = address;
    end_ = address + (capacity * N);
    current_ = address;
  }

  linear_allocator(const linear_allocator&) = delete;

  linear_allocator(linear_allocator&& allocator) {
    start_ = allocator.start_;
    end_ = allocator.end_;
    current_ = allocator.current_;

    allocator.start_ = nullptr;
  }

  ~linear_allocator() { clear_memory(); }

  linear_allocator& operator=(const linear_allocator&) = delete;
  linear_allocator& operator=(linear_allocator&& allocator) = delete;

  T* allocate() {
    if (current_ >= end_) {
      g_blue_screen->invoke("Allocation at linear allocator has failed!");
      return nullptr;
    }

    auto* ret = current_;
    current_ += N;

    return ret;
  }

  template <typename... Args>
  T* construct(Args&&... args)
    requires(std::is_constructible_v<T, Args...>)
  {
    if (current_ >= end_) {
      g_blue_screen->invoke("Allocation at linear allocator has failed!");
      return nullptr;
    }

    auto* ret = allocate();

    for (usize i = 0; i < N; i++) {
      new (&ret[i])(T)(std::forward<Args>(args)...);
    }

    return ret;
  }

  void deallocate(T* ptr) {
    if ((ptr == nullptr) || (ptr < start_) || (ptr >= end_) ||
        (!is_aligned<T, N>(ptr))) {
      return;
    }

    for (usize i = 0; i < N; i++) {
      ptr[i].~T();
    }

    return;
  }

 private:
  inline void clear_memory() {
    if (start_ == nullptr) {
      return;
    }

    usize number = end_ - start_;
    memory_set<uint8>(reinterpret_cast<uint8*>(start_), 0, number * sizeof(T));
  }

  T* start_;
  T* current_;
  T* end_;
};

}  // namespace kernel::memory

#endif  // (c) by andrew.la
