#include "Util.h"

#include "mg/common/Assert.h"

namespace mg {
namespace common {

	const bool glob_TrueBool = true;

	std::string
	StringFormat(
		const char *aFormat,
		...)
	{
		va_list va;
		va_start(va, aFormat);
		std::string res = StringVFormat(aFormat, va);
		va_end(va);
		return res;
	}

	std::string
	StringVFormat(
		const char *aFormat,
		va_list aParams)
	{
		va_list va;
		va_copy(va, aParams);
		int size = vsnprintf(nullptr, 0, aFormat, va);
		MG_DEV_ASSERT(size >= 0);
		va_end(va);
		++size;
		char* data = new char[size];
		va_copy(va, aParams);
		int size2 = vsnprintf(data, size, aFormat, va);
		MG_DEV_ASSERT(size2 + 1 == size);
		va_end(va);
		std::string res(data);
		delete[] data;
		return res;
	}

}
}
