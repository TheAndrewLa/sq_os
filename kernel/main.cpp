// main.cpp

#include "include/cpu_features.h"
#include "include/utils.h"

#include "vga/include/context.h"
#include "vga/include/framebuffer.h"
#include "vga/include/printer.h"

#include "interrupts/include/generator.h"

#include "memory/include/address_space.h"

#include "memory/include/allocators/linear_allocator.h"
#include "memory/include/allocators/pool_allocator.h"

#include "memory/page_tables/include/pde.h"
#include "memory/page_tables/include/pdpe.h"

#include "scheduler/include/task.h"

#define PIC_MASTER_COMMAND (0x20)
#define PIC_MASTER_DATA (PIC_MASTER_COMMAND + 0x1)

#define PIC_SLAVE_COMMAND (0xA0)
#define PIC_SLAVE_DATA (PIC_SLAVE_COMMAND + 0x1)

kernel::graphics::printer* g_printer_left;
kernel::graphics::printer* g_printer_tr;
kernel::graphics::printer* g_printer_br;

kernel::graphics::blue_screen* g_blue_screen;

kernel::usize g_counter;

extern "C" void kernel_main() {
  using namespace kernel;

  uint32 nFrequence = 1000;

  uint32 Div;
  uint8 tmp;

  Div = 1193180 / nFrequence;

  ports::outb(0x43, 0xB6);
  ports::outb(0x42, static_cast<uint8>(Div));
  ports::outb(0x42, static_cast<uint8>(Div >> 8));

  tmp = ports::inb(0x61);

  if (tmp != (tmp | 3)) {
    ports::outb(0x61, tmp | 3);
  }

  /*

  graphics::context ctx;

  graphics::blue_screen bs(ctx);

  graphics::framebuffer fb_left(ctx, graphics::frame(0, 0, 30, 25));
  graphics::framebuffer fb_tr(ctx, graphics::frame(30, 0, 50, 12));
  graphics::framebuffer fb_br(ctx, graphics::frame(30, 12, 50, 13));

  fb_left.add_border(graphics::border_styles::line, graphics::colors::gray);
  fb_tr.add_border(graphics::border_styles::line, graphics::colors::green);
  fb_br.add_border(graphics::border_styles::line, graphics::colors::red);

  graphics::printer printer_left(fb_left);
  graphics::printer printer_tr(fb_tr);
  graphics::printer printer_br(fb_br);

  g_printer_left = &printer_left;
  g_printer_br = &printer_br;
  g_printer_tr = &printer_tr;

  g_blue_screen = &bs;

  interrupts::generator::desc desc;

  desc.gates_address = 0x1E800;
  desc.tramplins_address = 0x1F000;
  desc.dispatcher_address = 0x8200;

  interrupts::generator generator(desc);

  generator.fill_gates();
  generator.fill_tramplins();
  generator.load_idt();

  constexpr uint8 icw1 = 0b00010001;
  constexpr uint8 icw4 = 0b00000001;

  constexpr uint8 icw2_master = 0x20;
  constexpr uint8 icw2_slave = 0x28;

  constexpr uint8 icw3_master = 0b00000100;
  constexpr uint8 icw3_slave = 0x2;

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

  ports::outb(PIC_MASTER_DATA, 0b11111010);
  ports::outb(PIC_SLAVE_DATA, 0b11111111);
  ports::io_delay();

  memory::pool_allocator<memory::pdp_entry, 0x4> pdp_allocator(
      reinterpret_cast<memory::pdp_entry*>(0x100000), 0x80);

  memory::pool_allocator<memory::pd_entry, 0x200> pd_allocator(
      reinterpret_cast<memory::pd_entry*>(0x20000), 0x80);

  memory::virtual_address_space kernel_addresses(pdp_allocator, pd_allocator);

  memory::mapping_args args = {
      .is_writable = true, .is_executable = true, .user_mode = false};

  constexpr usize _2mb = memory::megabytes(2);
  constexpr usize _4mb = memory::megabytes(4);
  constexpr usize _6mb = memory::megabytes(6);

  kernel_addresses.map_address(args, 0x0, 0x0);
  kernel_addresses.map_address(args, _2mb, _2mb);
  kernel_addresses.map_address(args, _4mb, _4mb);
  kernel_addresses.map_address(args, _6mb, _6mb);

  kernel_addresses.apply();

  cpu_features::enable_pae();
  cpu_features::enable_pse();

  cpu_features::enable_paging();
  
  ASM_STI();
  */
  ASM_ONELINE("jmp .");
}

void timer_handler(kernel::multitasking::task_context* ctx) {
  g_printer_left->operator<<("Timer value: ");
  g_printer_left->operator<<(g_counter);
  g_printer_left->operator<<('\n');

  g_counter++;

  kernel::ports::outb(PIC_MASTER_COMMAND, 0x20);
  kernel::multitasking::restore_context(ctx);
}

extern "C" void get_context(kernel::multitasking::task_context* ctx) {
  switch (ctx->vector) {
    case 0x20: {
      timer_handler(ctx);
    } break;

    default:
      g_blue_screen->invoke("Unhandled interrup!");
  }
}
