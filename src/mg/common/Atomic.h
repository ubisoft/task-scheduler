#pragma once

#include "mg/common/Definitions.h"

#if IS_PLATFORM_POSIX
#include "Atomic_Posix.h"
#else
#include "Atomic_Win.h"
#endif
