#ifndef KERNEL_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
#define KERNEL_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H

#include <concepts>
#include <type_traits>

#include "../../../include/utils.h"

namespace kernel::memory {

template <typename T, usize N = 1>
struct pool_allocator {
  pool_allocator() = delete;

  pool_allocator(T* base, usize capacity) {
    base_ = base;
    end_ = base + (capacity * N);
    free_ = base;

    clear_memory();

    for (auto i = base; i < end_ - N; i += N) {
      *reinterpret_cast<T**>(i) = i + N;
    }
  }

  pool_allocator(const pool_allocator&) = default;

  pool_allocator(pool_allocator&& allocator) {
    base_ = allocator.base_;
    end_ = allocator.end_;
    free_ = allocator.free_;

    allocator.base_ = nullptr;
    allocator.end_ = nullptr;
  }

  ~pool_allocator() { clear_memory(); }

  pool_allocator& operator=(const pool_allocator&) = default;

  pool_allocator& operator=(pool_allocator&& allocator) {
    clear_memory();

    base_ = allocator.base_;
    end_ = allocator.end_;
    free_ = allocator.free_;

    allocator.base_ = nullptr;
    allocator.end_ = nullptr;

    return *this;
  }

  T* allocate() {
    if (free_ == nullptr) {
      return nullptr;
    }

    auto ret = free_;
    free_ = *reinterpret_cast<T**>(free_);

    return ret;
  }

  template <typename... Args>
  T* construct(Args&&... args)
    requires(std::is_constructible_v<T, Args...>)
  {
    if (free_ == nullptr) {
      return nullptr;
    }

    auto* ret = allocate();

    for (usize i = 0; i < N; i++) {
      new (&ret[i])(T)(std::forward<Args>(args)...);
    }

    return ret;
  }

  void deallocate(T* ptr) {
    if ((ptr == nullptr) || (ptr < base_) || (ptr >= end_) ||
        (!is_aligned(ptr))) {
      return;
    }

    ptr->~T();

    *reinterpret_cast<T**>(ptr) = free_;
    free_ = ptr;
  }

 private:
  inline void clear_memory() {
    usize number = end_ - base_;
    memory_set<uint8>(reinterpret_cast<uint8*>(base_), 0, number * sizeof(T));
  }

  T* base_;
  T* end_;
  T* free_;
};

}  // namespace kernel::memory

#endif  // (c) by andrew.la
