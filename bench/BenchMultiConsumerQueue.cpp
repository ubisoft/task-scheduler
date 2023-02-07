#include "Bench.h"

#include "mg/common/MultiConsumerQueue.h"

namespace mg {
namespace bench {

	struct BenchValue
	{
		BenchValue()
			: myValue(nullptr)
		{
		}

		void* myValue;
	};

	using BenchQueue = mg::common::MultiConsumerQueue<BenchValue>;
	using BenchQueueConsumer = mg::common::MultiConsumerQueueConsumer<BenchValue>;

}
}

#include "BenchMultiConsumerQueueTemplate.hpp"
