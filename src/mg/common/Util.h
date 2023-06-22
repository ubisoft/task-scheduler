#pragma once

#include "mg/common/Definitions.h"

#include <cstdarg>
#include <string>

namespace mg {
namespace common {

	MG_STRFORMAT_PRINTF(1, 2)
	std::string StringFormat(
		const char *aFormat,
		...);

	MG_STRFORMAT_PRINTF(1, 0)
	std::string StringVFormat(
		const char *aFormat,
		va_list aParams);

}
}
