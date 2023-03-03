#include "Random.h"

#include "mg/common/Thread.h"

#include <random>
#include <ctime>

namespace mg {
namespace common {

	using RandomGenerator = std::mt19937;

	static RandomGenerator*
	RandomGeneratorNew()
	{
		RandomGenerator* myGenerator = new RandomGenerator();
		myGenerator->seed((unsigned)(time(nullptr) % UINT_MAX));
		return myGenerator;
	}

	static RandomGenerator*
	RandomGeneratorGetLocal()
	{
		static MG_THREADLOCAL RandomGenerator* res = nullptr;
		if (res == nullptr)
			res = RandomGeneratorNew();
		return res;
	}

	uint32_t
	Randomuint32_t()
	{
		return RandomUniformuint32_t(0, UINT32_MAX);
	}

	uint32_t
	RandomUniformuint32_t(
		uint32_t aMin,
		uint32_t aMax)
	{
		std::uniform_int_distribution<uint32_t> dist(aMin, aMax);
		return dist(*RandomGeneratorGetLocal());
	}

	bool
	RandomBool()
	{
		std::uniform_int_distribution<uint16_t> dist(0, 1);
		return dist(*RandomGeneratorGetLocal()) == 1;
	}
}
}
