/* This file is a part of MIR project.
   Copyright (C) 2020-2021 Vladimir Makarov <vmakarov.gcc@gmail.com>.
*/

static char stdarg_str[]
  = "#ifndef __STDARG_H\n"
    "#define __STDARG_H\n"
    "\n"
    "typedef struct {\n"
    "  long __gpr, __fpr;\n"
    "  void *__overflow_arg_area;\n"
    "  void *__reg_save_area;\n"
    "} va_list[1];\n"
    "\n"
    "#define va_start(ap, param) __builtin_va_start (ap)\n"
    "#define va_arg(ap, type) __builtin_va_arg(ap, (type *) 0)\n"
    "#define va_end(ap) 0\n"
    "#define va_copy(dest, src) ((dest) = (src))\n"
    "\n"
    "/* For standard headers of a GNU system: */\n"
    "#ifndef __GNUC_VA_LIST\n"
    "#define __GNUC_VA_LIST 1\n"
    "#endif\n"
    "typedef va_list __gnuc_va_list;\n"
    "#endif /* #ifndef __STDARG_H */\n";
