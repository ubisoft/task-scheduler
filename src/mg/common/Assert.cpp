// Copyright (C) 2022 Ubisoft Entertainment
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <https://www.gnu.org/licenses/>.

#include "Assert.h"

#include "mg/common/String.h"

#include <cstdio>

namespace mg {
namespace common {

	void
	AssertS(
		const char* aExpression,
		const char* aFile,
		int aLine)
	{
		fprintf(stderr, "assertion failed: (%s) %s %u\n", aExpression, aFile, aLine);
#if IS_PLATFORM_POSIX
		abort();
#else
		int* t = NULL;
		*t = 0;
#endif
	}

	void
	AssertF(
		const char* aExpression,
		const char* aFile,
		int aLine,
		const char* aFormat,
		...)
	{
		mg::common::HybridString<1024> expression;
		expression.Format("(%s): ", aExpression);

		va_list	ap;
		va_start(ap, aFormat);
		expression.AppendVFormat(aFormat, ap);
		va_end(ap);
		AssertS(expression, aFile, aLine);
	}

}
}
