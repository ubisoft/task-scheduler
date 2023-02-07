#pragma once

#include "mg/common/Types.h"

namespace mg {
namespace common {

	uint32 RandomUInt32();

	uint32 RandomUniformUInt32(
		uint32 aMin,
		uint32 aMax);

	bool RandomBool();
}
}
