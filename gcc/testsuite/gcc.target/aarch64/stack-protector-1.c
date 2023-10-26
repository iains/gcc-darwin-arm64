/* { dg-do run } */
/* { dg-require-effective-target fstack_protector } */
/* { dg-options "-fstack-protector-all -O2" } */

extern volatile long *stack_chk_guard_ptr;

volatile long *
get_ptr (void)
{
  return stack_chk_guard_ptr;
}

void __attribute__ ((noipa))
f (void)
{
  volatile int x;
  x = 1;
  x += 1;
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
"	.globl	" ASMNAME ("stack_chk_guard_ptr") "\n"
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
"	.globl	" ASMNAME ("main") "\n"
	TYPE (ASMNAME ("main"))
	ASMNAME ("main") ":\n"
"	bl	" ASMNAME ("get_ptr") "\n"
"	str	x0, [sp, #-16]!\n"
"	bl	" ASMNAME ("f") "\n"
"	str	x0, [sp, #8]\n"
"	ldr	x0, [sp]\n"
#if __ILP32__
"	ldr     w0, [x0]\n"
#else
"	ldr     x0, [x0]\n"
#endif
	CHECK (x1)
	CHECK (x2)
	CHECK (x3)
	CHECK (x4)
	CHECK (x5)
	CHECK (x6)
	CHECK (x7)
	CHECK (x8)
	CHECK (x9)
	CHECK (x10)
	CHECK (x11)
	CHECK (x12)
	CHECK (x13)
	CHECK (x14)
	CHECK (x15)
	CHECK (x16)
	CHECK (x17)
	CHECK (x18)
	CHECK (x19)
	CHECK (x20)
	CHECK (x21)
	CHECK (x22)
	CHECK (x23)
	CHECK (x24)
	CHECK (x25)
	CHECK (x26)
	CHECK (x27)
	CHECK (x28)
	CHECK (x29)
	CHECK (x30)
"	ldr	x1, [sp]\n"
	CHECK (x1)
"	mov	x0, #0\n"
"	b	" ASMNAME ("exit") "\n"
"1:\n"
"	b	" ASMNAME ("abort") "\n"
	SIZE (ASMNAME ("main"))
"	.popsection"
);
