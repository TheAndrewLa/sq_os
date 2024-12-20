#ifndef KERNEL_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_H
#define KERNEL_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_H

#include <concepts>
#include <type_traits>

#include "../../../include/utils.h"

namespace kernel::memory {

template <typename T, usize N = 1>
struct linear_allocator {
  linear_allocator() = delete;

  linear_allocator(T* address, usize capacity) {
    start_ = address;
    end_ = address + capacity;
    current_ = address;
  }

  linear_allocator(const linear_allocator&) = default;

  linear_allocator(linear_allocator&& allocator) {
    start_ = allocator.start_;
    end_ = allocator.end_;
    current_ = allocator.current_;

    allocator.start_ = nullptr;
    allocator.end_ = nullptr;
  }

  ~linear_allocator() { clear_memory(); }

  linear_allocator& operator=(const linear_allocator&) = default;

  linear_allocator& operator=(linear_allocator&& allocator) {
    clear_memory();

    start_ = allocator.start_;
    end_ = allocator.end_;
    current_ = allocator.current_;

    allocator.start_ = nullptr;
    allocator.end_ = nullptr;

    return *this;
  }

  T* allocate() {
    if (current_ >= end_) {
      return nullptr;
    }

    return (current_++);
  }

  template <typename... Args>
  T* construct(Args&&... args)
    requires(std::is_constructible_v<T, Args...>)
  {
    if (current_ >= end_) {
      return nullptr;
    }

    auto* ret = allocate();

    for (usize i = 0; i < N; i++) {
      new (&ret[i])(T)(std::forward<Args>(args)...);
    }

    return ret;
  }

  void deallocate(T* ptr) {
    ptr->~T();
    return;
  }

 private:
  inline void clear_memory() {
    usize number = end_ - start_;
    memory_set<uint8>(reinterpret_cast<uint8*>(start_), 0, number * sizeof(T));
  }

  T* start_;
  T* current_;
  T* end_;
};

}  // namespace kernel::memory

#endif
