#include "Time.h"

namespace mg {
namespace common {

	uint64
	GetMilliseconds()
	{
		return ::GetTickCount64();
	}

}
}
