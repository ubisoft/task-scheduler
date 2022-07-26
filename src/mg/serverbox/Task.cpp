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

#include "Task.h"

#include "mg/common/Time.h"

namespace mg {
namespace serverbox {

	void
	Task::SetDelay(
		uint32 aDelay)
	{
		PrivTouch();
		myDeadline = mg::common::GetMilliseconds() + aDelay;
	}

	void
	Task::AdjustDelay(
		uint32 aDelay)
	{
		AdjustDeadline(mg::common::GetMilliseconds() + aDelay);
	}

	void
	Task::SetDeadline(
		uint64 aDeadline)
	{
		PrivTouch();
		myDeadline = aDeadline;
	}

	void
	Task::AdjustDeadline(
		uint64 aDeadline)
	{
		PrivTouch();
		if (aDeadline < myDeadline)
			myDeadline = aDeadline;
	}

	bool
	Task::IsExpired() const
	{
		PrivTouch();
		return myIsExpired;
	}

	uint64
	Task::GetDeadline() const
	{
		PrivTouch();
		return myDeadline;
	}

	bool
	Task::ReceiveSignal()
	{
		PrivTouch();
		return mg::common::AtomicCompareExchange(&myStatus, TASK_STATUS_PENDING,
			TASK_STATUS_SIGNALED) == TASK_STATUS_SIGNALED;
	}

	void
	Task::PrivExecute()
	{
		PrivTouch();
		// Deadline is reset so as the task wouldn't use it again
		// if re-posted. Next post should specify a new deadline
		// or omit it. In that way when Post, you can always be
		// sure the old task deadline won't affect the next
		// execution.
		myDeadline = 0;
		myCallback(this);
	}

	void
	Task::PrivCreate()
	{
		myNext = nullptr;
		myIndex = -1;
		myStatus = TASK_STATUS_PENDING;
		myDeadline = 0;
		myIsInQueues = false;
		myIsExpired = false;
	}

	void
	Task::PrivTouch() const
	{
		MG_COMMON_ASSERT_F(!myIsInQueues,
			"An attempt to modify a task while it is in scheduler queues");
	}

}
}
