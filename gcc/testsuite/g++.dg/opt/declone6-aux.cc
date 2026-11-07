// The DESIGNATED EMITTER for declone6.C.  It includes the same header, so it
// sees the `extern template' declaration, and then overrides it with an
// explicit instantiation DEFINITION -- which [temp.explicit] permits provided
// the definition follows the declaration, as it does here.  This is the only
// standard-sanctioned way to nominate one TU out of many to emit the entity,
// and [temp.explicit] requires one: an entity used under an explicit
// instantiation declaration must be the subject of an explicit instantiation
// definition somewhere in the program.
#include "declone6.h"

template struct B<int>;			// the override: emit them HERE
