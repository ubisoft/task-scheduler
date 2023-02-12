#include "UnitTest.h"

#include "mg/common/Assert.h"
#include "mg/common/Util.h"

namespace mg {
namespace unittests {

	static void
	UnitTestUtilFormat()
	{
		MG_COMMON_ASSERT(mg::common::StringFormat("") == "");
		MG_COMMON_ASSERT(mg::common::StringFormat("abc") == "abc");
		MG_COMMON_ASSERT(mg::common::StringFormat(
			"a %d %s b c %u", 1, "str", 2U) == "a 1 str b c 2");
	}

	void
	UnitTestUtil()
	{
		TestSuiteGuard suite("Util");

		UnitTestUtilFormat();
	}

}
}
