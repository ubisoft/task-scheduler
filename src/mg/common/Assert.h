#pragma once

#include "mg/common/Util.h"

#define MG_COMMON_ASSERT_F(X, ...) do {														\
	if (MG_TEST(!(X)))																		\
		mg::common::AssertF(#X, __FILE__, __LINE__, __VA_ARGS__);							\
} while(false)

#define MG_COMMON_ASSERT(X) do {															\
	if (MG_TEST(!(X)))																		\
		mg::common::AssertS(#X, __FILE__, __LINE__);										\
} while(false)

#if IS_BUILD_DEBUG
#define MG_DEV_ASSERT MG_COMMON_ASSERT
#define MG_DEV_ASSERT_F MG_COMMON_ASSERT_F
#else
#define MG_DEV_ASSERT MG_UNUSED
#define MG_DEV_ASSERT_F MG_UNUSED
#endif

namespace mg {
namespace common {

	void AssertS(
		const char* aExpression,
		const char* aFile,
		int aLine);

	MG_STRFORMAT_PRINTF(4, 5)
	void AssertF(
		const char* aExpression,
		const char* aFile,
		int aLine,
		const char* aFormat,
		...);

}
}
