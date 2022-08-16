**OS**: WSL 1 Linux 4.4.0-19041-Microsoft on Windows 10 Enterprise 19042<br>
**CPU**: Intel(R) Xeon(R) W-2135 CPU @ 3.70GHz, 3696 Mhz, 6 Core(s), 12 Logical Processor(s)

---
**Nano load, 1 thread, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 1 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 15 497 607 tasks per second;
* x1.948 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      14 878 288
Exec per second median:   15 497 607
Exec per second max:      16 017 842

== Median report:
Microseconds per exec:          0.064526
Exec per second:                15497607
Mutex contention count:                0
Thread  0: exec:     10000000, sched:      2001


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       7 783 891
Exec per second median:    7 957 668
Exec per second max:       9 381 048

== Median report:
Microseconds per exec:          0.125665
Exec per second:                 7957668
Mutex contention count:           907263
Thread  0: exec:     10000000, sched:         0
```
</details>

---
**Nano load, 1 thread, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 1 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 20 688 512 tasks per second;
* x1.025 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      19 322 466
Exec per second median:   20 688 512
Exec per second max:      21 157 431

== Median report:
Microseconds per exec:          0.048336
Exec per second:                20688512
Mutex contention count:                0
Thread  0: exec:     50000000, sched:     10003


#### Trivial task scheduler
== Aggregated report:
Exec per second min:      19 794 541
Exec per second median:   20 184 002
Exec per second max:      21 422 790

== Median report:
Microseconds per exec:          0.049544
Exec per second:                20184002
Mutex contention count:           368778
Thread  0: exec:     50000000, sched:         0
```
</details>

---
**Nano load, 2 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 2 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 10 428 025 tasks per second;
* x1.470 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       9 082 529
Exec per second median:   10 428 025
Exec per second max:      11 093 079

== Median report:
Microseconds per exec:          0.191791
Exec per second:                10428025
Exec per second per thread:      5214012
Mutex contention count:              544
Thread  0: exec:      5130340, sched:       499
Thread  1: exec:      4869660, sched:       504


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       6 209 314
Exec per second median:    7 093 869
Exec per second max:       7 311 908

== Median report:
Microseconds per exec:          0.281934
Exec per second:                 7093869
Exec per second per thread:      3546934
Mutex contention count:          1264369
Thread  0: exec:      5003270, sched:         0
Thread  1: exec:      4996730, sched:         0
```
</details>

---
**Nano load, 2 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 2 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 8 476 746 tasks per second;
* x1.263 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       8 398 117
Exec per second median:    8 476 746
Exec per second max:       8 751 153

== Median report:
Microseconds per exec:          0.235940
Exec per second:                 8476746
Exec per second per thread:      4238373
Mutex contention count:             2697
Thread  0: exec:     25129261, sched:      2506
Thread  1: exec:     24870739, sched:      2498


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       6 527 222
Exec per second median:    6 713 498
Exec per second max:       6 801 179

== Median report:
Microseconds per exec:          0.297907
Exec per second:                 6713498
Exec per second per thread:      3356749
Mutex contention count:          6533810
Thread  0: exec:     25129791, sched:         0
Thread  1: exec:     24870209, sched:         0
```
</details>

---
**Nano load, 3 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 3 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 4 903 845 tasks per second;
* x1.147 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 663 894
Exec per second median:    4 903 845
Exec per second max:       5 222 206

== Median report:
Microseconds per exec:          0.611765
Exec per second:                 4903845
Exec per second per thread:      1634615
Mutex contention count:             2326
Thread  0: exec:      3226580, sched:       407
Thread  1: exec:      3417447, sched:       410
Thread  2: exec:      3355973, sched:       400


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       3 877 481
Exec per second median:    4 275 116
Exec per second max:       4 344 520

== Median report:
Microseconds per exec:          0.701735
Exec per second:                 4275116
Exec per second per thread:      1425038
Mutex contention count:          1651172
Thread  0: exec:      3320427, sched:         0
Thread  1: exec:      3545638, sched:         0
Thread  2: exec:      3133935, sched:         0
```
</details>

---
**Nano load, 3 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 3 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 4 800 004 tasks per second;
* x0.793 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 574 035
Exec per second median:    4 800 004
Exec per second max:       4 910 505

== Median report:
Microseconds per exec:          0.624999
Exec per second:                 4800004
Exec per second per thread:      1600001
Mutex contention count:            14057
Thread  0: exec:     16611754, sched:      1899
Thread  1: exec:     16722458, sched:      1907
Thread  2: exec:     16665788, sched:      1911


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       5 775 153
Exec per second median:    6 050 319
Exec per second max:       6 262 654

== Median report:
Microseconds per exec:          0.495842
Exec per second:                 6050319
Exec per second per thread:      2016773
Mutex contention count:          6409273
Thread  0: exec:     16838362, sched:         0
Thread  1: exec:     16737165, sched:         0
Thread  2: exec:     16424473, sched:         0
```
</details>

---
**Nano load, 5 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 5 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 4 886 129 tasks per second;
* x1.921 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 684 216
Exec per second median:    4 886 129
Exec per second max:       5 073 813

== Median report:
Microseconds per exec:          1.023305
Exec per second:                 4886129
Exec per second per thread:       977225
Mutex contention count:             6635
Thread  0: exec:      2018882, sched:       280
Thread  1: exec:      2015336, sched:       292
Thread  2: exec:      1967290, sched:       299
Thread  3: exec:      1995874, sched:       291
Thread  4: exec:      2002618, sched:       281


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 347 362
Exec per second median:    2 544 028
Exec per second max:       2 710 586

== Median report:
Microseconds per exec:          1.965387
Exec per second:                 2544028
Exec per second per thread:       508805
Mutex contention count:          2109806
Thread  0: exec:      2017027, sched:         0
Thread  1: exec:      1981875, sched:         0
Thread  2: exec:      1989962, sched:         0
Thread  3: exec:      2004901, sched:         0
Thread  4: exec:      2006235, sched:         0
```
</details>

---
**Nano load, 5 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 5 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 4 914 803 tasks per second;
* x1.312 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 800 181
Exec per second median:    4 914 803
Exec per second max:       5 034 010

== Median report:
Microseconds per exec:          1.017335
Exec per second:                 4914803
Exec per second per thread:       982960
Mutex contention count:            35864
Thread  0: exec:     10049567, sched:      1072
Thread  1: exec:      9994706, sched:      1133
Thread  2: exec:      9928228, sched:      1090
Thread  3: exec:     10001272, sched:      1074
Thread  4: exec:     10026227, sched:      1035


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       3 640 370
Exec per second median:    3 744 793
Exec per second max:       3 829 644

== Median report:
Microseconds per exec:          1.335187
Exec per second:                 3744793
Exec per second per thread:       748958
Mutex contention count:         12377656
Thread  0: exec:     10143502, sched:         0
Thread  1: exec:     10025344, sched:         0
Thread  2: exec:      9997768, sched:         0
Thread  3: exec:      9993685, sched:         0
Thread  4: exec:      9839701, sched:         0
```
</details>

---
**Nano load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 6 145 540 tasks per second;
* x5.492 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       6 021 208
Exec per second median:    6 145 540
Exec per second max:       6 430 851

== Median report:
Microseconds per exec:          1.627196
Exec per second:                 6145540
Exec per second per thread:       614554
Mutex contention count:            14254
Thread  0: exec:       980954, sched:       119
Thread  1: exec:      1016099, sched:       109
Thread  2: exec:       996079, sched:       108
Thread  3: exec:      1005287, sched:       115
Thread  4: exec:      1012591, sched:       120
Thread  5: exec:      1000330, sched:       128
Thread  6: exec:      1006240, sched:       120
Thread  7: exec:      1002669, sched:       123
Thread  8: exec:       979179, sched:       119
Thread  9: exec:      1000572, sched:       124


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 049 325
Exec per second median:    1 118 912
Exec per second max:       1 148 911

== Median report:
Microseconds per exec:          8.937252
Exec per second:                 1118912
Exec per second per thread:       111891
Mutex contention count:          2306700
Thread  0: exec:      1002987, sched:         0
Thread  1: exec:       997171, sched:         0
Thread  2: exec:       991535, sched:         0
Thread  3: exec:      1000427, sched:         0
Thread  4: exec:       996423, sched:         0
Thread  5: exec:       998181, sched:         0
Thread  6: exec:       998804, sched:         0
Thread  7: exec:      1000451, sched:         0
Thread  8: exec:       987846, sched:         0
Thread  9: exec:      1026175, sched:         0
```
</details>

---
**Nano load, 10 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 10 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 5 995 397 tasks per second;
* x2.544 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       5 983 563
Exec per second median:    5 995 397
Exec per second max:       6 081 058

== Median report:
Microseconds per exec:          1.667946
Exec per second:                 5995397
Exec per second per thread:       599539
Mutex contention count:            73496
Thread  0: exec:      5002342, sched:       593
Thread  1: exec:      5011056, sched:       605
Thread  2: exec:      4896682, sched:       547
Thread  3: exec:      4921558, sched:       554
Thread  4: exec:      5101543, sched:       572
Thread  5: exec:      5076044, sched:       588
Thread  6: exec:      5081025, sched:       560
Thread  7: exec:      5030787, sched:       590
Thread  8: exec:      4939882, sched:       560
Thread  9: exec:      4939081, sched:       552


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 225 373
Exec per second median:    2 356 350
Exec per second max:       2 420 098

== Median report:
Microseconds per exec:          4.243850
Exec per second:                 2356350
Exec per second per thread:       235635
Mutex contention count:         18996938
Thread  0: exec:      4998753, sched:         0
Thread  1: exec:      5036501, sched:         0
Thread  2: exec:      5132566, sched:         0
Thread  3: exec:      4924012, sched:         0
Thread  4: exec:      4977570, sched:         0
Thread  5: exec:      4949334, sched:         0
Thread  6: exec:      5010205, sched:         0
Thread  7: exec:      5070750, sched:         0
Thread  8: exec:      4956418, sched:         0
Thread  9: exec:      4943891, sched:         0
```
</details>

---
**Micro load, 1 thread, 1 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 1 -tasks 1000000 -exes 1
Run count: 5
Summary:
* 622 075 tasks per second;
* almost same as trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         594 833
Exec per second median:      622 075
Exec per second max:         635 377

== Median report:
Microseconds per exec:          1.607523
Exec per second:                  622075
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         586 084
Exec per second median:      624 320
Exec per second max:         657 256

== Median report:
Microseconds per exec:          1.601742
Exec per second:                  624320
Mutex contention count:            13914
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 1 thread, 100 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 1 -tasks 100000 -exes 10
Run count: 5
Summary:
* 631 099 tasks per second;
* x0.975 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         604 051
Exec per second median:      631 099
Exec per second max:         642 778

== Median report:
Microseconds per exec:          1.584536
Exec per second:                  631099
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         632 798
Exec per second median:      647 426
Exec per second max:         673 434

== Median report:
Microseconds per exec:          1.544577
Exec per second:                  647426
Mutex contention count:              590
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 2 threads, 2 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 2 -tasks 2000000 -exes 1
Run count: 5
Summary:
* 1 087 835 tasks per second;
* almost same as trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 039 111
Exec per second median:    1 087 835
Exec per second max:       1 099 451

== Median report:
Microseconds per exec:          1.838513
Exec per second:                 1087835
Exec per second per thread:       543917
Mutex contention count:               45
Thread  0: exec:      1000296, sched:       200
Thread  1: exec:       999704, sched:       196


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 043 272
Exec per second median:    1 094 677
Exec per second max:       1 105 705

== Median report:
Microseconds per exec:          1.827021
Exec per second:                 1094677
Exec per second per thread:       547338
Mutex contention count:           120953
Thread  0: exec:      1002906, sched:         0
Thread  1: exec:       997094, sched:         0
```
</details>

---
**Micro load, 2 threads, 200 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 2 -tasks 200000 -exes 10
Run count: 5
Summary:
* 1 070 293 tasks per second;
* x1.012 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 060 971
Exec per second median:    1 070 293
Exec per second max:       1 139 390

== Median report:
Microseconds per exec:          1.868646
Exec per second:                 1070293
Exec per second per thread:       535146
Mutex contention count:               25
Thread  0: exec:      1020203, sched:       184
Thread  1: exec:       979797, sched:       174


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 037 574
Exec per second median:    1 057 234
Exec per second max:       1 067 458

== Median report:
Microseconds per exec:          1.891728
Exec per second:                 1057234
Exec per second per thread:       528617
Mutex contention count:            73370
Thread  0: exec:       977589, sched:         0
Thread  1: exec:      1022411, sched:         0
```
</details>

---
**Micro load, 3 threads, 3 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 3 -tasks 3000000 -exes 1
Run count: 5
Summary:
* 1 491 302 tasks per second;
* x1.044 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 475 202
Exec per second median:    1 491 302
Exec per second max:       1 571 564

== Median report:
Microseconds per exec:          2.011665
Exec per second:                 1491302
Exec per second per thread:       497100
Mutex contention count:              188
Thread  0: exec:      1037310, sched:       207
Thread  1: exec:       961394, sched:       188
Thread  2: exec:      1001296, sched:       199


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 402 065
Exec per second median:    1 428 208
Exec per second max:       1 435 091

== Median report:
Microseconds per exec:          2.100534
Exec per second:                 1428208
Exec per second per thread:       476069
Mutex contention count:           355269
Thread  0: exec:      1003592, sched:         0
Thread  1: exec:       988668, sched:         0
Thread  2: exec:      1007740, sched:         0
```
</details>

---
**Micro load, 3 threads, 300 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 3 -tasks 300000 -exes 10
Run count: 5
Summary:
* 1 508 808 tasks per second;
* x1.078 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 432 839
Exec per second median:    1 508 808
Exec per second max:       1 524 397

== Median report:
Microseconds per exec:          1.988324
Exec per second:                 1508808
Exec per second per thread:       502936
Mutex contention count:              267
Thread  0: exec:       983392, sched:       183
Thread  1: exec:       989271, sched:       192
Thread  2: exec:      1027337, sched:       190


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 376 317
Exec per second median:    1 400 131
Exec per second max:       1 458 749

== Median report:
Microseconds per exec:          2.142655
Exec per second:                 1400131
Exec per second per thread:       466710
Mutex contention count:           410764
Thread  0: exec:       998807, sched:         0
Thread  1: exec:      1002687, sched:         0
Thread  2: exec:       998506, sched:         0
```
</details>

---
**Micro load, 5 threads, 5 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 5 -tasks 5000000 -exes 1
Run count: 5
Summary:
* 2 273 644 tasks per second;
* x1.159 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       2 224 813
Exec per second median:    2 273 644
Exec per second max:       2 328 709

== Median report:
Microseconds per exec:          2.199112
Exec per second:                 2273644
Exec per second per thread:       454728
Mutex contention count:             1421
Thread  0: exec:       988467, sched:       188
Thread  1: exec:      1024522, sched:       186
Thread  2: exec:       995364, sched:       184
Thread  3: exec:       984885, sched:       180
Thread  4: exec:      1006762, sched:       186


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 770 567
Exec per second median:    1 962 504
Exec per second max:       2 047 844

== Median report:
Microseconds per exec:          2.547765
Exec per second:                 1962504
Exec per second per thread:       392500
Mutex contention count:          1388531
Thread  0: exec:      1005541, sched:         0
Thread  1: exec:       999698, sched:         0
Thread  2: exec:       992433, sched:         0
Thread  3: exec:       998273, sched:         0
Thread  4: exec:      1004055, sched:         0
```
</details>

---
**Micro load, 5 threads, 500 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 5 -tasks 500000 -exes 10
Run count: 5
Summary:
* 2 268 660 tasks per second;
* x1.294 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       2 131 731
Exec per second median:    2 268 660
Exec per second max:       2 291 739

== Median report:
Microseconds per exec:          2.203944
Exec per second:                 2268660
Exec per second per thread:       453732
Mutex contention count:             1241
Thread  0: exec:       978020, sched:       179
Thread  1: exec:      1013887, sched:       187
Thread  2: exec:      1016703, sched:       191
Thread  3: exec:      1004397, sched:       186
Thread  4: exec:       986993, sched:       176


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 631 693
Exec per second median:    1 753 750
Exec per second max:       1 821 811

== Median report:
Microseconds per exec:          2.851033
Exec per second:                 1753750
Exec per second per thread:       350750
Mutex contention count:          1792885
Thread  0: exec:      1000823, sched:         0
Thread  1: exec:      1009381, sched:         0
Thread  2: exec:       996293, sched:         0
Thread  3: exec:       995224, sched:         0
Thread  4: exec:       998279, sched:         0
```
</details>

---
**Micro load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 3 331 836 tasks per second;
* x2.504 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 224 220
Exec per second median:    3 331 836
Exec per second max:       3 546 759

== Median report:
Microseconds per exec:          3.001348
Exec per second:                 3331836
Exec per second per thread:       333183
Mutex contention count:             9879
Thread  0: exec:      1001433, sched:       170
Thread  1: exec:      1005789, sched:       160
Thread  2: exec:      1015050, sched:       164
Thread  3: exec:      1000039, sched:       164
Thread  4: exec:       984742, sched:       157
Thread  5: exec:       976266, sched:       160
Thread  6: exec:      1008174, sched:       166
Thread  7: exec:      1016330, sched:       173
Thread  8: exec:       990619, sched:       174
Thread  9: exec:      1001558, sched:       157


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 160 292
Exec per second median:    1 330 633
Exec per second max:       1 355 983

== Median report:
Microseconds per exec:          7.515216
Exec per second:                 1330633
Exec per second per thread:       133063
Mutex contention count:          4660750
Thread  0: exec:      1007786, sched:         0
Thread  1: exec:      1008002, sched:         0
Thread  2: exec:       981768, sched:         0
Thread  3: exec:       991773, sched:         0
Thread  4: exec:      1009455, sched:         0
Thread  5: exec:      1007536, sched:         0
Thread  6: exec:      1009975, sched:         0
Thread  7: exec:      1006240, sched:         0
Thread  8: exec:       985340, sched:         0
Thread  9: exec:       992125, sched:         0
```
</details>

---
**Micro load, 10 threads, 1 000 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 10 -tasks 1000000 -exes 10
Run count: 5
Summary:
* 3 491 035 tasks per second;
* x2.402 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 400 968
Exec per second median:    3 491 035
Exec per second max:       3 531 815

== Median report:
Microseconds per exec:          2.864479
Exec per second:                 3491035
Exec per second per thread:       349103
Mutex contention count:            10650
Thread  0: exec:      1008214, sched:       156
Thread  1: exec:      1001888, sched:       155
Thread  2: exec:       992086, sched:       159
Thread  3: exec:      1009998, sched:       161
Thread  4: exec:      1003033, sched:       171
Thread  5: exec:       994606, sched:       160
Thread  6: exec:      1004640, sched:       162
Thread  7: exec:       994051, sched:       152
Thread  8: exec:       989630, sched:       155
Thread  9: exec:      1001854, sched:       150


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 309 854
Exec per second median:    1 453 406
Exec per second max:       1 561 031

== Median report:
Microseconds per exec:          6.880390
Exec per second:                 1453406
Exec per second per thread:       145340
Mutex contention count:          6632276
Thread  0: exec:      1005841, sched:         0
Thread  1: exec:      1007351, sched:         0
Thread  2: exec:       991005, sched:         0
Thread  3: exec:       992234, sched:         0
Thread  4: exec:      1003669, sched:         0
Thread  5: exec:      1001631, sched:         0
Thread  6: exec:      1007785, sched:         0
Thread  7: exec:      1006673, sched:         0
Thread  8: exec:       990819, sched:         0
Thread  9: exec:       992992, sched:         0
```
</details>
