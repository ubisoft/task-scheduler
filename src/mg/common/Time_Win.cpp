#include "Time.h"

namespace mg {
namespace common {

	uint64_t
	GetMilliseconds()
	{
		return ::GetTickCount64();
	}

}
}
