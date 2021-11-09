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

/* Yes, this is really supposed to work.  */
/* This allows for a pagesize of 16384, which we have on Darwin20, but should
   continue to work OK for pagesize 4096 which we have on earlier versions.
   The size is 1 (binary) Gb.  */
//static char pch_address_space[65536*16384] __attribute__((aligned (16384)));

#if defined(__x86_64) && defined(__LP64__)
# define TRY_EMPTY_VM_SPACE	0x1000000000
#elif defined(__x86_64)
# define TRY_EMPTY_VM_SPACE	0x60000000
#elif defined(__i386)
# define TRY_EMPTY_VM_SPACE	0x60000000
#elif defined(__powerpc__) && defined(__LP64__)
# define TRY_EMPTY_VM_SPACE	0x1000000000
#elif defined(__powerpc__)
# define TRY_EMPTY_VM_SPACE	0x60000000
#elif defined(__aarch64__)
# define TRY_EMPTY_VM_SPACE	0x1000000000
#else
# define TRY_EMPTY_VM_SPACE	0
#endif

/* Return the address of the PCH address space, if the PCH will fit in it.  */

void *
darwin_gt_pch_get_address (size_t sz, int fd ATTRIBUTE_UNUSED)
{
#if 1
  size_t buffer_size = 32 * 1024 * 1024;
  void *addr, *buffer;

  addr = mmap ((void *)TRY_EMPTY_VM_SPACE, sz, PROT_READ | PROT_WRITE,
	       MAP_PRIVATE, fd, 0);

  /* If we failed the map, that means there's *no* free space.  */
  if (addr == (void *) MAP_FAILED)
    return NULL;

  /* Unmap the area before returning.  */
  munmap (addr, sz);

  /* If we got the exact area we requested, then that's great.  */
  if (TRY_EMPTY_VM_SPACE && addr == (void *) TRY_EMPTY_VM_SPACE)
    return addr;

  /* Otherwise, we need to try again with buffer space.  */
  buffer = mmap (0, buffer_size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
  addr = mmap (0, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (buffer != (void *) MAP_FAILED)
    munmap (buffer, buffer_size);
  if (addr == (void *) MAP_FAILED)
    return NULL;
  munmap (addr, sz);

  return addr;

#else
  if (sz <= sizeof (pch_address_space)) {
    return pch_address_space;
  } else {
    fprintf(stderr, "BAD for %p, size %d\n", (void *)&pch_address_space[0], sz);
    return NULL;
  }
#endif
}

/* Check ADDR and SZ for validity, and deallocate (using munmap) that part of
   pch_address_space beyond SZ.  */

int
darwin_gt_pch_use_address (void *addr, size_t sz, int fd, size_t off)
{
#if 1
  void *mapped_addr;

  /* We're called with size == 0 if we're not planning to load a PCH
     file at all.  This allows the hook to free any static space that
     we might have allocated at link time.  */
  if (sz == 0)
    return -1;

  /* Try to map the file with MAP_PRIVATE.  */
  mapped_addr = mmap (addr, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, off);

  if (mapped_addr == addr)
    return 1;

  if (mapped_addr != (void *) MAP_FAILED)
    munmap (mapped_addr, sz);

  /* Try to make an anonymous private mmap at the desired location.  */
  mapped_addr = mmap (addr, sz, PROT_READ | PROT_WRITE,
	       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

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

#else
  const size_t pagesize = getpagesize();
  void *mmap_result;
  int ret;
  gcc_checking_assert ((size_t)pch_address_space % pagesize == 0
			&& sizeof (pch_address_space) % pagesize == 0);

  ret = (addr == pch_address_space && sz <= sizeof (pch_address_space));
  if (sz == 0 || !ret)
    {
      /* Return the space we had set aside.  */
      if (munmap (pch_address_space, sizeof (pch_address_space)) != 0)
	fatal_error (input_location,
		 "could not unmap %<pch_address_space%>: %m");
    }

  if (sz == 0)
    return -1;

  /* Round the size to a whole page size.  Normally this is a no-op.  */
  sz = (sz + pagesize - 1) / pagesize * pagesize;

  if (ret)
    {
      /* Return the space we do not need.  */
      if (munmap (pch_address_space + sz, sizeof (pch_address_space) - sz) != 0)
	fatal_error (input_location,
		 "could not unmap %<pch_address_space%>: %m");
    }

  //	if (ret)
    {
      mmap_result = mmap (addr, sz,
			  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED,
			  fd, off);

      /* The file might not be mmap-able.  */
      ret = mmap_result != (void *) MAP_FAILED;

      /* Sanity check for broken MAP_FIXED.  */
      gcc_assert (!ret || mmap_result == addr);
    }

  return ret;
#endif
}
