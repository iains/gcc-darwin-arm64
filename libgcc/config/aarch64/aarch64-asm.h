/* AArch64 asm definitions.
   Copyright (C) 2023-2024 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include "auto-target.h"

/* Marking variant PCS symbol references is important for PLT calls
   otherwise it is for documenting the PCS in the symbol table.  */
#ifdef HAVE_AS_VARIANT_PCS
# define variant_pcs(name) .variant_pcs name
#else
# define variant_pcs(name)
#endif

/* GNU_PROPERTY_AARCH64_* macros from elf.h for use in asm code.  */
#define FEATURE_1_AND 0xc0000000
#define FEATURE_1_BTI 1
#define FEATURE_1_PAC 2

/* Supported features based on the code generation options.  */
#if defined(__ARM_FEATURE_BTI_DEFAULT)
# define BTI_FLAG FEATURE_1_BTI
# define BTI_C hint	34
#else
# define BTI_FLAG 0
# define BTI_C
#endif

#if __ARM_FEATURE_PAC_DEFAULT & 3
# define PAC_FLAG FEATURE_1_PAC
# define PACIASP hint	25; .cfi_window_save
# define AUTIASP hint	29; .cfi_window_save
#else
# define PAC_FLAG 0
# define PACIASP
# define AUTIASP
#endif

#define PASTE2(a, b) PASTE2a(a, b)
#define PASTE2a(a, b) a ## b

#ifdef __USER_LABEL_PREFIX__
# define ASMNAME(name) PASTE2(__USER_LABEL_PREFIX__, name)
#else
# define ASMNAME(name) name
#endif

#ifdef __ELF__
#define L(label) .L ## label
#define HIDDEN(name) .hidden name
#define GLOBAL(name) .global name
#define SYMBOL_SIZE(name) .size name, .-name
#define SYMBOL_TYPE(name, _type) .type name, _type
#elif __APPLE__
#define L(label) L ## label
#define HIDDEN(name) .private_extern name
#define GLOBAL(name) .globl name
#define SYMBOL_SIZE(name)
#define SYMBOL_TYPE(name, _type)
#else
#define L(label) .L ## label
#define HIDDEN(name)
#define GLOBAL(name) .global name
#define SYMBOL_SIZE(name)
#define SYMBOL_TYPE(name, _type)
#endif

/* Add a NT_GNU_PROPERTY_TYPE_0 note.  */
#define GNU_PROPERTY(type, value)	\
  .section .note.gnu.property, "a";	\
  .p2align 3;				\
  .word 4;				\
  .word 16;				\
  .word 5;				\
  .asciz "GNU";				\
  .word type;				\
  .word 4;				\
  .word value;				\
  .word 0;				\
  .previous

#if defined(__linux__) || defined(__FreeBSD__)
/* Do not require executable stack.  */
.section .note.GNU-stack, "", %progbits
.previous

/* Add GNU property note if built with branch protection.  */
# if (BTI_FLAG|PAC_FLAG) != 0
GNU_PROPERTY (FEATURE_1_AND, BTI_FLAG|PAC_FLAG)
# endif
#endif

.macro	ENTRY_ALIGNP2m, name, align
	.text
	.p2align \align
	GLOBAL (\name)
	SYMBOL_TYPE(\name, %function)
\name:
	.cfi_startproc
	BTI_C
.endm

#define ENTRY(name) ENTRY_ALIGNP2m name, 4

.macro	ENDm, name
	.cfi_endproc
	SYMBOL_SIZE (\name)
.endm
