# Benchmarks

See `result_...` folders for reports.

## Method

The idea is not just run some target features against artificial load. The point is to compare it with an alternative implementation.

The benchmarks are implemented in multiple executables. Each exe uses one implementation of a certain feature.

For example, take task scheduler feature. There are 2 exes:

* `bench_taskscheduler`. It runs `TaskScheduler`, the real task scheduler with all its features.
* `bench_taskscheduler_trivial`. It runs an alternative task scheduler implemented in a very trivial way, extremely simple. It lacks most features but is quite fast in some scenarios.

Both exes run exactly the same bench scenarios, but using different scheduler implementations. The same works for other features.

## Running

The executables can be run locally either directly or via a script.

**Direct** run is just starting the exe, providing the parameters, observing the output. The parameters better see in the code. Can run individual tests, or can run one of them multiple times and get aggregated info printed. Like min/median/max values of a target metric.

**Script** is how to run extra many benchmarks and compare different implementations. The script `report.py` takes a JSON config which provides exes and scenarios to test; runs the exes on all scenarios; generates a markdown report. Like the ones stored in `result_...` folders. The easiest way to understand how to run it is to look at the example configs and at the source code.
