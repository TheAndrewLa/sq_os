#include "defs.h"
#include "graphics.h"

extern "C" void kernel_main() {
  vga::clear_screen();

  // TODO: printing small logo =>
  //  1) Check PAE
  //  2) Read kernel from filesystem
  //  3) Enable PAE + set some control registers
  //  4) Set up paging

  ASM_ONELINE("jmp .");
}
