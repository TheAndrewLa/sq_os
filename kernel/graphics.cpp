#include "graphics.h"

static usize strlen(const char* str) {
  const char* ptr = str;

  while (*ptr != 0x0)
    ptr++;

  return static_cast<usize>(ptr - str);
}

void vga::clear_screen() {
  u16* ptr = reinterpret_cast<u16*>(::vga::memory_start);
  u16* end = reinterpret_cast<u16*>(::vga::memory_end);

  while (ptr < end) {
    *ptr = 0x0;
    ptr++;
  }

  ::vga::reset_cursor();
}
