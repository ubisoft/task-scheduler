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

	uint32
	RandomUInt32()
	{
		return RandomUniformUInt32(0, UINT32_MAX);
	}

	uint32
	RandomUniformUInt32(
		uint32 aMin,
		uint32 aMax)
	{
		std::uniform_int_distribution<uint32> dist(aMin, aMax);
		return dist(*RandomGeneratorGetLocal());
	}

	bool
	RandomBool()
	{
		std::uniform_int_distribution<int16> dist(0, 1);
		return dist(*RandomGeneratorGetLocal()) == 1;
	}
}
}
