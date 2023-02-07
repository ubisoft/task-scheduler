#pragma once

#include "mg/common/Types.h"

namespace mg {
namespace common {

	class QPTimer
	{
	public:
		QPTimer();

		void Start();

		double GetMilliSeconds();

	private:
#if IS_PLATFORM_POSIX
		uint64 myStartNs;
#else
		uint64 myStartQuant;
#endif
	};

}
}
