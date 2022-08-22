# MultiProducerQueue

The tests show `MultiProducerQueue` versus a trivial queue.

The queue has no way to pop items one by one. Only all at the same time.

The trivial queue simply uses mutex locks for each push and 'pop all'. It is perfectly fair in terms of CPU usage, and is even good for many cases.

The benchmarks measure not only busy-looped producers and consumers, but also the more typical usage: blocking consumer.

A consumer rarely can afford having a busy loop or even just a polling loop with sleeps. The more natural usage is when the consumer clears the queue and then sleeps exactly until the queue becomes non-empty. The producers are supposed to notify the consumer about this.

The blocking consumption is implemented using `mg::common::Signal` class. The benchmarks measure both the busy-loop consumption and the blocking consumption.

## Results

The trivial lock-based queue looses to `MultiProducerQueue` in every single bench (except 2 cases on Windows - these are highly unstable, jitter is up to x2 times even on sequential runs).

The queue `MultiProducerQueue` is often faster than the lock-based one x1.5-x2 times. In rare cases can go up to x12 (macOS).

The difference is clearer when producer thread count goes up. That leads to higher mutex contention in the trivial queue. The producers are getting blocked. In the lock-free queue the producers in case of a conflict just retry the push and it appears to be much faster.

In busy-loop benchmarks the mutex contention is incomparable - the lock-free queue simply always has 0 mutex contention.

In blocking consumption benchmarks the `mg::common::Signal` adds some contention even to the lock-free queue, because it has a mutex inside. But nonetheless the lock-free queue remains faster and has generally much less contention. Often the difference is 1-2 orders of magnitude. But in rare cases the blocking consumption can have more contention in `mg::common::Signal` while giving higher RPS. Unclear yet why that happened. Could be the bench being too unstable or the signal having hidden perf issues.
