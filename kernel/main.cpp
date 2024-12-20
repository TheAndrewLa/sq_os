// main.cpp

#include "include/utils.h"

#include "vga/include/context.h"
#include "vga/include/printer.h"

#include "interrupts/include/generator.h"

#include "memory/include/address_space.h"

#include "memory/allocators/include/linear_allocator.h"
#include "memory/allocators/include/pool_allocator.h"

#include "memory/page_tables/include/pde.h"
#include "memory/page_tables/include/pdpe.h"

#define PIC_MASTER_COMMAND (0x20)
#define PIC_MASTER_DATA (0x21)

#define PIC_SLAVE_COMMAND (0xA0)
#define PIC_SLAVE_DATA (0xA1)

const char* msg_offset = R"(  )";
const char* msg_line01 = R"( ___          ___    ___ )";
const char* msg_line02 = R"(/ __|  __ _  / _ \  / __|)";
const char* msg_line03 = R"(\__ \ / _` || (_) | \__ \)";
const char* msg_line04 = R"(|___/ \__, | \___/  |___/)";
const char* msg_line05 = R"(         |_|             )";

struct process_context {
  kernel::uint32 edi;
  kernel::uint32 esi;
  kernel::uint32 ebp;
  kernel::uint32 esp;
  kernel::uint32 ebx;
  kernel::uint32 edx;
  kernel::uint32 ecx;
  kernel::uint32 eax;

  alignas(kernel::uint32) kernel::uint16 gs;
  alignas(kernel::uint32) kernel::uint16 fs;
  alignas(kernel::uint32) kernel::uint16 es;
  alignas(kernel::uint32) kernel::uint16 ds;

  alignas(kernel::uint32) kernel::uint8 vector;

  kernel::uint32 error_code;
};

kernel::graphics::printer* global_printer;
kernel::usize global_counter;

extern "C" void kernel_main() {
  using namespace kernel;

  graphics::context ctx;
  graphics::printer printer(ctx);

  global_printer = &printer;
  global_counter = 0;

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
  printer << msg_offset << msg_line05 << '\n';

  interrupts::generator::desc desc;

  desc.gates_address = 0x30000;
  desc.tramplins_address = 0x30800;
  desc.dispatcher_address = 0x8200;
  desc.syscalls_count = 0x20;

  interrupts::generator generator(desc);

  generator.fill_gates();
  generator.fill_tramplins();
  generator.load_idt();

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

  ports::outb(PIC_MASTER_DATA, 0b11111000);
  ports::outb(PIC_SLAVE_DATA, 0b11111111);
  ports::io_delay();

  //  <---------------  TODO  ---------------->
  //  <=======================================>
  //      Get total memory size
  //      Set up page tables
  //      Set up kernel allocator
  //      Enable PAE
  //      Set some control registers
  //  <=======================================>

  memory::pool_allocator<memory::pdp_entry, 0x4> pdp_allocator(
      reinterpret_cast<memory::pdp_entry*>(0x40000), 256);

  memory::pool_allocator<memory::pd_entry, 0x200> pd_allocator(
      reinterpret_cast<memory::pd_entry*>(0x100000), 256);

  memory::virtual_address_space kernel_addresses(&pdp_allocator, &pd_allocator);

  memory::mapping_args args = {
      .is_writable = true, .is_executable = true, .user_mode = false};

  kernel_addresses.map_address(args, 0x0, 0x0);
  kernel_addresses.apply();

  //  <----------  TODO  ----------->
  //  <=============================>
  //      Write small ATA driver
  //      Read program from disk
  //  <=============================>

  uint32 cr4 = registers::read_cr4();
  registers::write_cr4(cr4 | (1 << 5) | (1 << 4));

  printer << kernel_addresses.get_pdp() << '\n';
  printer << kernel_addresses.get_pdp()->get_directories() << '\n';

  registers::write_cr3(reinterpret_cast<uint32>(kernel_addresses.get_pdp()));

  // uint32 cr0 = registers::read_cr0();
  // registers::write_cr0(cr0 | (1 << 31));

  ASM_STI();
  ASM_ONELINE("jmp .");
}

void timer_handler() {
  global_printer->operator<<("Timer: ");
  global_printer->operator<<(global_counter);
  global_printer->operator<<('\n');

  global_counter++;

  kernel::ports::outb(PIC_MASTER_COMMAND, 0x20);
}

void keyboard_handler() {
  kernel::uint8 scan_code = kernel::ports::inb(0x60);
  global_printer->operator<<(scan_code);
  global_printer->operator<<('\n');

  kernel::ports::outb(PIC_MASTER_COMMAND, 0x20);
}

extern "C" void handler(process_context* ctx) {
  switch (ctx->vector) {
    case 0x20: {
      timer_handler();
    } break;

    case 0x21: {
      keyboard_handler();
    } break;

    default: {
      global_printer->operator<<("UNHANDLED: ");
      global_printer->operator<<(ctx->vector);
      ASM_ONELINE("jmp .");
    }
  }

  return;
}
