/* Definition of cumulative_args_t, the opaque handle to a target's
   CUMULATIVE_ARGS.
   Copyright The GNU Toolchain Authors.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_CUMULATIVE_ARGS_H
#define GCC_CUMULATIVE_ARGS_H

#if CHECKING_P

struct cumulative_args_t { void *magic; void *p; };

#else /* !CHECKING_P */

/* When using a GCC build compiler, we could use
   __attribute__((transparent_union)) to get cumulative_args_t function
   arguments passed like scalars where the ABI would mandate a less
   efficient way of argument passing otherwise.  However, that would come
   at the cost of less type-safe !CHECKING_P compilation.  */

union cumulative_args_t { void *p; };

#endif /* !CHECKING_P */

#endif /* GCC_CUMULATIVE_ARGS_H */
