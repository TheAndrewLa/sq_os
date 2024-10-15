// graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <utility>
#include "assert.h"
#include "defs.h"

namespace vga {

using coordinates = std::pair<usize, usize>;

enum class colors : u8 {
  black = 0x0,
  blue = 0x1,
  green = 0x2,
  cyan = 0x3,
  red = 0x4,
  purple = 0x5,
  brown = 0x6,
  gray = 0x7,

  dark_gray = 0x8,
  light_blue = 0x9,
  light_green = 0xA,
  light_cyan = 0xB,
  light_red = 0xC,
  light_purple = 0xD,
  yellow = 0xE,
  white = 0xF,
};

const usize memory_start = 0xB8000;
const usize memory_end = 0xB8FA0;

const usize line_size = 80;
const usize column_size = 25;

static coordinates cursor(0, 0);

inline void reset_cursor() {
  ::vga::cursor = coordinates(0, 0);
}

inline u16* get_cursor_ptr() {
  return reinterpret_cast<u16*>(::vga::memory_start) + ::vga::cursor.first +
         line_size * ::vga::cursor.second;
}

inline u16* get_cursor_ptr(const coordinates& cursor) {
  return reinterpret_cast<u16*>(::vga::memory_start) + cursor.first +
         line_size * cursor.second;
}

void clear_screen();

void putc(char symbol);
void putc(const coordinates& cursor, char symbol);

void puts(char* str);
void puts(const coordinates& cursor, char symbol);

void print(char* fmt, ...);
void print(const coordinates& cursor, char* fmt, ...);

}  // namespace vga

#endif  // (c) by andrew.la
