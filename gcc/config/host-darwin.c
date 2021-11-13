/* Darwin host-specific hook definitions.
   Copyright (C) 2003-2021 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "diagnostic-core.h"
#include "config/host-darwin.h"

/* For Darwin (macOS only) platforms, without ASLR (PIE) enabled on the
   binaries, the following VM addresses are expected to be available.
   NOTE, that for aarch64, ASLR is always enabled - but the VM address
   mentioned below is available (at least on Darwin20).

   The spaces should all have 512Mb available c.f. PCH files for large
   C++ or Objective-C in the range of 150Mb for 64b hosts.  */

#if defined(__x86_64) && defined(__LP64__)
# define TRY_EMPTY_VM_SPACE	0x1000000000
# define SAFE_ALLOC_SIZE	0x20000000
#elif defined(__x86_64)
# define TRY_EMPTY_VM_SPACE	0x6fe00000
# define SAFE_ALLOC_SIZE	0x20000000
#elif defined(__i386)
# define TRY_EMPTY_VM_SPACE	0x6fe00000
# define SAFE_ALLOC_SIZE	0x20000000
#elif defined(__POWERPC__) && defined(__LP64__)
# define TRY_EMPTY_VM_SPACE	0x1000000000
# define SAFE_ALLOC_SIZE	0x20000000
#elif defined(__POWERPC__)
# define TRY_EMPTY_VM_SPACE	0x6fe00000
# define SAFE_ALLOC_SIZE	0x20000000
#elif defined(__aarch64__)
# define TRY_EMPTY_VM_SPACE	0x0f80000000
# define SAFE_ALLOC_SIZE	0x20000000
#else
# error "unknown Darwin target"
#endif

/* Try to map a known position in the VM.  The current PCH implementation
   can adjust values at write-time, but not at read-time thus we need to
   pick up the same position when reading as we got at write-time.  */

void *
darwin_gt_pch_get_address (size_t sz, int fd ATTRIBUTE_UNUSED)
{
  if (sz > SAFE_ALLOC_SIZE)
    {
      error_at (input_location, "PCH memory request exceeds available space");
      return NULL;
    }

  void *addr = mmap ((void *)TRY_EMPTY_VM_SPACE, sz, PROT_READ | PROT_WRITE,
		     MAP_PRIVATE, fd, 0);

  /* If we failed the map, that means there is *no* free space.  */
  if (addr == (void *) MAP_FAILED)
    return NULL;

  /* Unmap the area before returning.  */
  munmap (addr, sz);

  /* If we got the exact area we requested, then that's great.  */
  if (TRY_EMPTY_VM_SPACE && addr == (void *) TRY_EMPTY_VM_SPACE)
    return addr;

  /* Otherwise, we need to try again but put some buffer space first.  */
  size_t buffer_size = 32 * 1024 * 1024;
  void *buffer = mmap (0, buffer_size, PROT_NONE,
		       MAP_PRIVATE | MAP_ANON, -1, 0);
  addr = mmap (0, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (buffer != (void *) MAP_FAILED)
    munmap (buffer, buffer_size);
  if (addr == (void *) MAP_FAILED)
    return NULL;

  munmap (addr, sz);
  return addr;
}

/* Try to mmap the PCH file at ADDR for SZ bytes at OFF offset in the file.
   If we succeed return 1, if we cannot mmap the desired address, then we
   fail with -1.  */

int
darwin_gt_pch_use_address (void *addr, size_t sz, int fd, size_t off)
{
  void *mapped_addr;

  /* We're called with size == 0 if we're not planning to load a PCH
     file at all.  This allows the hook to free any static space that
     we might have allocated at link time.  */
  if (sz == 0)
    return -1;

  /* Try to map the file with MAP_PRIVATE.  */
  mapped_addr = mmap ((char*)addr, sz, PROT_READ | PROT_WRITE,
		      MAP_PRIVATE, fd, off);

  /* Hopefully, we succeed.  */
  if (mapped_addr == (char*)addr)
    return 1;

  if (mapped_addr != (void *) MAP_FAILED)
    munmap (mapped_addr, sz);

  /* Try to make an anonymous private mmap at the desired location.  */
  mapped_addr = mmap (addr, sz, PROT_READ | PROT_WRITE,
	       MAP_PRIVATE | MAP_ANON, -1, 0);

  if (mapped_addr != addr)
    {
      if (mapped_addr != (void *) MAP_FAILED)
	munmap (mapped_addr, sz);
      return -1;
    }

  if (lseek (fd, off, SEEK_SET) == (off_t)-1)
    return -1;

  while (sz)
    {
      ssize_t nbytes;

      nbytes = read (fd, addr, MIN (sz, (size_t)-1 >> 1));
      if (nbytes <= 0)
	return -1;
      addr = (char *) addr + nbytes;
      sz -= nbytes;
    }

  return 1;
}
