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
