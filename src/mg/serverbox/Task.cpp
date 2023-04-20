#include "Task.h"

#include "mg/common/Assert.h"
#include "mg/common/Time.h"

namespace mg {
namespace serverbox {

	void
	Task::SetDelay(
		uint32_t aDelay)
	{
		PrivTouch();
		myDeadline = mg::common::GetMilliseconds() + aDelay;
	}

	void
	Task::AdjustDelay(
		uint32_t aDelay)
	{
		AdjustDeadline(mg::common::GetMilliseconds() + aDelay);
	}

	void
	Task::SetDeadline(
		uint64_t aDeadline)
	{
		PrivTouch();
		myDeadline = aDeadline;
	}

	void
	Task::AdjustDeadline(
		uint64_t aDeadline)
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

	uint64_t
	Task::GetDeadline() const
	{
		PrivTouch();
		return myDeadline;
	}

	bool
	Task::ReceiveSignal()
	{
		PrivTouch();
		TaskStatus oldStatus = TASK_STATUS_SIGNALED;
		// Acquire-barrier to sync with the release-barrier on the
		// signal sending. Can't be relaxed, because the task might send
		// and receive the signal without the scheduler's participation
		// and can't count on synchronizing any memory via it.
		return myStatus.CmpExchgStrongAcquire(oldStatus, TASK_STATUS_PENDING);
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
		myStatus.StoreRelease(TASK_STATUS_PENDING);
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
