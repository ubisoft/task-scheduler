#include "Bench.h"

#include "mg/serverbox/TaskScheduler.h"

namespace mg {
namespace bench {

	using Task = mg::serverbox::Task;
	using TaskScheduler = mg::serverbox::TaskScheduler;
	using TaskSchedulerThread = mg::serverbox::TaskSchedulerThread;

}
}

#include "BenchTaskSchedulerTemplate.hpp"
