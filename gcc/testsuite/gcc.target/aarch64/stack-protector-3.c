/* { dg-do run } */
/* { dg-require-effective-target fstack_protector } */
/* { dg-options "-fstack-protector-all -O2" } */

extern volatile long *stack_chk_guard_ptr;

void __attribute__ ((noipa))
f (void)
{
  volatile int x;
  /* Munging the contents of __stack_chk_guard should trigger a
     stack-smashing failure for this function.  */
  *stack_chk_guard_ptr += 1;
}

#define XSTRING(x) #x
#define ASMNAME(cname) ASMNAME2 (__USER_LABEL_PREFIX__, cname)
#define ASMNAME2(prefix, cname) XSTRING (prefix) cname

/* Mach-O names no section ".data", prefixes user labels with an underscore,
   has no ELF symbol types or sizes, and spells a weak definition
   differently -- a .globl definition in the executable takes precedence over
   the one libSystem exports, which is what this test needs.  */
#ifdef __MACH__
# define DATA_SECTION	"\t.pushsection __DATA,__data\n"
# define WEAK(X)	"\t.globl\t" X "\n"
# define TYPE(X)	""
# define SIZE(X)	""
#else
# define DATA_SECTION	"\t.pushsection .data\n"
# define WEAK(X)	"\t.weak\t" X "\n"
# define TYPE(X)	"\t.type\t" X ", %function\n"
# define SIZE(X)	"\t.size\t" X ", .-" X "\n"
#endif

#define CHECK(REG) "\tcmp\tx0, " #REG "\n\tbeq\t1f\n"

asm (
	DATA_SECTION
"	.align	3\n"
	ASMNAME ("stack_chk_guard_ptr") ":\n"
#if __ILP32__
"	.word	" ASMNAME ("__stack_chk_guard") "\n"
#else
"	.xword	" ASMNAME ("__stack_chk_guard") "\n"
#endif
	WEAK (ASMNAME ("__stack_chk_guard"))
	ASMNAME ("__stack_chk_guard") ":\n"
"	.word	0xdead4321\n"
"	.word	0xbeef8765\n"
"	.text\n"
	TYPE (ASMNAME ("__stack_chk_fail"))
	ASMNAME ("__stack_chk_fail") ":\n"
"	mov	x0, #0\n"
"	b	" ASMNAME ("exit") "\n"
	SIZE (ASMNAME ("__stack_chk_fail"))
"	.popsection"
);

int
main (void)
{
  f ();
  __builtin_abort ();
}
