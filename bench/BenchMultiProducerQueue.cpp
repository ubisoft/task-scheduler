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
