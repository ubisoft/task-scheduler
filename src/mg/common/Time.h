#pragma once

#include "mg/common/Types.h"

namespace mg {
namespace common {

	// Returns the number of milliseconds since some point in the
	// past. Monotonic, won't go back on clock changes.
	uint64_t GetMilliseconds();

}
}
