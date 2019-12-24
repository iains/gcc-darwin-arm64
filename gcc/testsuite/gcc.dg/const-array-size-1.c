/* Positive test for the const-variable-array-size extension, extracted from
   the released macOS 10.13 SDK, which is the shape the extension exists for:

     Security.framework/Versions/A/Headers/Authorization.h

       static const size_t kAuthorizationExternalFormLength = 32;
       typedef struct {
	       char bytes[kAuthorizationExternalFormLength];
       } AuthorizationExternalForm;

   C++ accepts a const object with a constant initializer as a constant
   expression and C does not, so without the extension this is a hard error
   rather than a warning: a struct member cannot be variably modified.  Later
   SDKs changed the declaration to a macro, but the released ones cannot, so
   the extension is still needed to consume them.  */

/* { dg-do compile } */
/* { dg-options "-fallow-const-var-array-size -Wvla" } */

static const __SIZE_TYPE__ form_length = 32;

typedef struct {
  char bytes[form_length]; /* { dg-bogus "variably modified|variable length array" } */
} external_form;

/* The bound is substituted, so the type has a constant size and sizeof is a
   constant expression -- which is what makes this a test rather than merely a
   compilation.  */
_Static_assert (sizeof (external_form) == 32, "not a constant size");

external_form form;
