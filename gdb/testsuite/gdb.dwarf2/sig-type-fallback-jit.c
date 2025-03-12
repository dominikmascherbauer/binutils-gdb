/* This test program is part of GDB, the GNU debugger.

   Copyright 2024 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Simulate loading of JIT code.  */

#include "../gdb.base/jit-elf-util.h"
#include "../gdb.base/jit-protocol.h"

/* Must be defined by .exp file when compiling to know
   what address to map the ELF binary to.  */
#ifndef LIB_ADDRESS
#error "Must define LOAD_ADDRESS"
#endif
#ifndef LIB_NAME_STRING
#error "Must define LIB_NAME_STRING"
#endif

int
main ()
{
  size_t obj_size;
  void *load_addr = (void *) (size_t) (LIB_ADDRESS);
  void *addr = load_elf (LIB_NAME_STRING, &obj_size, load_addr);

  /* Link entry at the end of the list.  */
  struct jit_code_entry *const entry = calloc (1, sizeof (*entry));
  entry->symfile_addr = (const char *) addr;
  entry->symfile_size = obj_size;
  __jit_debug_descriptor.relevant_entry = entry;
  __jit_debug_descriptor.first_entry = entry;

  /* Notify GDB.  */
  __jit_debug_descriptor.action_flag = JIT_REGISTER;
  __jit_debug_register_code ();

  // breakpoint 1

  /* Now unregister entry.  */
  /* Notify GDB.  */
  __jit_debug_descriptor.action_flag = JIT_UNREGISTER;
  __jit_debug_register_code ();

  __jit_debug_descriptor.first_entry = NULL;
  __jit_debug_descriptor.relevant_entry = NULL;
  free (entry);

  return 0;
}
