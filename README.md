# TaskScheduler

**TaskScheduler** is a set of algorithms for asynchronous threaded code execution.

The scheduler runs so called *tasks* which have a simple API of their own plus user can attach to them anything in form of *callbacks*.

A very simple example:
```C++
TaskScheduler sched("name", thread_count, queue_size);
Task* t = new Task([](Task *self) {
	printf("Executed in scheduler!\n");
	delete self;
});
sched.Post(t);
```

## Description

#### Use cases
Not limited by these, but most typical:
- Single shot mildly heavy tasks;
- Massive load of millions of quick tasks;
- Fair load of heavy multi-step tasks (aka coroutines);

To use the task as coroutines the scheduler offers features like task sleeping, wakeup deadlines, explicit wakeups, signal delivery to individual tasks.

#### Fairness
The task execution is fair. It means they are not pinned to threads. That in turn gives even CPU load on all workers. It will not happen that some tasks appear to be too heavy and occupy one thread's CPU 100% while other threads do nothing.

The execution is also fair in terms of order. Meaning that the tasks start execution in worker threads in the same order as were posted (unless have different deadlines). Although it obviously doesn't guarantee that they end in the same order (unless the worker thread is just one).

#### Performance
The performance depends on thread count and task durations. An example of how it can look:
- 3.70 GHz Intel Xeon;
- 5 worker threads, CPU core count is > 5;
- A task body takes up to tens of nanoseconds, quite short. Worst case scenario for contention.

That gives **more than 5 millions of tasks per second**. The benchmarks are running as part of the testing suite in `test/` folder. That is helpful if want to reproduce and/or measure on another machine.

#### Correctness
The algorithms used in the scheduler are validated in TLA+ specifications to ensure absence of deadlocks, unintentional reordering, task loss, and other logical mistakes.

## Getting Started

### Dependencies

* At least C++11;
* A standard C++ library. On non-Windows also need `pthread` library.
* Compiler support:
	- Windows MSVC;
	- Clang;
	- GCC;
* OS should be any version of Linux, Windows, WSL, Mac. The actual testing was done on:
	- Windows 10;
	- WSLv1;
	- Debian 4.19;
	- MacOS Catalina 10.15.5;
* Supports only architecture x86-64. On ARM it might work, but wasn't compiled nor tested;
* CMake. Compatible with `cmake` CLI and with VisualStudio CMake.

### Build and test

#### Visual Studio
* Open VisualStudio;
* Select "Open a local folder";
* Select `TaskScheduler/` folder where the main `CMakeLists.txt` is located;
* Wait for CMake parsing and configuration to complete;
* Build and run as a normal VS project.

#### CMake CLI
From root of the project:

```Bash
mkdir build
cd build
cmake ../
```
Now can run the tests: `./test/test`.

Useful tips (for clean project, from the `build` folder created above):
* Compiler switch:
	- clang:<br/>
	  `CC=clang CXX=clang++ cmake ../`;
	- GCC:<br/>
	  `CC=gcc CXX=g++ cmake ../`;
* Build type switch:
	- Release, all optimizations, no debug info:<br/>
	  `cmake -DCMAKE_BUILD_TYPE=Release ../`;
	- Release, all optimizations:<br/>
	  `cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ../`;
	- Debug, no optimization at all:<br/>
	  `cmake -DCMAKE_BUILD_TYPE=Debug ../`;

### Installation

Doesn't need an installation. The scheduler builds into a static library: `./build/src/mg/serverbox/libmgserverbox.a` which should be linked with user's project. The header files can be taken from `src/` folder, not all are needed.

However it is important to know that most of the project consists of the scheduler's dependencies which basically re-implement or wrap STL (containers, various thread-related types). That is done in order to keep the code of `TaskScheduler` itself intact from its original version used inside of Ubisoft. Full version of Massgate, where the original scheduler is located, doesn't use STL almost at all.

That means the project is not really supposed to be embedded into some other library or an executable exactly as is. Unless the user is fine with dragging along with it the things like custom array, string, thread, mutex, etc.

The ideal way of "installation" is to take the code, replace all dependencies of `TaskScheduler` with STL or QT or some other library used in user's project, and build it there. For example, replace `mg::common::BinaryHeap` with `std::priority_queue`, `mg::common::Array` with `std::vector`, `mg::common::Callback` with `std::function`, etc. Although it is likely the performance might got down a bit.

This repository is rather one of the possible implementations of the scheduler algorithms + fully covering unit tests than a ready-to-use library.

### Examples

A collection of some simple examples which also cover most of the usages.

**One shot quick task**<br>
Use the scheduler as a thread pool for executing some simple functions.
<details><summary>(click to reveal)</summary>

```C++
TaskScheduler sched(name, threadCount, subQueueSize);

sched.Post(new Task([](Task* aTask) {
	DoMyWork();
	delete aTask;
}))
```
</details>

**Multi-step task, coroutine**<br>
Task can be used as a coroutine which starts some async work on each step, gets a result on wakeup, makes next step, and repeats that until it ends and deletes self. Its context could be stored in a class or a struct. Or bound via lambda capturing. The example shows how a coroutine could send several HTTP requests one by one.
<details><summary>(click to reveal)</summary>

```C++
TaskScheduler sched(name, threadCount, subQueueSize);

class MyCoroutine
{
public:
	MyCoroutine()
		: myTask(this, &MyCoroutine::PrivStep1)
	{
		sched.Post(&myTask);
	}

private:
	void
	PrivStep1(
		Task* aTask)
	{
		assert(aTask == &myTask);
		// When wakeup next time, do the next step.
		myTask.SetCallback(this, &MyCoroutine::PrivStep2);

		// Post self again when complete.
		myClient.Get(firstUrl, [&sched, aTask]() {
			sched.Post(aTask);
		});
	}

	void
	PrivStep2(
		Task* aTask)
	{
		assert(aTask == &myTask);

		// When wakeup next time, do the next step.
		myTask.SetCallback(this, &MyCoroutine::PrivStep3);

		// Post self again when complete.
		myClient.Head(secondUrl, [&sched, aTask]() {
			sched.Post(aTask);
		});
	}

	void
	PrivStep3(
		Task* aTask)
	{
		assert(aTask == &myTask);
		delete this;
	}

	HTTPClient myClient;
	Task myTask;
};

new MyCoroutine();
```
</details>

**Async work with a deadline**<br>
Task deadlines allow to implement any kinds of timeouts. For instance, send an HTTP request, and wakeup either when timeout happens or when the request is finished. The example uses task signal to demonstrate its most canonical purpose.
<details><summary>(click to reveal)</summary>

```C++
TaskScheduler sched(name, threadCount, subQueueSize);

class MyCoroutine
{
public:
	MyCoroutine()
		: myTask(this, &MyCoroutine::PrivStart)
	{
		sched.Post(&myTask);
	}

private:
	void
	PrivStart(
		Task* aTask)
	{
		assert(aTask == &myTask);
		// When wakeup next time, check the result.
		myTask.SetCallback(this, &MyCoroutine::PrivWait);

		// The request should have a timeout 15 sec.
		myTask.SetDelay(15 * 1000);

		myClient.Get(someUrl, &myRequest, [&]() {
			// Atomically wakeup + notify the task about request
			// completion.
			sched.Signal(&myTask);
		});

		// Start waiting.
		sched.Post(&myTask);
	}

	void
	PrivWait(
		Task* aTask)
	{
		assert(aTask == &myTask);

		if (myTask.ReceiveSignal())
		{
			// Completed!
			printf("Result: %s\n", myRequest.ToString());
			delete this;
			return;
		}
		// Otherwise the only wakeup reason here could be that the
		// deadline is due. Cancel the request so it completes
		// faster and sends the signal.
		assert(myTask.IsExpired());
		myClient.Cancel(&myRequest);

		// Continue the waiting.
		sched.PostWait(&myTask);
	}

	HTTPClient myClient;
	Request myRequest;
	Task myTask;
};

new MyCoroutine();
```
</details>

**Task as a long-living async work executor**<br>
Task can live for a long time to be used as a background worker. For example, send requests into the network or collect statistics for monitoring purposes. The example also shows the most typical usage of task wakeup.
<details><summary>(click to reveal)</summary>

```C++
TaskScheduler sched(name, threadCount, subQueueSize);

class MyConnection
{
public:
	MyConnection()
		: myTask(this, &MyConnection::PrivExecute)
	{
		sched.Post(&myTask);
	}

	void
	Send(
		Request* aRequest)
	{
		myQueue.Append(aRequest);

		// Wakeup the task to let it know there is new work to do.
		sched.Wakeup(&myTask);
	}

private:
	void
	PrivExecute(
		Task* aTask)
	{
		assert(aTask == &myTask);

		// Execute all queued requests.
		Request* req = myQueue.PopAll();
		while (req != nullptr)
		{
			Request* next = req->myNext;
			myClient.Put(req->myUrl, req->myData);
			delete req;
			req = next;
		}

		// Wait for more work.
		sched.PostWait(&myTask);
	}

	HTTPClient myClient;
	RequestQueue myQueue;
	Task myTask;
};

MyConnection* conn = new MyConnection();

// Execute the requests in background.
conn->Send(new Request(url1, "some data1"));
conn->Send(new Request(url2, "some data2"));
```
</details>

## Contribution

Contributions can be done via pull requests. There is no a strict list of code style rules, new code simply should resemble the existing code as close as possible. Each patch before sending it on a review should be tested on all supported compilers and operating systems. See the list of them above in dependencies. All features and bugfixes should have unit tests which pass, and the old unit tests also should pass.

## Architecture overview

The scheduler is not a single monolithic algorithm. It is fused from several smaller ones. It is best to see them in the code with comments. Below is a short overview of the big picture.

### Multi-Producer-Single-Consumer Queue

The scheduler receives tasks from the external code via a front queue. It is an unbounded lock-free MPSC queue. See `src\mg\common\MultiProducerQueueIntrusive.h`.

Push is an atomic compare-exchange which in case of simultaneous access by multiple threads is retried. No spinlocks, nor any mutexes.

Pop is only able to take all the tasks at once and the order is reversed. That is the price of it being lock-free, a single atomic exchange. Basically, this queue is a stack, not a list. The order is restored back to normal when the scheduler processes the popped items.

### Multi-Consumer-Single-Producer Queue

This is a backend queue of the scheduler. From here the worker threads pick up the tasks ready for execution. It is an unbounded semi-lock-free MCSP queue. See `src\mg\common\MultiConsumerQueue.h`.

There is no a simple well known algorithm for having multiple consumers which would be at the same time wait-free; lock-free; unbounded; and with ability to pop items one by one. Mostly that is because of the ABA problem of concurrent algorithms. Working them around usually is quite untrivial and can cost notable fraction of performance.

The MCSP queue presented here makes a compromise. It is semi-lock-free. Meaning that it has a mutex, but it is taken extremely rarely. Moreover, the algorithm parameters allow to control the lock frequency.

How it works is that in essence the queue is a mutex-protected list of lock-free bounded queues. Lock-free bounded queue is quite easy to implement both for multi-consumer and for multi-producer cases. To drop the limits it is enough to have an unlimited list of such bounded queues, a list of blocks.

The consumers, when attach to the queue, under a lock remember the current read-position in this list, and then consume the saved block in a lock-free way one item by one. When the block is exhausted, they switch to a next block again under a mutex.

The producer works in a similar way - it remembers the block-list tail, pushes new items into it in a lock-free way, and when it becomes full, the producer appends a new block under a lock.

The critical section here (protected by the mutex) is very narrow - just a switch of a few pointers. What is even better, it depends on block size. The bigger are these lock-free sub-queues, the rarer the lock is taken. For example, if sub-queue size is 10000, then the lock is taken once per 10000 pops.

An example of how the queue can look:

```
                      mutex-protected
                          links
 popped                    ____                write
 items                    /    \              position
+v-v-v----+     +---------+    +---------+    +--v------+
|         | --> |    x x x| -> |x x x x x| -> |x - - - -|
+--- ^----+     +----^----+    +---------+    +---------+
  consumer1       consumer2                  \___________/
  position        position                    write block
```
- Here the queue has 4 blocks (sub-queues);
- Each sub-queue has size 5 (can do 5 lock-free pops until need to lock the mutex);
- The links between the blocks are protected by a mutex;
- There are 2 consumers. One of them is a bit behind. It will catch up on next pop;
- The last block is for new items. It has space for 4 more items.

Compared to a trivial queue which would take a mutex lock on each pop and push, this MCSP queue under extreme contention is faster up to x10 times according to benchmarks. For reference one can run tests and observe the output to find out how it performs on various hardware.

For instance there is a bench with 6 threads doing constant pops and 1 thread doing 1 000 000 pushes. It was run on MCSP queue and on the trivial intrusive list with mutex lock on each pop and push. The bench measured test duration.

```
Windows desktop:
  - Trivial queue: 1228 ms;
  - MCSP:           212 ms;

Linux WSL 1:
  - Trivial queue: 709 ms;
  - MCSP:          152 ms;
```

### Signal

Signal is a thread synchronization type, similar to mutex and condition variable. It allows to send an event from one thread to another in a blocking way, but it has a few most common cases optimized to be lock-free. See `src\mg\common\Signal.h`.

The scheduler uses it to
- Notify the worker threads about new ready-to-do tasks coming;
- Notify self that there are new tasks in the front queue.

The API looks like an IO object like pipe or a socket but allows to send and receive just one bit, a flag, which notifies a receiver about some occurred event. In a simplified way this is what it does:
- Send: set the flag;
- Receive: wait until the flag is set, then clear it.

A trivial signal could be implemented like a mutex + a condition variable + a flag. Each send or receive would take the lock, do the operation, free the lock. On receive optionally wait on the condition variable. On send notify the condition variable.

The signal used in the scheduler has some optimizations on top:
- Sending is lock-free if the flag is still not consumed since last send;
- Receipt is lock-free if the flag is installed. The mutex is locked only if couldn't receive right away.

These simple changes eliminate most of the contention.

### Task Scheduler

It would be good firstly to agree on what is scheduling (in the current context). The scheduling is taking the incoming tasks; doing some pre-processing on them; spreading read-to-do tasks over the worker threads for execution.

The scheduler itself is a set of threads. All the threads are equal, meaning that there is no a dedicated *"thread doing the scheduling"* and *"threads executing tasks"*. They all do the scheduling and execute the tasks together.

This is achieved by making the actual scheduling a migrating role, which is further called the "sched-role". The sched-role is taken by one of the workers at a time, whoever of them was free at the moment. That thread does the scheduling and then joins the other threads for executing the ready tasks. When all the ready tasks are done, the threads again compete for who would do the next round of scheduling. And so on.

A very simplified pseudocode of a worker thread looks as follows:

```C++
while(true) {
	if (TakeSchedulerRole())
	{
		ProcessFrontTasks();
		PopulateReadyQueue();
		FreeSchedulerRole();
	}
	while (readyQueue->Pop(task))
		task->Execute();
}
```

This guarantees that if there is work to do, then all the threads will do it equally. Each of them either executes ready tasks or prepares new tasks. No idle threads unless there is no work to do. That in turn gives even CPU load on all the threads.

Having the sched-role always executed in at most one thread at a time allows to do in there untrivial logic and not care about mutexes (for example, use plain arrays, lists, heaps, etc). It is not just a channel between multi-producer front queue and multi-consumer ready queue.

The tasks between these queues go through some preprocessing.

#### Task deadline

The tasks have optional deadlines. It is quite a useful feature for
- Periodic tasks. For instance, a task can wakeup once per second to report statistics for monitoring;
- Timeouts. A task could send a request (an HTTP request maybe) and wants to wakeup after a few seconds to check on response. Then it sends the request, sets the deadline, and will be executed not right away but when the deadline expires.

In order to support the deadlines the sched-role has a waiting prioritized queue (a binary heap) sorted by deadlines. The tasks received from the front queue firstly are checked for their deadline. If it is in the future, the task goes into the waiting queue. Otherwise goes straight for execution into the ready queue.

When the sched-role executes next time, it takes all the expired tasks out of the waiting queue and puts them for execution into the ready queue.

#### Task wakeup

The tasks can be explicitly woken up before their deadline. Combined with the deadlines, it makes the tasks quite a powerful concept. Essentially, turns them into coroutines but without an own stack. All the context needs to be stored explicitly somewhere (class or struct on the heap maybe).

For instance, consider again the example about sending a request. A task sent an HTTP request into an HTTP client and wants to wakeup either on timeout or when a response arrives. Then it sets the deadline to now + timeout, and on the response receipt it wakes self up. When awake, it checks whether it is expired and either processes the response or aborts the timed out HTTP request.

Wake up is a complicated operation, because it requires to notify the scheduler so as it would remove the task from its waiting queue and would put it out for execution. The problem is that at the moment of wakeup the task can be anywhere. It can be still in the front queue, already in the waiting queue, or be already prepared for execution in the ready queue, or even being executed right now.

To rule out what to do each task has an atomic state which is updated using only lock-free operations. The state is updated every time when the task moves through the pipeline. And on each update like wakeup the caller can tell what to do with the task (and whether need to do anything).

For example, when task is pushed into the front queue, it is `PENDING`. When it is already scheduled for execution, it is `READY`.

When the sched-role receives a task, it atomically makes `set WAITING if was PENDING`. If the operation fails, it means the task is `READY`, the sched-role puts it for execution now.

When wakeup happens in any place of user code, it does `set READY if was PENDING or WAITING`. If it succeeds and the state was `WAITING`, then the scheduler needs to be notified about the wakeup. The task is pushed into the front queue then.

It is a small piece of task's lock-free state machine, but it gives the main idea. Via atomic compare-exchange it is always possible to tell whether the task is going to be executed soon, and whether the scheduler needs to be notified about a wakeup explicitly.

#### Task signal

Task signal is a feature which allows to do atomically 2 actions: set a flag + wakeup the task. Why would it be needed instead of making a separate atomic flag set + normal wakeup? The reason can be demonstrated on a simple example.

Assume the task started async work somewhere. It wants to be woken up either when the work is done or when a timeout happens, and then delete self. Here is pseudocode:

```C++
void
FinishSomeWork()
{
	isReady = true;                 // (1)
	scheduler.Wakeup(task);         // (2)
}

void
TaskBody(Task* aTask)
{
	if (isReady)
	{
		// Work is done.
		delete this;
		return;
	}
	// Timed out.
	assert(aTask->IsExpired());
	isCanceled = true;
	// Wait for the work cancel.
	scheduler.PostWait(aTask);
}

void
TaskStart()
{
	scheduler.PostDeadline(new Task(TaskBody), 30 * 1000);
}

// FinishSomeWork() is called somewhere after TaskStart().
```

This code is broken. Because after (1) the task may be woken up by a timeout, will see `isReady`, and will delete itself. Then `Wakeup()` in (2) will crash.

Task signal feature allows to do `FinishSomeWork()` atomically. See an example above in this document in the *"Examples"* section.

Signal is implemented as a special task state `SIGNALED`. It is set in a lock-free way similar to how wakeup works. But the difference is that the task in its body can notice that it was explicitly signaled, not just woken up regularly. Then it can take some action like safely delete itself.

Signals can be applied to various scenarios, not just as a deletion helper. For example, a multistep task on each step might start some async work and on wakeup treat the signal as a sign of the work being finished instead of timed out. And start a next step instead of deleting self.

Worth mentioning that signals are not obligatory to use. In the scenario above the problem could also be solved by reference counting, but it would force user to 1) store a reference counter, 2) actual deletion might happen not in the task body (hence not in a worker thread), but somewhere else - that sometimes is undesirable if deletion is heavy.
