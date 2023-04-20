#pragma once

#include "mg/common/Atomic.h"
#include "mg/common/TypeTraits.h"

#include <functional>

namespace mg {
namespace serverbox {

	class Task;
	class TaskScheduler;
	class TaskSchedulerThread;

	// The status is an internal thing, not supposed to be used by
	// the external code.
	enum TaskStatus : int32_t
	{
		// Task is pending if it not known yet if it has a
		// deadline in the future. The task can be anywhere except
		// ready queue and waiting queue.
		TASK_STATUS_PENDING,
		// The task is moved to the waiting queue. Or is going to
		// be added there, or was just popped from there. It has
		// a deadline in the future.
		TASK_STATUS_WAITING,
		// Task is going to be executed asap. Or is being executed
		// right now and was woken up during the execution. The
		// task can be anywhere.
		TASK_STATUS_READY,
		// The task is signaled, so will be executed asap. Or is
		// being executed right now, and was signaled during the
		// execution. The task can be anywhere.
		TASK_STATUS_SIGNALED,
	};

	using TaskCallback = std::function<void(Task*)>;

	// Task is a callback called in one of the worker threads in a
	// scheduler. But not just a callback. It also provides some
	// features such as deadlines, delays, signaling. Also it can
	// be inherited to add more context.
	//
	class Task
	{
	public:
		Task();

		// Can construct from a callback, or create a callback
		// in-place.
		template<typename Functor>
		Task(
			Functor&& aFunc);

		~Task();

		// Can set from an existing callback object, or create a
		// callback in-place.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		template<typename Functor>
		void SetCallback(
			Functor&& aFunc);

		// **You must prefer** Deadline or Wait if you want an
		// infinite delay. To avoid unnecessary current time get.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		void SetDelay(
			uint32_t aDelay);

		// Set deadline to a minimal value between the current
		// deadline and the current time + the given delay. It
		// makes sense to use when the task consists of several
		// independent parts which set their own deadlines and it
		// must wakeup by the earliest one. Keep in mind, that
		// when the task just starts execution, its deadline is
		// dropped to 0. To use the feature one can set the
		// deadline to infinity in the beginning of an execution
		// and only then adjust it.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		void AdjustDelay(
			uint32_t aDelay);

		// Setting a deadline may be useful when it is known in
		// advance - allows to avoid at least one call to get the
		// current time.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		void SetDeadline(
			uint64_t aDeadline);

		// The same as delay adjustment but the deadline is passed
		// by the user. Not calculated inside. This is faster when
		// know the deadline in advance.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		void AdjustDeadline(
			uint64_t aDeadline);

		// The task won't be executed again until an explicit
		// wakeup or signal.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		void SetWait();

		// Check if the task has a signal emitted.
		// Can be called anytime.
		bool IsSignaled();

		// Check if the task was woken up naturally, by a
		// deadline, not by an explicit signal or wakeup. Will be
		// always true for tasks not having any deadline. So makes
		// sense only for tasks scheduled with a real deadline.
		// Keep in mind, that the task can be expired and signaled
		// at the same time.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		bool IsExpired() const;

		// Deadline of the task to apply when it is posted next
		// time. Deadline is dropped to 0 when the task starts
		// execution, so this getter makes only sense after a new
		// deadline is set.
		// Can't be called when the task has been posted to the
		// scheduler waiting for execution.
		uint64_t GetDeadline() const;

		// Atomically try to receive a signal. In case of success
		// the signal is cleared, and true is returned. If the
		// task is signaled but ReceiveSignal is not used, it will
		// be rescheduled **ignoring all deadlines** until the
		// signal is received. Also signals don't stack. It means
		// that if multiple signals were sent, they all will be
		// consumed with one receipt.
		// Can be called anytime.
		bool ReceiveSignal();

	private:
		void PrivExecute();

		void PrivCreate();

		void PrivTouch() const;
	public:
		// The comparator is public so as it could be used by the
		// waiting queue.
		bool operator<=(
			const Task& aTask) const;

		// Next is public so as it could be used by the intrusive
		// front queue.
		Task* myNext;
		// Index is public so as it could be used by the intrusive
		// waiting queue.
		int32_t myIndex;
	private:
		mg::common::Atomic<TaskStatus> myStatus;
		uint64_t myDeadline;
		TaskCallback myCallback;
		// True if the task is inside the scheduler in one of its
		// queues. It means it can't be altered anyhow. Only for
		// debug and sanity checks.
		bool myIsInQueues;
		bool myIsExpired;

		friend class TaskScheduler;
	};

	inline
	Task::Task()
	{
		PrivCreate();
	}

	template<typename Functor>
	inline
	Task::Task(
		Functor&& aFunc)
		: myCallback(std::forward<Functor>(aFunc))
	{
		PrivCreate();
	}

	inline
	Task::~Task()
	{
		PrivTouch();
	}

	template<typename Functor>
	inline void
	Task::SetCallback(
		Functor&& aFunc)
	{
		PrivTouch();
		myCallback = std::forward<Functor>(aFunc);
	}

	inline void
	Task::SetWait()
	{
		SetDeadline(MG_DEADLINE_INFINITE);
	}

	inline bool
	Task::IsSignaled()
	{
		return myStatus.LoadAcquire() == TASK_STATUS_SIGNALED;
	}

	inline bool
	Task::operator<=(
		const Task& aTask) const
	{
		return myDeadline <= aTask.myDeadline;
	}

}
}
