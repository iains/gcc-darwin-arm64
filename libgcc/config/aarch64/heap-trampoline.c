#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *allocate_trampoline_page (void);
int get_trampolines_per_page (void);
struct tramp_ctrl_data *allocate_tramp_ctrl (struct tramp_ctrl_data *parent);
void *allocate_trampoline_page (void);

void __builtin_nested_func_ptr_created (void *chain, void *func, void **dst);
void __builtin_nested_func_ptr_deleted (void);

struct tramp_ctrl_data;
struct tramp_ctrl_data
{
  struct tramp_ctrl_data *prev;

  int free_trampolines;

  /* This will be pointing to an executable mmap'ed page.  */
  struct aarch64_trampoline *trampolines;
};

struct aarch64_trampoline {
  uint32_t insns[6];
  void *func_ptr;
  void *chain_ptr;
};

int
get_trampolines_per_page (void)
{
  return getpagesize() / sizeof(struct aarch64_trampoline);
}

static _Thread_local struct tramp_ctrl_data *tramp_ctrl_curr = NULL;

void *
allocate_trampoline_page (void)
{
  void *page;

  page = mmap (0, getpagesize (), PROT_WRITE | PROT_EXEC,
	       MAP_ANON | MAP_PRIVATE, 0, 0);

  return page;
}

struct tramp_ctrl_data *
allocate_tramp_ctrl (struct tramp_ctrl_data *parent)
{
  struct tramp_ctrl_data *p = malloc (sizeof (struct tramp_ctrl_data));
  if (p == NULL)
    return NULL;

  p->trampolines = allocate_trampoline_page ();

  if (p->trampolines == MAP_FAILED)
    return NULL;

  p->prev = parent;
  p->free_trampolines = get_trampolines_per_page();

  return p;
}

static const uint32_t aarch64_trampoline_insns[] = {
  0xd503245f, /* hint    34 */
  0x580000b1, /* ldr     x17, .+20 */
  0x580000d2, /* ldr     x18, .+24 */
  0xd61f0220, /* br      x17 */
  0xd5033f9f, /* dsb     sy */
  0xd5033fdf /* isb */
};

void
__builtin_nested_func_ptr_created (void *chain, void *func, void **dst)
{
  if (tramp_ctrl_curr == NULL)
    {
      tramp_ctrl_curr = allocate_tramp_ctrl (NULL);
      if (tramp_ctrl_curr == NULL)
	abort ();
    }

  if (tramp_ctrl_curr->free_trampolines == 0)
    {
      void *tramp_ctrl = allocate_tramp_ctrl (tramp_ctrl_curr);
      if (!tramp_ctrl)
	abort ();

      tramp_ctrl_curr = tramp_ctrl;
    }

  struct aarch64_trampoline *trampoline
    = &tramp_ctrl_curr->trampolines[get_trampolines_per_page ()
				    - tramp_ctrl_curr->free_trampolines];

  memcpy (trampoline->insns, aarch64_trampoline_insns,
	  sizeof(aarch64_trampoline_insns));
  trampoline->func_ptr = func;
  trampoline->chain_ptr = chain;

  tramp_ctrl_curr->free_trampolines -= 1;

  __builtin___clear_cache ((void *)trampoline->insns,
			   ((void *)trampoline->insns + sizeof(trampoline->insns)));

  *dst = &trampoline->insns;
}

void
__builtin_nested_func_ptr_deleted (void)
{
  if (tramp_ctrl_curr == NULL)
    abort ();

  tramp_ctrl_curr->free_trampolines += 1;

  if (tramp_ctrl_curr->free_trampolines == get_trampolines_per_page ())
    {
      if (tramp_ctrl_curr->prev == NULL)
	return;

      munmap (tramp_ctrl_curr->trampolines, getpagesize());
      struct tramp_ctrl_data *prev = tramp_ctrl_curr->prev;
      free (tramp_ctrl_curr);
      tramp_ctrl_curr = prev;
    }
}
