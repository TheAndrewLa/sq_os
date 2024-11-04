// variadic_args.h
#ifndef BOOTLOADER_VARIADIC_ARGS_H
#define BOOTLOADER_VARIADIC_ARGS_H

#define va_list char*
#define va_start(list, last_arg) list = (char*)(&last_arg) + sizeof(last_arg)

#define va_arg(list, type) \
  *(type*)(list);          \
  list += sizeof(type)

#define va_end(list)

#endif  // (c) by andrew.la
