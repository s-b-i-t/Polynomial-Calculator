/*
 CHANGELOG (Codex robustness pass)
 - `Rational<T>` is now header-only (see `Rational.h`) to avoid template/linker traps.
 - This translation unit is kept to preserve existing build scripts/filenames.
 */

#include "Rational.h"
