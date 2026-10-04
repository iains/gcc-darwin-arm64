// Emitter for declone6.C.  It includes the same header, so it sees
// the `extern template' declaration, and then overrides it with an explicit
// instantiation def -- which [temp.explicit] permits provided the
// definition follows the declaration, as it does here.

#include "declone6.h"

template struct B<int>;			// the override: emit them HERE
