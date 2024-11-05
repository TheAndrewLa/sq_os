
#include "graphics/include/context.h"

#include "graphics/include/prefix_printer.h"
#include "graphics/include/printer.h"

#include "include/defs.h"

const char* msg_space = "                      ";

const char* msg_line01 = R"(+--------------------------------+)";
const char* msg_line02 = R"(|    _____        ____   _____   |)";
const char* msg_line03 = R"(|   / ____|      / __ \ / ____|  |)";
const char* msg_line04 = R"(|  | (___   __ _| |  | | (___    |)";
const char* msg_line05 = R"(|   \___ \ / _` | |  | |\___ \   |)";
const char* msg_line06 = R"(|   ____) | (_| | |__| |____) |  |)";
const char* msg_line07 = R"(|  |_____/ \__, |\____/|_____/   |)";
const char* msg_line08 = R"(|             | |                |)";
const char* msg_line09 = R"(|             |_|                |)";
const char* msg_line10 = R"(+--------------------------------+)";

extern "C" void loader_main() {
  boot::graphics::context ctx;

  boot::graphics::printer printer(ctx, boot::graphics::colors::white);

  printer << msg_space << msg_line01 << '\n';
  printer << msg_space << msg_line02 << '\n';
  printer << msg_space << msg_line03 << '\n';
  printer << msg_space << msg_line04 << '\n';
  printer << msg_space << msg_line05 << '\n';
  printer << msg_space << msg_line06 << '\n';
  printer << msg_space << msg_line07 << '\n';
  printer << msg_space << msg_line08 << '\n';
  printer << msg_space << msg_line09 << '\n';
  printer << msg_space << msg_line10 << '\n' << '\n';

  printer << static_cast<boot::uint8>(100) << '\n';
  printer << static_cast<boot::uint16>(200) << '\n';
  printer << '\t' << static_cast<boot::uint64>(1024) << '\n';

  /*
  print_info(ctx, "Welcome to SqOS!");
  print_success(ctx, "PAE is available");
  print_error(ctx, "Network is not configured");
  print_success(ctx, "FAT32 driver is configured");
  print_fatal(ctx, "Long mode is not available!");
  */

  // TODO: printing small logo, then
  //  1) Check PAE
  //  2) Read kernel from filesystem
  //  3) Reserve memory for IDT
  //  4) Set up PIC
  //  5) Enable PAE + Set CR4 register
  //  6) Set up paging (page level 4)

  ASM_ONELINE("jmp .");
}
