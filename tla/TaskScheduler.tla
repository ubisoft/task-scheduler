----------------------------- MODULE TaskScheduler -----------------------------
\*
\* Multi-threaded cooperative task scheduler with deadlines and wake-ups
\* support. The scheduler operates on jobs/tasks/coroutines - anything what can
\* be executed. Further they are called tasks. It consists of the following
\* components:
\*
\* ## Front.
\* This is where the users put their tasks. It contains a front task queue. And
\* a signal variable which is signaled when the front queue becomes non-empty.
\*
\* ## Scheduler role.
\* The role is single - done by a single thread at a time. It does work of
\* taking the new jobs stacked in the front, checking deadlines, and putting
\* jobs into a next queue - ready-queue - from which they are taken by all the
\* worker threads.
\*
\* Despite the role is single, it is not pinned to one thread. Each worker
\* thread can become a scheduler for a while depending on who is free at the
\* moment. Thanks to that, no need to have a dedicated thread just for
\* scheduling. The task scheduler works even if has just a single thread. Then
\* the latter just switches between being a worker and being a scheduler.
\*
\* ## Worker role.
\* Workers are the threads which are not the scheduler. Worker pops tasks from a
\* ready-queue and executes them until the queue is empty. Then it tries to
\* become a scheduler to populate this queue again, and then becomes a worker to
\* execute them. And so on.
\*
\* A thread's loop could be roughly displayed as:
\*
\*     while (true) {
\*         schedule();
\*         while (task = ready_queue.pop())
\*             task.execute();
\*         wait_ready();
\*     }
\*
\*     schedule() {
\*         if (not atomic_flag_set(global_is_busy))
\*             return;
\*         populate_ready_queue();
\*         if (is_empty(ready_queue))
\*             wait_front();
\*         signal_ready();
\*         atomic_flag_clear(global_is_busy);
\*     }
\*
\* The scheduler supports features:
\*
\* - Deadlines. A task can be given a deadline before it is posted into the
\*   scheduler. Then it is executed at that deadline. Not right away.
\*
\* - Wakeup. A task can be woken up to execute it earlier than the deadline.
\*
\* - Signal. A task can be signaled. It is an atomic wakeup + setting a special
\*   flag. Signal can be used to communicate with the task's function. Very
\*   useful when need to tell the task about some other async job being finished
\*   or request it to do something while the task was sleeping.
\*
\*   For example, if the task wants to do some async work and delete itself,
\*   then signals make it very simple. The task starts the work and goes to
\*   sleep. When work is done, the task is signaled. The scheduler atomically
\*   wakes the task up + sets a flag in it. During execute it will check if the
\*   signal is received. And if it does, then the work is done and the task can
\*   delete self.
\*
\*   Without signals it is very hard to do because of spurious wakeups. The task
\*   could have a deadline and wakeup before the async work was done.
\*
\*   Consider this example (bad, won't work).
\*
\*    // The work the task wants to do asynchronously.
\*    work_func(task) {
\*        do_some_work();
\*        // Set an atomic flag and wakeup the task.
\*        task.is_work_done = true; - - - - - - - - - - - - - - - - - - (1)
\*        scheduler.wakeup(task); - - - - - - - - - - - - - - - - - - - (2)
\*    }
\*
\*   // Task first step - start the async work.
\*   task_start(this) {
\*       // Set an atomic flag that the work is not done.
\*       this.is_work_done = false;
\*       // Start work execution in some other thread and go to sleep.
\*       start_work(this, work_func);
\*       scheduler.post_wait(this, task_wait);
\*   }
\*
\*   task_wait(this) {
\*       // Check for a spurious wakeup.
\*       if (!is_work_done) - - - - - - - - - - - - - - - - - - - - - - (3)
\*           return scheduler.post_wait(this, task_wait);
\*
\*       // The work is done, should be safe to delete self.
\*       handle_result();
\*       delete this; - - - - - - - - - - - - - - - - - - - - - - - - - (4)
\*   }
\*
\*   {
\*       // Launch the task in the global task scheduler.
\*       task = new task();
\*       scheduler.post(task, task_start);
\*   }
\*
\*   This example is broken. The step (4) might crash. Because the steps (3)
\*   and (4) might be executed right between the steps (1) and (2). Then the
\*   task is deleted, and step (2) will try to use deleted memory.
\*
\*   Signals allow to do steps (1) and (2) in a single atomic operation. So
\*   The steps (3) and (4) won't see an intermediate state.
\*
\* The implementation needs threads, mutexes, condition variables, atomics,
\* multi-consumer-single-producer queue, and single-consumer-multi-producer
\* queue. Both queues unbounded.
\*
EXTENDS TLC, Integers, Sequences

--------------------------------------------------------------------------------
\*
\* Constant values.
\*

\* How many times need to execute each task. Tasks are re-posted back to the
\* scheduler until reach the target execution count. To test how signals and
\* wakeups affect the tasks between posts.
CONSTANT ExecTarget
CONSTANT NULL
CONSTANT TaskIDs
\* Worker threads are inside of the scheduler.
CONSTANT WorkerThreadIDs
\* User threads communicate with the scheduler and the tasks. They post new
\* tasks, signal and wakeup them randomly. Do all the same what real users are
\* allowed to do.
CONSTANT UserThreadIDs

\* All the threads and the tasks are symmetrical. Because it does not matter
\* whether a task1 or task2 was woken up or signaled or posted in which order.
\* Or whether a task's state was changed by user thread1 or thread2.
\* The same for worker threads - it is not important whether the scheduler role
\* now lives in worker thread1 or thread2. It just lives somewhere or nowhere.
Perms == Permutations(WorkerThreadIDs) \cup Permutations(UserThreadIDs) \cup
         Permutations(TaskIDs)

--------------------------------------------------------------------------------
\*
\* Variables.
\*
\* One the important types here are signal-variables. These have nothing to do
\* with task signals. Just similar name. They consist of an atomic boolean flag,
\* a condition variable, and a mutex. The flag can be set, can be consumed, and
\* threads can wait on the flag being set. When it becomes set, it will wake up
\* only one of the sleeping threads (condition variable's "notify one").
\*
\* Signal-variable can be implemented in a quite efficient way so that is almost
\* never will touch the mutex and condition variable when under high load.

\* IDs of free tasks available for a next post.
VARIABLE TaskPool
\* All the tasks with their states. In other places always only task IDs are
\* used. It simplifies changing their states and selecting a random task to
\* wakeup/signal.
VARIABLE Tasks

TaskVars == <<TaskPool, Tasks>>

\* Scheduler's internal threads.
VARIABLE WorkerThreads
\* Signal whether ready-queue might be not empty.
VARIABLE IsReadySignaled
\* Multi-consumer-single-producer queue. It is populated by the scheduler role
\* (hence single producer) and hands out tasks to all the worker threads (hence
\* multi-consumer).
VARIABLE ReadyQueue

WorkerVars == <<WorkerThreads, IsReadySignaled, ReadyQueue>>

VARIABLE UserThreads
\* Number of task-signals not yet received but expected to be received. In the
\* end should become 0, but while the tasks didn't finish execution, it can
\* grow. In reality this number makes 0 sense but here it is used for validation
\* that the task signals can not be lost.
VARIABLE UserSignalCount

UserVars == <<UserThreads, UserSignalCount>>

\* Signal whether front-queue becomes non-empty.
VARIABLE IsFrontSignaled
\* Unbounded multi-producer-single-consumer queue. It is populated by user
\* threads with new tasks to execute (hence multi-producer). It is processed by
\* the scheduler role (hence single-consumer).
VARIABLE FrontQueue

FrontVars == <<IsFrontSignaled, FrontQueue>>

\* Atomic flag whether there is a worker thread having the scheduler role.
VARIABLE IsSchedulerTaken
\* Queue of tasks waiting for a deadline. It is used only by the scheduler role.
\* Hence can be a simple binary heap of waiting tasks sorted by deadlines.
VARIABLE WaitingQueue

SchedVars == <<IsSchedulerTaken, WaitingQueue>>

vars == <<TaskVars, WorkerVars, UserVars, FrontVars, SchedVars>>

--------------------------------------------------------------------------------
\*
\* Helper functions.
\*

\* In TLA+ there is no convenient way to change multiple fields of a struct,
\* especially if it is in an array of other structs. Functions here help to make
\* it look sane. For that the order of arguments is inverted - the object to
\* change goes last. Thanks to that, multiple mutations can be written as
\*
\*   SetField(key1, val1,
\*   SetField(key2, val2,
\*   SetField(key3, val3,
\*   Object)))
\*
\* Almost like normal assignment. This is additionally simplified by the fact
\* that there are no struct types. The same setter can be used for any struct,
\* only member name matters.

SetWait(v, b) == [b EXCEPT !.do_wait = v]
SetState(v, b) == [b EXCEPT !.state = v]
SetStatus(v, b) == [b EXCEPT !.status = v]
SetTaskID(v, b) == [b EXCEPT !.task_id = v]
SetExecCount(v, b) == [b EXCEPT !.exec_count = v]

\* The same but for struct arrays.

ArrLen(s) == Len(s)
ArrLast(s) == s[Len(s)]
ArrIsEmpty(s) == Len(s) = 0
ArrPopHead(s) == Tail(s)
ArrSet(i, v, s) == [s EXCEPT ![i] = v]
ArrSetLast(v, s) == ArrSet(ArrLen(s), v, s)
ArrAppend(v, s) == Append(s, v)
ArrSetWait(i, v, s) == ArrSet(i, SetWait(v, s[i]), s)
ArrSetState(i, v, s) == ArrSet(i, SetState(v, s[i]), s)
ArrSetStatus(i, v, s) == ArrSet(i, SetStatus(v, s[i]), s)
ArrSetTaskID(i, v, s) == ArrSet(i, SetTaskID(v, s[i]), s)
ArrSetExecCount(i, v, s) == ArrSet(i, SetExecCount(v, s[i]), s)

\* Constructors.

TaskNew == [
  \* Atomic field. Is used by all the threads a lot.
  status |-> "pending",
  \* Flag whether the task should wait for random duration when gets into the
  \* scheduler next time. It simulates deadlines. Except that having specific
  \* deadline value makes little sense. Instead, TLA+ will check all possible
  \* combinations of which tasks when are woken up by their 'deadlines'.
  \* Non-atomic, set only by the user when he posts the task. In reality this
  \* would be a deadline value as a timestamp.
  do_wait |-> FALSE,
  \* How many times the task was executed already.
  exec_count |-> 0
]

WorkerThreadNew == [
  state |-> "idle",
  \* ID of the task the thread is working with right now. In reality the task
  \* would be just on the thread's stack as a variable.
  task_id |-> NULL
]

UserThreadNew == [
  state |-> "idle",
  \* ID of the task the thread is working with right now. In reality the task
  \* would be just on the thread's stack as a variable.
  task_id |-> NULL
]

Init ==
  /\ TaskPool = TaskIDs
  /\ Tasks = [tid \in TaskIDs |-> TaskNew]
  /\ WorkerThreads = [wid \in WorkerThreadIDs |-> WorkerThreadNew]
  /\ UserThreads = [uid \in UserThreadIDs |-> UserThreadNew]
  /\ UserSignalCount = 0
  /\ IsFrontSignaled = FALSE
  /\ IsReadySignaled = FALSE
  /\ IsSchedulerTaken = FALSE
  /\ FrontQueue = << >>
  /\ WaitingQueue = {}
  /\ ReadyQueue = << >>

\* In the steps below the splitting is done by actions which are visible to
\* other participants. For example, task wakeup consists of its status change +
\* potential push into the front queue - it is split into 2 steps.
\* Actions, which do not change globally visible data or are atomic, can be done
\* in one step. For instance, an atomic increment can be done as a single step.
\* Without spitting read and write.

--------------------------------------------------------------------------------
\* Push a task to the front queue. It state must have been set correspondingly
\* before that.

UserPushTaskFrontDo(uid, tid) ==
  LET w == UserThreads[uid]
      t == Tasks[tid] IN
  /\ Assert(t.exec_count < ExecTarget, "Max exec count")
  \* ---
  /\ FrontQueue' = ArrAppend(tid, FrontQueue)
  \* Signal the front when 'empty' turns into 'non-empty'. This helps to avoid
  \* doing the signal most of the times under high load. Assuming that during
  \* push into the queue it is free to find whether it was empty before. Could
  \* be cheaper to send the signal every time if the front queue is not
  \* efficient enough.
  /\ IF ArrIsEmpty(FrontQueue) THEN
     /\ UserThreads' = ArrSetState(uid, "push_front_signal", UserThreads)
     ELSE
     /\ UserThreads' = ArrSet(uid, UserThreadNew, UserThreads)
  /\ UNCHANGED<<IsFrontSignaled>>

UserPushTaskFrontStart(uid) ==
  LET w == UserThreads[uid] IN
  /\ w.state = "push_front"
  \* ---
  /\ UserPushTaskFrontDo(uid, w.task_id)

UserPushTaskFrontSignal(uid) ==
  LET w == UserThreads[uid] IN
  /\ w.state = "push_front_signal"
  \* ---
  \* Even if the front queue was already consumed by the scheduler, sending the
  \* signal here is not a problem. The scheduler role is ready to spurious
  \* wakeups.
  /\ IsFrontSignaled' = TRUE
  /\ UserThreads' = ArrSet(uid, UserThreadNew, UserThreads)
  /\ UNCHANGED<<FrontQueue>>

UserPushTaskFront(uid) ==
  /\ \/ UserPushTaskFrontStart(uid)
     \/ UserPushTaskFrontSignal(uid)
  /\ UNCHANGED<<TaskPool, Tasks, WorkerVars, SchedVars, UserSignalCount>>

--------------------------------------------------------------------------------
\* Post a task for a next execution with randomly enabled or disabled deadline.

UserPostTaskImpl(uid, tid, DoWait) ==
  LET w == UserThreads[uid]
      t == Tasks[tid] IN
  /\ w.state = "idle"
  /\ t.exec_count < ExecTarget
  \* ---
  /\ Tasks' = ArrSetWait(tid, DoWait, Tasks)
  /\ UserPushTaskFrontDo(uid, tid)

UserPostTaskNoWait(uid, tid) == UserPostTaskImpl(uid, tid, FALSE)

UserPostTaskWait(uid, tid) == UserPostTaskImpl(uid, tid, TRUE)

UserPostTask(uid) ==
  /\ \E tid \in TaskPool:
     /\ \/ UserPostTaskNoWait(uid, tid)
        \/ UserPostTaskWait(uid, tid)
     /\ TaskPool' = TaskPool \ {tid}
  /\ UNCHANGED<<WorkerVars, IsFrontSignaled, SchedVars, UserSignalCount>>

--------------------------------------------------------------------------------
\* Wakeup a task. Works on any task, even if it wasn't posted yet or won't be
\* posted again. Although the tasks which won't be posted again are ignored
\* here. Because are not interesting anyhow but increase TLA+ run time.
\*
\* On ready and signaled tasks it is nop, because they are going to be executed
\* soon anyway. For waiting tasks they are pushed to the front queue so as the
\* scheduler would notice the wakeup and remove the task from the waiting queue.
\*
\* The status check + change can be done using an atomic compare-exchange:
\*
\*   old = nil
\*   old_next = load(&status);
\*   do {
\*       old = old_next;
\*       if (old = 'signaled' || old = 'ready')
\*           return;
\*   } while ((old_next = cmpxchg(&status, 'ready', old)) != old);
\*
\* The loop tries to set 'ready' status until it becomes 'ready' or suddenly a
\* signal arrives. Signals are stronger than wake ups. The compare-exchange
\* protects from 'signal' -> 'ready' overwrite - it would loose the signal.
\*
\* The model below does not use a loop because its result could be still
\* considered atomic as [set 'ready' if not 'signaled']. But in the code it
\* would the loop like above.

UserWakeupTask(uid) ==
  LET w == UserThreads[uid] IN
  /\ w.state = "idle"
  \* ---
  /\ \E tid \in TaskIDs: LET t == Tasks[tid] IN
     /\ t.exec_count < ExecTarget
     /\ t.status # "signaled" /\ t.status # "ready"
     \* ---
     /\ Tasks' = ArrSetStatus(tid, "ready", Tasks)
     /\ IF t.status = "waiting" THEN
        \* Waiting tasks are in the waiting-queue. Need to let the scheduler
        \* know they must be popped from that queue before execution. The
        \* notification works via the front queue. It is safe to post the task
        \* here, because the user can be sure it is not in any other queue now
        \* except waiting-queue, thanks to seeing the old status.
        /\ UserThreads' = ArrSetState(uid, "push_front",
                          ArrSetTaskID(uid, tid,
                          UserThreads))
        ELSE
        \* If the task is not waiting, then it is being executed already, or is
        \* in the front- or ready-queue. Anyway, setting the status is enough.
        \* If it is going to be executed soon, then wakeup worked. If it was
        \* already executed, then at the next post the scheduler will see its
        \* 'ready' status and execute right away.
        /\ UserThreads' = ArrSet(uid, UserThreadNew, UserThreads)
     /\ UNCHANGED<<TaskPool, WorkerVars, FrontVars, SchedVars, UserSignalCount>>

--------------------------------------------------------------------------------
\* Signal a task. Works on any task, even if it wasn't posted yet or won't be
\* posted again. Although the tasks which won't be posted again are ignored
\* here. Because are not interesting anyhow but increase TLA+ run time.
\*
\* Also the model won't send a signal to an already signaled task. It is
\* possible, but signals are not counted anyway. So this would be just nop.
\*
\* Easy to see that task-signal is not literally flag. It is rather a special
\* task status. This is how it allows to do atomic 'wakeup + flag set'. It is
\* a single variable which is changed atomically.

UserSignalTask(uid) ==
  LET w == UserThreads[uid] IN
  /\ w.state = "idle"
  \* ---
  /\ \E tid \in TaskIDs: LET t == Tasks[tid] IN
     /\ t.exec_count < ExecTarget
     /\ t.status # "signaled"
     \* ---
     \* Works just like wakeup, but this status is stronger than 'ready'.
     /\ Tasks' = ArrSetStatus(tid, "signaled", Tasks)
     /\ UserSignalCount' = UserSignalCount + 1
     /\ IF t.status = "waiting" THEN
        /\ UserThreads' = ArrSetState(uid, "push_front",
                          ArrSetTaskID(uid, tid,
                          UserThreads))
        ELSE
        /\ UNCHANGED<<UserThreads>>
  /\ UNCHANGED<<TaskPool, WorkerVars, FrontVars, SchedVars>>

--------------------------------------------------------------------------------
\* All the actions the user threads can do with the tasks.

User(uid) ==
  \/ UserPushTaskFront(uid)
  \/ UserPostTask(uid)
  \/ UserWakeupTask(uid)
  \/ UserSignalTask(uid)

--------------------------------------------------------------------------------
\* Try to perform scheduling.

\* Only one worker thread at a time can become a scheduler. Other will go
\* execute tasks.
SchedEnter(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "idle"
  /\ ~IsSchedulerTaken
  \* ---
  /\ IsSchedulerTaken' = TRUE
  /\ WorkerThreads' = ArrSetState(wid, "sched_check_waiting", WorkerThreads)
  /\ UNCHANGED<<WaitingQueue, FrontVars, Tasks, IsReadySignaled, ReadyQueue>>

\* Put tasks having expired deadlines into the ready-queue from where they are
\* picked up by workers. Deadlines are random so this state will pop random
\* waiting tasks one by one until it stops at an indefinite moment. TLA+ turns
\* it into all possible combinations of which tasks expire when and in which
\* order.
SchedCheckWaiting(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "sched_check_waiting"
  /\ Assert(IsSchedulerTaken, "Is in scheduler")
  \* ---
  \* Do any of these 3 actions depending on which of them are available. TLA+
  \* will try all combinations of them.
  /\ \/ /\ ~ArrIsEmpty(FrontQueue)
        /\ WorkerThreads' = ArrSetState(wid, "sched_check_front", WorkerThreads)
        /\ UNCHANGED<<Tasks, WaitingQueue, ReadyQueue>>
     \/ /\ ArrIsEmpty(FrontQueue)
        /\ WorkerThreads' = ArrSetState(wid, "sched_wait_front", WorkerThreads)
        /\ UNCHANGED<<Tasks, WaitingQueue, ReadyQueue>>
     \/ /\ \E tid \in WaitingQueue: LET t == Tasks[tid] IN
           \* Pop tasks one by one selecting one random at a time. TLA+ will try
           \* all ways of selecting a task.
           /\ WaitingQueue' = WaitingQueue \ {tid}
           /\ IF t.status = "waiting" THEN
              /\ Tasks' = ArrSetStatus(tid, "ready", Tasks)
              /\ ReadyQueue' = ArrAppend(tid, ReadyQueue)
              ELSE
              \* The task can be signaled while waited in the waiting-queue.
              \* Then it is not added to the ready-queue, because it is expected
              \* to arrive via the front queue soon. Should not be both in ready
              \* and front queue at the same time ever.
              /\ Assert(t.status = "signaled" \/ t.status = "ready", "State")
              /\ UNCHANGED<<Tasks, ReadyQueue>>
        /\ UNCHANGED<<WorkerThreads>>
  /\ UNCHANGED<<IsReadySignaled, FrontVars, IsSchedulerTaken>>

\* The next step is to take all the tasks from the front queue and sort them
\* out:
\* - Some of these are new posts without deadlines, for immediate execution;
\* - Some have a deadline and want to wait for it or for a wakeup/signal;
\* - Some are actually wake ups or signals for already waiting tasks stored in
\*   the waiting queue;
SchedCheckFront(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "sched_check_front"
  /\ Assert(IsSchedulerTaken, "Is in scheduler")
  /\ Assert(~ArrIsEmpty(FrontQueue), "Has front queue")
  \* ---
  /\ LET tid == FrontQueue[1] t == Tasks[tid] IN
     /\ IF ArrLen(FrontQueue) = 1 THEN
        \* The entire front queue is consumed. Consume the signal too. This
        \* should be done before the queue becomes empty. Otherwise a new task
        \* might arrive after the last task was popped and just before the front
        \* signal is consumed. Then the queue would be non-empty but without
        \* this signal and that would be a deadlock.
        /\ IsFrontSignaled' = FALSE
        /\ WorkerThreads' = ArrSetState(wid, "sched_wait_front", WorkerThreads)
        ELSE
        /\ UNCHANGED<<IsFrontSignaled, WorkerThreads>>
     /\ FrontQueue' = ArrPopHead(FrontQueue)
     \* Status check + change can be done via atomic compare-exchange.
     \* The 'wait' flag does not need to be atomic. It is never accessed by more
     \* than one thread at a time.
     /\ IF t.do_wait /\ t.status = "pending" THEN
        \* 'Pending' means it is a regular post. And 'wait' means it has a
        \* deadline. Put it into the waiting queue then.
        /\ Tasks' = ArrSetStatus(tid, "waiting", Tasks)
        /\ WaitingQueue' = WaitingQueue \union {tid}
        /\ UNCHANGED<<ReadyQueue>>
        ELSE
        /\ IF t.status = "pending" THEN
           \* Pending but no wait - regular post for immediate execution.
           /\ Tasks' = ArrSetStatus(tid, "ready", Tasks)
           ELSE
           \* Not pending = signal or wakeup. Need to remove it from the waiting
           \* queue if it is there and schedule for execution.
           /\ UNCHANGED<<Tasks>>
        /\ WaitingQueue' = WaitingQueue \ {tid}
        /\ ReadyQueue' = ArrAppend(tid, ReadyQueue)
  /\ UNCHANGED<<IsReadySignaled, IsSchedulerTaken>>

\* If has no ready tasks then wait until anything comes to the front or the
\* closest task-deadline expires. Keep the scheduler role during that. Other
\* threads will sleep on the ready-signal which the scheduler will send when
\* finds ready-tasks. Under big load the ready-queue is rarely empty, so the
\* signals overhead is very low.
SchedWaitFront(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "sched_wait_front"
  /\ Assert(IsSchedulerTaken, "Is in scheduler")
  \* ---
  /\ IF ArrLen(ReadyQueue) = 0 THEN
     /\ WorkerThreads' = ArrSetState(wid, "sched_wait_front_signal",
                                     WorkerThreads)
     ELSE
     /\ WorkerThreads' = ArrSetState(wid, "sched_exit", WorkerThreads)
  /\ UNCHANGED<<Tasks, IsReadySignaled, ReadyQueue, FrontVars, SchedVars>>

\* When got a signal, retry the scheduling.
SchedWaitFrontSignal(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "sched_wait_front_signal"
  /\ IsFrontSignaled \/ WaitingQueue # {}
  /\ Assert(IsSchedulerTaken, "Is in scheduler")
  \* ---
  /\ IsFrontSignaled' = FALSE
  /\ WorkerThreads' = ArrSetState(wid, "sched_check_waiting", WorkerThreads)
  /\ UNCHANGED<<Tasks, IsReadySignaled, ReadyQueue, FrontQueue, SchedVars>>

\* Have tasks in the ready-queue. Exit the scheduler and go help other workers
\* to execute them.
SchedExit(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "sched_exit"
  /\ Assert(IsSchedulerTaken, "Is in scheduler")
  \* ---
  /\ IsSchedulerTaken' = FALSE
  /\ WorkerThreads' = ArrSetState(wid, "sched_signal_ready", WorkerThreads)
  /\ UNCHANGED<<Tasks, ReadyQueue, IsReadySignaled, FrontVars, WaitingQueue>>

\* After the thread finished scheduling, it must raise the ready-signal. Must
\* be done after the scheduler role is freed. If the signal would be emitted
\* with the scheduler role occupied, all the workers could block on ready-signal
\* in their loops.
\*
\* It could happen like this. Assume there are 2 threads. One becomes a
\* scheduler, schedules one task, and is paused by the OS right before the
\* scheduler flag is freed.
\*
\* Then the second thread starts working, executes the ready task, fails to
\* become a sched (the flag is still taken), and goes to infinite sleep waiting
\* for ready tasks.
\*
\* Then the first thread continues execution, frees the flag, also fails to
\* execute anything and also goes to infinite sleep on the same condition. This
\* easily can be reproduced right here if this action and the
\* scheduler-flag-freeing action would swap places.
\*
\* The signal below fixes it so if a thread finished the scheduling, it will
\* either go and execute tasks, or will go sleep, but the signal will wake it
\* again or some another thread to elect a new sched-thread.
\*
\* On the other hand it does not lead to busy-loop. If there are no new expired
\* tasks, eventually one worker will be stuck in scheduler role on waiting for
\* new tasks, and other workers will sleep on waiting for ready tasks.
SchedSignalReady(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "sched_signal_ready"
  \* ---
  /\ IsReadySignaled' = TRUE
  /\ WorkerThreads' = ArrSetState(wid, "worker_execute", WorkerThreads)
  /\ UNCHANGED<<Tasks, ReadyQueue, FrontVars, SchedVars>>

Sched(wid) ==
  /\ \/ SchedEnter(wid)
     \/ SchedCheckWaiting(wid)
     \/ SchedCheckFront(wid)
     \/ SchedWaitFront(wid)
     \/ SchedWaitFrontSignal(wid)
     \/ SchedExit(wid)
     \/ SchedSignalReady(wid)
  /\ UNCHANGED<<TaskPool, UserVars>>

--------------------------------------------------------------------------------
\* Worker role loop.

WorkerEnter(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "idle"
  /\ IsSchedulerTaken
  \* ---
  /\ WorkerThreads' = ArrSetState(wid, "worker_execute", WorkerThreads)
  /\ UNCHANGED<<TaskPool, Tasks, ReadyQueue, IsReadySignaled, UserSignalCount>>

WorkerExecuteStart(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "worker_execute"
  \* ---
  /\ IF ArrIsEmpty(ReadyQueue) THEN
     /\ WorkerThreads' = ArrSetState(wid, "worker_wait", WorkerThreads)
     /\ UNCHANGED<<ReadyQueue>>
     ELSE LET tid == ReadyQueue[1] IN
     /\ WorkerThreads' = ArrSetState(wid, "worker_execute_one",
                         ArrSetTaskID(wid, tid,
                         WorkerThreads))
     /\ ReadyQueue' = ArrPopHead(ReadyQueue)
  /\ UNCHANGED<<TaskPool, Tasks, IsReadySignaled, UserSignalCount>>

\* Wait until the scheduler says there is something in the ready-queue. Only
\* one worker wakes up at a time. If it sees there are tasks really, then it
\* wakes up a next worker for help (via becoming a scheduler and sending the
\* ready-signal again in the end) and starts executing tasks. Workers wake up
\* like domino as long as there are tasks. If the task count is < worker count,
\* then there won't be spurious wakeups.
WorkerExecuteWait(wid) ==
  LET w == WorkerThreads[wid] IN
  /\ w.state = "worker_wait"
  /\ IsReadySignaled
  \* ---
  /\ IsReadySignaled' = FALSE
  /\ WorkerThreads' = ArrSetState(wid, "idle", WorkerThreads)
  /\ UNCHANGED<<TaskPool, Tasks, ReadyQueue, UserSignalCount>>

WorkerExecuteOne(wid) ==
  LET w == WorkerThreads[wid]
      tid == w.task_id
      t == Tasks[tid] IN
  /\ w.state = "worker_execute_one"
  \* ---
  \* Signal consumption can be done as a compare-exchange: set to 'pending' if
  \* is 'signaled'.
  /\ IF t.status = "signaled" THEN
     /\ Assert(UserSignalCount > 0, "User signal count is positive")
     /\ UserSignalCount' = UserSignalCount - 1
     ELSE
     /\ UNCHANGED<<UserSignalCount>>
  /\ Tasks' = ArrSetStatus(tid, "pending",
              ArrSetExecCount(tid, t.exec_count + 1,
              ArrSetWait(tid, NULL,
              Tasks)))
  /\ TaskPool' = TaskPool \union {tid}
  /\ WorkerThreads' = ArrSetState(wid, "worker_execute",
                      ArrSetTaskID(wid, NULL,
                      WorkerThreads))
  /\ UNCHANGED<<IsReadySignaled, ReadyQueue>>

Worker(wid) ==
  /\ \/ WorkerEnter(wid)
     \/ WorkerExecuteStart(wid)
     \/ WorkerExecuteWait(wid)
     \/ WorkerExecuteOne(wid)
  /\ UNCHANGED<<SchedVars, FrontVars, UserThreads>>

--------------------------------------------------------------------------------
\*
\* Actions.
\*

NextWorker == \E wid \in WorkerThreadIDs: Worker(wid) \/ Sched(wid)

NextUser == \E uid \in UserThreadIDs: User(uid)

Next ==
  \/ NextUser
  \/ NextWorker

--------------------------------------------------------------------------------
\*
\* Invariants.
\*

\* In the end all tasks must be executed exactly the needed number of times, all
\* the signals must be empty so the threads are sleeping, and all the queues are
\* empty.
TerminalProperty == <>[](
  /\ TaskPool = TaskIDs
  /\ ~IsFrontSignaled
  /\ ~IsReadySignaled
  /\ IsSchedulerTaken
  /\ ArrIsEmpty(FrontQueue)
  /\ WaitingQueue = {}
  /\ ArrIsEmpty(ReadyQueue)
  /\ \A tid \in TaskIDs: Tasks[tid].exec_count = ExecTarget
  /\ UserSignalCount = 0)

\* A task can never be in 2 places at the same time. Not counting the waiting
\* queue inside of the scheduler. Wakeup from the waiting queue works via the
\* front queue, so sometimes a task can be in 2 places. But then it never goes
\* front waiting to ready queue until is delivered into the scheduler via the
\* front queue.
SinglePlaceInvariant ==
  /\ \A tid \in TaskIDs:
     (IF tid \in TaskPool THEN 1 ELSE 0) +
     (IF \E i \in DOMAIN(FrontQueue): FrontQueue[i] = tid THEN 1 ELSE 0) +
     (IF \E i \in DOMAIN(ReadyQueue): ReadyQueue[i] = tid THEN 1 ELSE 0) +
     (IF \E wid \in WorkerThreadIDs: WorkerThreads[wid].task_id = tid
         THEN 1 ELSE 0) <= 1

TotalInvariant ==
  /\ SinglePlaceInvariant

Spec ==
  /\ Init
  /\ [][Next]_vars
  /\ SF_vars(NextWorker)
  /\ SF_vars(NextUser)

================================================================================
