#ifndef KERNEL_GRAPHICS_BLUE_SCREEN_H
#define KERNEL_GRAPHICS_BLUE_SCREEN_H

#include "context.h"

namespace kernel::graphics {

struct blue_screen {
  blue_screen() = delete;
  blue_screen(context& ctx);

  void invoke(const char* message);

 private:
  inline void put_char(char symbol) {
    ctx_.set_char(symbol, color_pair(TEXT_COLOR, BACK_COLOR));
    ctx_.move_cursor(1, 0);
  }

  void put_string(const char* message);
  void put_string(const char* message, usize number);

  static constexpr colors BACK_COLOR = colors::blue;
  static constexpr colors TEXT_COLOR = colors::white;

  static constexpr usize PREFIX_LENGTH = 2;

  static constexpr usize TITLE_HEIGHT = 5;
  static constexpr usize TITLE_WIDTH = 45;

  static constexpr char TITLE_CHARS[TITLE_HEIGHT][TITLE_WIDTH] = {
      R"(        _                    _              )",
      R"( _   _ | |__           ___  | |__           )",
      R"(| | | || '_ \  _____  / _ \ | '_ \          )",
      R"(| |_| || | | ||_____|| (_) || | | | _  _  _ )",
      R"( \__,_||_| |_|        \___/ |_| |_|(_)(_)(_))"};

  context& ctx_;
};

}  // namespace kernel::graphics

#endif  // (c) by andrew.la
