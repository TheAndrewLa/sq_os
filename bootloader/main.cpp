
#include "graphics/include/context.h"
#include "include/defs.h"

/*
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

void print_info(boot::graphics::context& ctx, const char* string);
void print_error(boot::graphics::context& ctx, const char* string);
void print_fatal(boot::graphics::context& ctx, const char* string);
void print_success(boot::graphics::context& ctx, const char* string);
*/

extern "C" void loader_main() {
  boot::graphics::context ctx;

  ctx.background_color() = boot::graphics::colors::blue;
  ctx.clear_screen();

  /*
  boot::graphics::context ctx;

  ctx << msg_space << msg_line01 << '\n';
  ctx << msg_space << msg_line02 << '\n';
  ctx << msg_space << msg_line03 << '\n';
  ctx << msg_space << msg_line04 << '\n';
  ctx << msg_space << msg_line05 << '\n';
  ctx << msg_space << msg_line06 << '\n';
  ctx << msg_space << msg_line07 << '\n';
  ctx << msg_space << msg_line08 << '\n';
  ctx << msg_space << msg_line09 << '\n';
  ctx << msg_space << msg_line10 << '\n' << '\n';

  print_info(ctx, "Welcome to SqOS!");
  print_success(ctx, "PAE is available");
  print_error(ctx, "Network is not configured");
  print_success(ctx, "FAT32 driver is configured");
  print_fatal(ctx, "Long mode is not available!");

  // TODO: printing small logo, then
  //  1) Check PAE
  //  2) Read kernel from filesystem
  //  3) Reserve memory for IDT
  //  4) Set up PIC
  //  5) Enable PAE + Set CR4 register
  //  6) Set up paging (page level 4)

  */

  ASM_ONELINE("jmp .");
}

/*
void print_info(boot::graphics::context& ctx, const char* string) {
  ctx.set_text_color(boot::graphics::colors::light_blue);
  ctx << "[INFO]";

  ctx.set_text_color(boot::graphics::colors::white);
  ctx << "   " << "\t-\t" << string << '\n';
}

void print_error(boot::graphics::context& ctx, const char* string) {
  ctx.set_text_color(boot::graphics::colors::light_purple);
  ctx << "[ERROR]";

  ctx.set_text_color(boot::graphics::colors::white);
  ctx << "  " << "\t-\t" << string << '\n';
}

void print_success(boot::graphics::context& ctx, const char* string) {
  ctx.set_text_color(boot::graphics::colors::light_green);
  ctx << "[SUCCESS]";

  ctx.set_text_color(boot::graphics::colors::white);
  ctx << "\t-\t" << string << '\n';
}

void print_fatal(boot::graphics::context& ctx, const char* string) {
  ctx.set_text_color(boot::graphics::colors::light_red);
  ctx << "[FATAL]";

  ctx.set_text_color(boot::graphics::colors::white);
  ctx << "  " << "\t-\t" << string << '\n';
}
*/
