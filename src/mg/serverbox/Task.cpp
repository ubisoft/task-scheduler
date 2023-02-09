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
		TaskStatus expected = TASK_STATUS_SIGNALED;
		return myStatus.compare_exchange_strong(expected, TASK_STATUS_PENDING);
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
