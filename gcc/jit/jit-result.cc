/* Internals of libgccjit: implementation of gcc_jit_result
   Copyright (C) 2013-2026 Free Software Foundation, Inc.
   Contributed by David Malcolm <dmalcolm@redhat.com>.

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

#include "config.h"
#define INCLUDE_DLFCN_H
#include "system.h"
#include "coretypes.h"
#include "target.h"

#include "jit-common.h"
#include "jit-logging.h"
#include "jit-result.h"
#include "jit-tempdir.h"

#ifdef _WIN32
#include "jit-w32.h"
#endif

namespace gcc {
namespace jit {

/* Constructor for gcc::jit::result.  */

result::
result(logger *logger, handle dso_handle, tempdir *tempdir_) :
  log_user (logger),
  m_dso_handle (dso_handle),
  m_tempdir (tempdir_)
{
  JIT_LOG_SCOPE (get_logger ());
}

/* gcc::jit::result's destructor.

   Called implicitly by gcc_jit_result_release.  */

result::~result()
{
  JIT_LOG_SCOPE (get_logger ());

#ifdef _WIN32
  FreeLibrary(m_dso_handle);
#else
  dlclose (m_dso_handle);
#endif
  /* Responsibility for cleaning up the tempdir (including "fake.so" within
     the filesystem) might have been handed to us by the playback::context,
     so that the cleanup can be delayed (see PR jit/64206).

     If so, clean it up now.  */
  delete m_tempdir;
}

/* Attempt to locate the given function by name within the
   playback::result, using dlsym.

   Implements the post-error-checking part of
   gcc_jit_result_get_code.  */

void *
result::
get_code (const char *funcname)
{
  JIT_LOG_SCOPE (get_logger ());

  void *code;

#ifdef _WIN32
  /* Clear any existing error.  */
  SetLastError(0);

  code = (void *)GetProcAddress(m_dso_handle, funcname);
  if (GetLastError() != 0)  {
    print_last_error ();
  }
#else
  const char *error;
  /* Clear any existing error.  */
  dlerror ();

  code = dlsym (m_dso_handle, funcname);

  if ((error = dlerror()) != NULL)  {
    fprintf(stderr, "%s\n", error);
  }
#endif

  return code;
}

#ifndef _WIN32
/* Emulated TLS has no exported symbol for the variable itself.  What is
   exported is the control object, so we need to first look to see if there
   is a control object for the named var; if there is we can then translate
   the address.  Control object name composition copied from tree-emutls.cc.  */

#if !defined (NO_DOT_IN_LABEL)
# define EMUTLS_SEPARATOR	"."
#elif !defined (NO_DOLLAR_IN_LABEL)
# define EMUTLS_SEPARATOR	"$"
#else
# define EMUTLS_SEPARATOR	"_"
#endif

static void *
get_emulated_tls_global (void *dso_handle, const char *name)
{
  const char *prefix = (targetm.emutls.var_prefix
			? targetm.emutls.var_prefix
			: "__emutls_v" EMUTLS_SEPARATOR);
  char *control_name = concat (prefix, name, NULL);
  /* Look this up and exit early if it is not available.  */
  void *control = dlsym (dso_handle, control_name);
  free (control_name);
  if (!control)
    return NULL;

  /* We have to call get_address function to translate the emulated TLS
     control object address to a pointer the actual var.  */
  const char *fn_name;
  if (targetm.emutls.get_address)
    {
      fn_name = targetm.emutls.get_address;
      if (strncmp (fn_name, "__builtin_", 10) == 0)
	fn_name += 10;
    }
  else
    fn_name = "__emutls_get_address";

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconditionally-supported"
  /* We need the function pointer here, but we need to look up the version
     that the dynamic linker resolved - which is most likely not in the same
     DSO as the variable we are looking up.  */
  using fn_t = void * (*) (void*);
  auto get_address_fn
    = reinterpret_cast<fn_t> (dlsym (RTLD_DEFAULT, fn_name));
#pragma GCC diagnostic pop
  if (!get_address_fn)
    return NULL;

  return get_address_fn (control);
}
#endif /* !_WIN32 */

/* Attempt to locate the given global by name within the
   playback::result, using dlsym.

   Implements the post-error-checking part of
   gcc_jit_result_get_global.  */

void *
result::
get_global (const char *name)
{
  JIT_LOG_SCOPE (get_logger ());

  void *global;

#ifdef _WIN32
  /* Clear any existing error.  */
  SetLastError(0);

  global = (void *)GetProcAddress(m_dso_handle, name);
  if (GetLastError() != 0)  {
    print_last_error ();
  }
#else
  const char *error;
  /* Clear any existing error.  */
  dlerror ();

  global = dlsym (m_dso_handle, name);

  if (!global && !targetm.have_tls)
    {
      /* Under emulated TLS there is no symbol for the variable itself, so the
	 global lookup above fails. The only way to tell if there is a TLS var
	 with this name is by trying to look it up via its control object.  */
      void *tls_global = get_emulated_tls_global (m_dso_handle, name);
      if (tls_global)
	{
	  /* Discard the "symbol not found" error from the failed lookup.  */
	  dlerror ();
	  return tls_global;
	}
    }

  if ((error = dlerror()) != NULL)  {
    fprintf(stderr, "%s\n", error);
  }
#endif

  return global;
}

} // namespace gcc::jit

} // namespace gcc
