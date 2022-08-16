# MultiConsumerQueue

The tests show `MultiConsumerQueue` versus a trivial queue.

The trivial queue simply uses mutex locks for each push and pop. It has no explicit 'consumer' objects, is perfectly fair in terms of CPU usage, and is even good for many cases. But its performance degrades quickly as more threads are added and contention increases.

## Results

The results very greatly depending on kind of load. The benchmarks test the cases when each popped item is just immediately discarded (empty load), and when the consumer thread spends on it a few microseconds simulating its processing (micro load).

When the load is empty, the trivial queue outperforms `MultiConsumerQueue` at first a bit on some platforms, but quickly degrades. With more and more threads added the trivial queue eventually is unable to process even a million items per second due to extreme contention. It hits the bottom with 10 threads when the trivial queue is >x4.5 times slower.

When the load is micro, the results are roughly the same. Indeed, most of the time is just spent on this fake 'processing', so both trivial queue and `MultiConsumerQueue` perform almost equally. Except that the trivial queue still hits mutex contention >x100 times more often.

Can for sure say that if the consumers do any kind of work on the popped items, the queue itself won't be a bottleneck in any application.

## Alternative

If the reader is interested, they can also take a look at https://github.com/cameron314/concurrentqueue. It is a multi-producer-multi-consumer queue which seems to use logic similar to `MultiConsumerQueue`.

It also has benchmarks: https://moodycamel.com/blog/2014/a-fast-general-purpose-lock-free-queue-for-c++.htm#benchmarks.

Although these benches might be either too outdated or have accidentally lowered perf of other queues. For example, consider a 64bit instance, 8 cores 2.13GHz each. There are 2 consuming threads, the queue is pre-populated. The queue is lock-based - mutex on each push and pop.

```
Moodycamel lock-based queue:  1 340 000 pops per second;
My trivial lock-based queue: 10 122 771 pops per second;
```

Hard to tell why their results are so low for the lock-based queue, but it might make all their results incomparable to the ones presented here.
