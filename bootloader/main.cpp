// main.cpp
#include "include/defs.h"
#include "include/ports.h"

#include "graphics/include/context.h"
#include "graphics/include/printer.h"

#include "interrupts/include/generator.h"

// =========================================================
// 0x100000 = exact address where kernel should be loaded!
// =========================================================

// =========================================================
// collect_context = the first function in kernel code
// =========================================================

#define PIC_MASTER_COMMAND (0x20)
#define PIC_MASTER_DATA (0x21)

#define PIC_SLAVE_COMMAND (0xA0)
#define PIC_SLAVE_DATA (0xA1)

const char* msg_offset = R"(            )";
const char* msg_line01 = R"( ___          ___    ___ )";
const char* msg_line02 = R"(/ __|  __ _  / _ \  / __|)";
const char* msg_line03 = R"(\__ \ / _` || (_) | \__ \)";
const char* msg_line04 = R"(|___/ \__, | \___/  |___/)";
const char* msg_line05 = R"(         |_|             )";

using namespace boot;

graphics::printer* global_printer;

void panic();

extern "C" void loader_main() {
  graphics::context ctx;
  graphics::printer printer(ctx);

  global_printer = &printer;

  graphics::prefix_printer info(
      ctx, graphics::colors::white, graphics::colors::black, "[INFO]",
      graphics::colors::light_blue, graphics::colors::black);

  graphics::prefix_printer success(
      ctx, graphics::colors::white, graphics::colors::black, "[SUCCESS]",
      graphics::colors::light_green, graphics::colors::black);

  graphics::prefix_printer fatal(
      ctx, graphics::colors::white, graphics::colors::black, "[FATAL]",
      graphics::colors::black, graphics::colors::red);

  printer << '\n';
  printer << msg_offset << msg_line01 << '\n';
  printer << msg_offset << msg_line02 << '\n';
  printer << msg_offset << msg_line03 << '\n';
  printer << msg_offset << msg_line04 << '\n';
  printer << msg_offset << msg_line05 << '\n' << '\n' << '\n';

  info << "Welcome to SqOS!" << '\n';

  auto assert = [&fatal](bool cond, const char* message) noexcept {
    if (!cond) {
      fatal << "ASSERTION FAILED." << ' ' << message << '\n';
      ASM_CLI();
      ASM_ONELINE("jmp .");
    }
  };

  //  <----------  TODO  ----------->
  //  <=============================>
  //      Use CPUID
  //      Fill CPU_INFO structure
  //  <=============================>

  //  <----------  TODO  ----------->
  //  <=============================>
  //      Write small ATA driver
  //      Read kernel from disk
  //  <=============================>

  interrupts::generator::desc desc;
  desc.gates_address = 0x100040;
  desc.tramplins_address = 0x100840;
  desc.universal_dispatcher_address = reinterpret_cast<usize>(&panic);
  desc.syscalls_count = 0x20;

  interrupts::generator generator(assert, desc);

  generator.fill_gates();
  generator.fill_tramplins();
  generator.load_idt();

  success << "Interrupt tables were set up!" << '\n';

  //  <---------------  TODO  ---------------->
  //  <   Brand-new GDT                       >
  //  <=======================================>
  //      Layout:
  //       - Kernel mode code segment
  //       - Kernel mode data segment
  //       --------------------------
  //       - User mode code segment
  //       - User mode data segment
  //  <=======================================>

  uint8 icw1 = 0b00010001;
  uint8 icw4 = 0b00000001;

  uint8 icw2_master = 0x20;
  uint8 icw2_slave = 0x28;

  uint8 icw3_master = 0b00000100;
  uint8 icw3_slave = 0x2;

  ports::outb(PIC_MASTER_COMMAND, icw1);
  ports::outb(PIC_SLAVE_COMMAND, icw1);
  ports::io_delay();

  ports::outb(PIC_MASTER_DATA, icw2_master);
  ports::outb(PIC_SLAVE_DATA, icw2_slave);
  ports::io_delay();

  ports::outb(PIC_MASTER_DATA, icw3_master);
  ports::outb(PIC_SLAVE_DATA, icw3_slave);
  ports::io_delay();

  ports::outb(PIC_MASTER_DATA, icw4);
  ports::outb(PIC_SLAVE_DATA, icw4);
  ports::io_delay();

  ports::outb(PIC_MASTER_DATA, 0x0);
  ports::outb(PIC_SLAVE_DATA, 0x0);
  ports::io_delay();

  ports::outb(PIC_MASTER_DATA, 0b11111001);
  ports::outb(PIC_SLAVE_DATA, 0b11111111);
  ports::io_delay();

  success << "Intel 8259a was set up!" << '\n';

  //  <---------------  TODO  ---------------->
  //  <   Maybe move it to kernel             >
  //  <=======================================>
  //      Get total memory size (with CPUID)
  //      Set up page tables
  //      Enable PAE
  //      Set some control registers
  //  <=======================================>

  info << "OS preparing was finished!" << '\n';

  ASM_STI();
  ASM_ONELINE("jmp .");
}

void panic() {
  global_printer->operator<<("PANIC: Unhandled interrupt!\n");
  ASM_ONELINE("jmp .");
}
