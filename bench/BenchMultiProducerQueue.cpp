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

#include "Bench.h"

#include "mg/common/MultiProducerQueueIntrusive.h"

namespace mg {
namespace bench {

	struct BenchValue
	{
		BenchValue();

		BenchValue* myNext;
	};

	using MultiProducerQueue = mg::common::MultiProducerQueueIntrusive<BenchValue>;

	inline
	BenchValue::BenchValue()
		: myNext(nullptr)
	{
	}

}
}

#include "BenchMultiProducerQueueTemplate.hpp"
