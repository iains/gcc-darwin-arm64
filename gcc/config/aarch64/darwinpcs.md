# The Darwin ABI (darwinpcs) for AArch64 (Arm64) Mach-O

## Introduction.

This describes the Darwin PCS (darwinpcs) as implemented in GCC.

Base information is taken from:

[1] [ARM IHI 0055B : AAPCS64 (current through AArch64 ABI release 1.0, dated 22nd May 2013)](http://infocenter.arm.com/help/topic/com.arm.doc.ihi0055b/IHI0055B_aapcs64.pdf)

[2] [Apple iOS document : darwinpcs](https://developer.apple.com/library/archive/documentation/Xcode/Conceptual/iPhoneOSABIReference/Articles/ARM64FunctionCallingConventions.html#//apple_ref/doc/uid/TP40013702-SW1)

[3] [The LLVM backend for Mach-O/arm64 from GIT (LLVM-12 was used initially).](https://github.com/llvm/llvm-project.git)

## Terminology

**Darwin** is the kernel used for macOS (10/OSX and 11) and iOS (all versions,
so far).

**Mach-O** is the file format used for object files and DSOs, including executables
on Darwin platforms (to some extent, the two names are interchangeable in
describing rules applicable to a back end).

The `AArch64` port for Darwin is known as `arm64` (`arm64` is synonymous with
`aarch64` for Darwin in GCC).   There is an ILP32 variant, `arm64_32` (not yet
considered in detail or handled by these branches).

There are two main technical sections

* Part 1 which describes the darwinpcs deviations from AAPCS64.

   This is a primarily cross-reference between [2] and [1].

* Part 2 contains additional pertinent information.

   Some is recorded in [2] but most is determined from generic Darwin/Mach-O
   rules and the implementation [3].

* Part 3 describes the deviations from darwinpcs present in GCC.

   GCC implements some features not available ib the host tools which are
   based on LLVM/clang.

   GCC does not yet (as of GCC-12.1) support all parts of the darwinpcs as
   written here.

**Note** Since a macOS or iOS system is built with a toolchain based on [3],
that sets the de facto ABI.  Therefore, the ABI as implemented by the XCode
version appropriate to a given OS release shall take precedence over version(s)
described in the referenced documents in the event of discrepancy.

## PART 1 - AAPCS64 and darwinpcs.

### Outline

Darwin PCS Differences from AAPCS64.

The intent of these notes are to match the differences described in [2] against
the section numbers and rule designations of [1] since the AArch64 port code
uses the rule designations in comments.

The organisation of these notes is by section heading of [1].

[2] Refers to the darwinpcs as "iOS" which was the first Darwin OS variant
implementing it, however it is stated (albeit unofficially?) that Arm64 macOS
will adopt the same ABI and is expected to be able to execute iOS executables.

So, for the present, 'iOS' is considered to be equivalent to 'macOS'
(generically 'Darwin').

In the text from [2] the expression "generic procedure call standard" refers
to the AAPCS64 [1].

Darwin PCS rules are designated `D.N` below.

### AAPCS64 Section 1.

No amendments.

### AAPCS64 Section 2.

No amendments.

### AAPCS64 Section 3.

No amendments.

The darwinpcs is non-conforming with the aapcs64 in the areas described below.

### AAPCS64 Section 4.

No amendments

but note:
* Darwin's `char` and `wchar_t` are both signed.
* Where applicable, the `__fp16 type` is `IEEE754-2008` format.

### AAPCS64 Section 5.

5.1 Machine Registers
5.1.1 General-purpose Registers

Darwin reserves `x18` as the platform register (as permitted).

5.2 Processes, Memory and the Stack
5.2.3 The Frame Pointer

From [2] : The frame pointer register (x29) must always address a valid frame
record, although some functions—such as leaf functions or tail calls—may elect
not to create an entry in this list.

This corresponds to the first bullet and is conforming.  It implies that Darwin
should warn if the user tries to use an option that omits the FP.

5.4 Parameter Passing
5.4.2 Parameter Passing Rules

`D.1` From [2] : Empty struct types are ignored for parameter-passing purposes.
This behavior applies to the GNU extension in C and, where permitted by the
language, in C++.

It is noted that this might not correspond to any specific rule - but,
presumably, needs to be applied in marshalling arguments.

`D.2` From [2] : In the generic procedure call standard, all function arguments
passed on the stack consume slots in multiples of 8 bytes. In iOS, this
requirement is dropped, and values consume only the space required.   Padding
is still inserted on the stack to satisfy arguments’ alignment requirements.

`D.3` From [2] : The general ABI specifies that it is the callee’s responsibility
to sign or zero-extend arguments having fewer than 32 bits, and that unused bits
in a register are unspecified. In iOS, however, the caller must perform such
extensions, up to 32 bits.  This apparently conflicts with the `D.2` above and
thus can only be applied to values passed in registers?

(notwithstanding C rules for widening).

`D.4` From [2] : The generic procedure call standard requires that arguments
with 16-byte alignment passed in integer registers begin at an even-numbered
xN, skipping a previous odd-numbered xN if necessary. The iOS ABI drops this
requirement.

#### Variadic Functions

From [2]:
The iOS ABI for functions that take a variable number of arguments is entirely
different from the generic version.

Stages A and B of the generic procedure call standard are performed as usual.
in particular, even variadic aggregates larger than 16 bytes are passed via a
reference to temporary memory allocated by the caller. After that, the fixed
arguments are allocated to registers and stack slots as usual in iOS.

The NSRN(*sic*) (?NSAA was intended?) is then rounded up to the next multiple
of 8 bytes, and each variadic argument is assigned to the appropriate number
of 8-byte stack slots.

The C language requires arguments smaller than int to be promoted before a call,
but beyond that, unused bytes on the stack are not specified by the darwinpcs.

(see section 7) As a result of this change, the type va_list is an alias for
char * rather than for the struct type specified in the generic PCS.
It is also not in the std namespace when compiling C++ code.

#### Stage A

No Changes.

#### Stage B

No changes.

#### Stage C

Insert C.6.5 `D.3`
If the argument is an Integral or Pointer Type, the size of the argument is
less than 4 bytes and the NGRN is less than 8, the argument is sign or zero-
extended as appropriate to 4 bytes.

C.8 Delete rule per `D.4`

C.12
`D.2` Amend to:
The NSAA is rounded up to Natural Alignment of the argument’s type.

C.14 Delete rule per `D.2`

From the observations section:
"Both before and after the layout of each argument, then NSAA will have a
 minimum alignment of 8."  This no longer applies.

### AAPCS64 Section 6.

No changes (noting that the `__fp16` type is `IEEE754-2008` format.)

### AAPCS64 Section 7.

7.1 Data Types
7.1.1 Arithmetic Types

Table 3 is amended thus.

| C/C++ Type | Machine Type | Notes |
| --- | --- | --- |
| char | signed byte | compatible with other Darwin variants  |
| wchar_t | int | ditto |

**The size of long double (and by implication the _Imaginary and _Complex C99
variants) is set to be the same as double.**

We need to consider **_very_** carefully how to handle this.  The current
(64bit) long double is mangled as 'e' by the clang toolchain.


7.1.4 Additional Types

Table 5 is amended to reflect the different variadic function rules.

`D.6` The type `va_list` is an alias for `char *`
`D.7` The `va_list` type is _not_ presented in `std::` for C++.

7.2 Argument Passing Conventions

Possibly, might require adjustment for `D.3`?

## PART 2 - Other platform information

### Additional comments from [2]

#### Red Zone

The ARM64 iOS red zone consists of the 128 bytes immediately below the stack
pointer sp. As with the x86-64 ABI, the operating system has committed not to
modify these bytes during exceptions. User-mode programs can rely on them not
to change unexpectedly, and can potentially make use of the space for local
variables.

In some circumstances, this approach can save an sp-update instruction on
function entry and exit.

At present, it seems, that there's no port using a red zone for AArch64, and
there's no implementation - so this optimisation opportunity will be unused
at least initially.  TODO.

#### Divergences from the Generic C++ ABI

The generic ARM64 C++ ABI is specified in C++ Application Binary Interface
Standard for the ARM 64-bit architecture, which is in turn based on the
Itanium C++ ABI used by many UNIX-like systems.

Some sections are ELF-specific and not applicable to the underlying object
format used by iOS. There are, however, some significant differences from
these specifications in iOS.

##### Name Mangling

When compiling C++ code, types get incorporated into the names of functions
in a process referred to as “mangling.” The iOS ABI differs from the generic
specification in the following small ways.

Because `va_list` is an alias for `char *`, it is mangled in the same way—as
`Pc` instead of `St9__va_list` (Section 7).

NEON vector types are mangled in the same way as their 32-bit ARM counterparts,
rather than using the 64-bit scheme. For example, iOS uses `17__simd128_int32_t`
instead of the generic `11__Int32x4_t`.

##### Other Itanium Divergences

In the generic ABI, empty structs are treated as aggregates with a single byte
member for parameter passing. In iOS, however, they are ignored unless they
have a nontrivial destructor or copy-constructor. If they do have such
functions, they are considered as aggregates with one byte member in the
generic manner.

As with the ARM 32-bit C++ ABI, iOS requires the complete-object (C1) and base-
object (C2) constructors to return this to their callers. Similarly, the
complete object (D1) and base object (D2) destructors return this. This
requirement is not made by the generic ARM64 C++ ABI.

In the generic C++ ABI, array cookies change their size and alignment according
to the type being allocated. As with the 32-bit ARM, iOS provides a fixed
layout of two size_t words, with no extra alignment requirements.

In iOS, object initialization guards are nominally `uint64_t` rather than
`int64_t`.
This affects the prototypes of the functions `__cxa_guard_acquire`,
`__cxa_guard_release` and `__cxa_guard_abort`.

In the generic ARM64 ABI, function pointers whose type differ only in being
extern "C" or extern "C++" are interchangeable. This is not the case in iOS.

### Undocumented items

* The platform ABI contains provisions for the swift language, but since GCC
has no swift FE there's no need to implement them (it might be wise to ensure
that any reserved registers are handled appropriately tho)

* Darwin user-space code is PIC (2) = fPIC (so nominally 'large' but the code
model is not modified by the PIC setting [I think FIXME: check]).

FIXME: ??? I'm not clear about kernel mode at present.

The following symbol kinds always have a GOT indirection for Mach-O-pic.

* undefined external
* weak [not hidden]
* common

FIXME: check other rules for GOT indirections.

### Darwin code models

* TINY is _not_ supported
* SMALL supported (DEFAULT)
* LARGE supported

   AFAICT, Darwin's large model is PIC (with perhaps a very limited number of
   modes)
   However large+PIC is stated to be unimplemented in the current aarch64
   backend so that's a TODO.

FIXME: ??? I'm not clear about kernel mode at present.

### Darwin arm64 TLS

Darwin has a single TLS model (not attempting to implement in the short-
term).  It's closest to ELF xxxxxxx FIXME: which one?

### Generic Darwin/Mach-O Comments for people familiar with ELF.

Darwin
* does _not_ support strong symbol aliases
* does support weak symbol aliases
* supports visibility - default and hidden.
* Has a "for linker only" symbol visibility.

   Such symbols are visible to the static linker (`ld64`), but not externally.
  These are used to support the Mach-O "subsections_by_symbol" linker mode
  (default for > 10years).  Any symbol that is not 'global' and does not begin
  with 'L' (the local symbol designation) is counted as 'linker visible'.
 * does _not_ support 'static' code in the user space

   Everything needs to be invoked using the dynamic linker (`dyld`).  There is
  neither crt0.o nor a static edition of libc.

FIXME: CHECK the kernel model here.

### Darwin Relocations and Assembler syntax

* `Mach-O` for `Arm64` uses a reduced set of relocations c.f. the ELF set.

   There are only 11 entries but the relocation format allows for multiple sizes
(1, 2, 4, 8) where that's appropriate, and for ancillary data (e.g. a scale),
so the actual number of permutations is larger.

* Generally, Darwin supports relocations of the form A - B + signed const

   A must be known (i.e. defined in the current TU).

* `Mach-O` for `Arm64` has postfix assembler syntax.

   Where there's an assembly language representation for the relocation type
   it appears after the name (e.g. `foo@PAGE` in contrast to the ELF
   `:got:foo`).

#### Relocs list

For pointers (no source representation).

`ARM64_RELOC_UNSIGNED = 0`

Must be followed by an `ARM64_RELOC_UNSIGNED`

`ARM64_RELOC_SUBTRACTOR = 1`

A B/BL instruction with 26-bit displacement.
(no source representation)

`ARM64_RELOC_BRANCH26 = 2`

PC-rel distance to page of target [adrp].

`foo@PAGE`

`ARM64_RELOC_PAGE21 = 3`

Offset within page, scaled by r_length [add imm, ld/st].

`foo@PAGEOFF`

`ARM64_RELOC_PAGEOFF12 = 4`

PC-rel distance to page of GOT slot [adrp].

`foo@GOTPAGE`
`ARM64_RELOC_GOT_LOAD_PAGE21 = 5`

Offset within page of GOT slot, scaled by r_length [add imm, ld/st].

`foo@GOTPAGEOFF`

`ARM64_RELOC_GOT_LOAD_PAGEOFF12 = 6`


For pointers to GOT slots.
(4 and 8 byte versions)

`foo@GOT`

`ARM64_RELOC_POINTER_TO_GOT = 7`


PC-rel distance to page of TLVP slot [adrp].

`foo@TVLPPAGE`

`ARM64_RELOC_TLVP_LOAD_PAGE21 = 8`

Offset within page of TLVP slot, scaled by r_length [add imm, ld/st].

`foo@TVLPPAGEOFF`

`ARM64_RELOC_TLVP_LOAD_PAGEOFF12 = 9`

Must be followed by `ARM64_RELOC_PAGE21` or `ARM64_RELOC_PAGEOFF12`.
(no source representation)

The addend is a signed 24bit quantity (+/- 8M range).

`ARM64_RELOC_ADDEND = 10`

## PART 2 - GCC-12 deviations from the PCS and supporting information.

### D.3 is not yet supported (github issue #74)

  GCC promotes in the callee not the caller.

### Support for nested functions

  GCC provides nested functions which are used overtly from C but also to
  implement some parts of Ada and Fortran.

  This requires assigning a register to act as the STATIC CHAIN.
  For GCC-12 this is X16

  Support for nested function trampolines is provided by a heap-based table.

### Support for __float128

  The darwinpcs has no provision for a 128bit float type.
  GCC-12 supports IEEE741 128bit float values by sof-float.
  The ABI used for __float128 matches that for AAPCS64

## End.
