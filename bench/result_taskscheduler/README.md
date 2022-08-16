# TaskScheduler

The tests show `TaskScheduler` versus a trivial scheduler.

The trivial scheduler simply uses mutex locks for each task post and in each worker thread to get a task for execution. It is perfectly fair in terms of CPU usage, doesn't pin tasks to worker threads, and isn't even that bad in performance for some cases.

The result reports demonstrate how much more severe mutex contention is in the trivial task scheduler and how significantly its performance degrades with more threads added. Although on certain runs it can be close to `TaskScheduler` when contention is low.

**Note** however, that the trivial scheduler is not a drop-in replacement of `TaskScheduler` - it lacks all the features except dumb execution of tasks: no task deadline, no task wakeup, no task signal, nothing else. Any attempt to add those features purely on mutex-based logic would require to rework it from scratch and inevitably make it unusably slower.

Nonetheless, if the coroutine features are not needed, expected task/second load is going to be low, and thread count will be very small, then the trivial scheduler might be just fine for your case due to its extreme simplicity.

## Results

See the `.md` files in the same folder for details. Overall summary is that `TaskScheduler` easily provides more than million tasks executed per second. In certain runs it can even reach 13 000 000. Can for sure say that if the tasks do any kind of work, the scheduler itself won't be a bottleneck in any application.

Moreover, despite being quite overloaded with lots of features, in almost all cases it still outperforms the lock-based trivial task scheduler. When contention is low, the win isn't big either. But in certain runs apparently the speed difference reaches x7 on Linux, x11 on Mac, x5 on Windows.
