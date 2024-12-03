#ifndef BOOTLOADER_MEMORY_H
#define BOOTLOADER_MEMORY_H

#include <concepts>
#include <memory>
#include "defs.h"

namespace boot::memory {

template <std::copyable T>
void memory_copy(const T* src, T* dst, usize size) {
  if (src == nullptr || dst == nullptr) {
    return;
  }

  usize i = 0;
  while (i < size) {
    *(dst + i) = *(src + i);
    i++;
  }
}

template <std::copyable T>
void memory_set(T* mem, const T& element, usize size) {
  if (mem == nullptr) {
    return;
  }

  usize i = 0;

  while (i < size) {
    *(mem + i) = element;
    i++;
  }
}

template <typename T>
struct linear_allocator {

  using value_type = T;

  using size_type = usize;
  using difference_type = isize;

  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal = std::false_type;

  linear_allocator(void* addr, usize size)
      : start_(reinterpret_cast<T*>(addr)),
        current_(reinterpret_cast<T*>(addr)),
        end_(reinterpret_cast<T*>(addr) + size) {}

  linear_allocator(const linear_allocator&) = delete;
  linear_allocator(linear_allocator&&) = delete;

  ~linear_allocator() = default;

  linear_allocator& operator=(const linear_allocator&) = delete;
  linear_allocator& operator=(linear_allocator&&) = delete;

  bool operator==(const linear_allocator& other) {
    return start_ == other.start_ && end_ == other.end_;
  }

  T* allocate(usize n = 1) {
    if (current_ + n > end_) {
      return nullptr;
    } else {
      T* ptr = current_;
      current_ += n;

      return ptr;
    }
  }

  void deallocate(T* ptr) { return; }

 private:
  T* start_;
  T* current_;
  T* end_;
};

}  // namespace boot::memory

#endif  // (c) by andrew.la
