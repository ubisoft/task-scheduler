**OS**: macOS Catalina 10.15.5<br>
**CPU**: Quad-Core Intel Core i7 2.5 GHz, 4 cores, hyper-threading enabled

---
**Nano load, 1 thread, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 1 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 12 364 225 tasks per second;
* x2.663 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      11 760 038
Exec per second median:   12 364 225
Exec per second max:      12 722 152

== Median report:
Microseconds per exec:          0.080878
Exec per second:                12364225
Mutex contention count:                0
Thread  0: exec:     10000000, sched:      2002


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       4 562 739
Exec per second median:    4 642 362
Exec per second max:       4 703 124

== Median report:
Microseconds per exec:          0.215408
Exec per second:                 4642362
Mutex contention count:           122058
Thread  0: exec:     10000000, sched:         0
```
</details>

---
**Nano load, 1 thread, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 1 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 15 933 168 tasks per second;
* x1.685 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      15 641 087
Exec per second median:   15 933 168
Exec per second max:      16 002 909

== Median report:
Microseconds per exec:          0.062762
Exec per second:                15933168
Mutex contention count:                0
Thread  0: exec:     50000000, sched:     10003


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       9 326 742
Exec per second median:    9 455 398
Exec per second max:       9 640 724

== Median report:
Microseconds per exec:          0.105760
Exec per second:                 9455398
Mutex contention count:            52109
Thread  0: exec:     50000000, sched:         0
```
</details>

---
**Nano load, 2 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 2 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 9 794 191 tasks per second;
* x4.994 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       9 700 141
Exec per second median:    9 794 191
Exec per second max:       9 971 191

== Median report:
Microseconds per exec:          0.204203
Exec per second:                 9794191
Exec per second per thread:      4897095
Mutex contention count:              715
Thread  0: exec:      4951335, sched:       535
Thread  1: exec:      5048665, sched:       531


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 941 739
Exec per second median:    1 961 185
Exec per second max:       2 051 702

== Median report:
Microseconds per exec:          1.019792
Exec per second:                 1961185
Exec per second per thread:       980592
Mutex contention count:           713507
Thread  0: exec:      5032921, sched:         0
Thread  1: exec:      4967079, sched:         0
```
</details>

---
**Nano load, 2 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 2 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 10 232 402 tasks per second;
* x2.474 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      10 113 336
Exec per second median:   10 232 402
Exec per second max:      10 290 282

== Median report:
Microseconds per exec:          0.195458
Exec per second:                10232402
Exec per second per thread:      5116201
Mutex contention count:             4419
Thread  0: exec:     24977673, sched:      2520
Thread  1: exec:     25022327, sched:      2515


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       4 070 469
Exec per second median:    4 136 769
Exec per second max:       4 248 458

== Median report:
Microseconds per exec:          0.483469
Exec per second:                 4136769
Exec per second per thread:      2068384
Mutex contention count:          1532598
Thread  0: exec:     25453338, sched:         0
Thread  1: exec:     24546662, sched:         0
```
</details>

---
**Nano load, 3 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 3 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 7 021 941 tasks per second;
* x3.386 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       7 004 223
Exec per second median:    7 021 941
Exec per second max:       7 059 115

== Median report:
Microseconds per exec:          0.427232
Exec per second:                 7021941
Exec per second per thread:      2340647
Mutex contention count:             3234
Thread  0: exec:      3335319, sched:       505
Thread  1: exec:      3336655, sched:       500
Thread  2: exec:      3328026, sched:       496


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 061 549
Exec per second median:    2 073 741
Exec per second max:       2 199 931

== Median report:
Microseconds per exec:          1.446661
Exec per second:                 2073741
Exec per second per thread:       691247
Mutex contention count:          1053862
Thread  0: exec:      3320299, sched:         0
Thread  1: exec:      3322925, sched:         0
Thread  2: exec:      3356776, sched:         0
```
</details>

---
**Nano load, 3 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 3 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 7 488 029 tasks per second;
* x1.857 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       7 452 117
Exec per second median:    7 488 029
Exec per second max:       7 545 701

== Median report:
Microseconds per exec:          0.400639
Exec per second:                 7488029
Exec per second per thread:      2496009
Mutex contention count:            15203
Thread  0: exec:     16648336, sched:      1691
Thread  1: exec:     16674088, sched:      1653
Thread  2: exec:     16677576, sched:      1672


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       3 948 829
Exec per second median:    4 032 050
Exec per second max:       4 113 171

== Median report:
Microseconds per exec:          0.744038
Exec per second:                 4032050
Exec per second per thread:      1344016
Mutex contention count:          5037102
Thread  0: exec:     16767919, sched:         0
Thread  1: exec:     16724444, sched:         0
Thread  2: exec:     16507637, sched:         0
```
</details>

---
**Nano load, 5 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 5 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 7 996 785 tasks per second;
* x8.934 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       7 870 021
Exec per second median:    7 996 785
Exec per second max:       8 045 054

== Median report:
Microseconds per exec:          0.625251
Exec per second:                 7996785
Exec per second per thread:      1599357
Mutex contention count:             6748
Thread  0: exec:      2017134, sched:       251
Thread  1: exec:      2001702, sched:       262
Thread  2: exec:      1979571, sched:       262
Thread  3: exec:      1990935, sched:       257
Thread  4: exec:      2010658, sched:       256


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         877 668
Exec per second median:      895 082
Exec per second max:         924 529

== Median report:
Microseconds per exec:          5.586078
Exec per second:                  895082
Exec per second per thread:       179016
Mutex contention count:          1676458
Thread  0: exec:      1990132, sched:         0
Thread  1: exec:      2007666, sched:         0
Thread  2: exec:      1943651, sched:         0
Thread  3: exec:      1980805, sched:         0
Thread  4: exec:      2077746, sched:         0
```
</details>

---
**Nano load, 5 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 5 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 7 971 107 tasks per second;
* x3.642 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       7 859 015
Exec per second median:    7 971 107
Exec per second max:       8 520 815

== Median report:
Microseconds per exec:          0.627265
Exec per second:                 7971107
Exec per second per thread:      1594221
Mutex contention count:            32987
Thread  0: exec:      9970276, sched:      1061
Thread  1: exec:     10019099, sched:      1009
Thread  2: exec:     10014773, sched:      1046
Thread  3: exec:      9996158, sched:      1031
Thread  4: exec:      9999694, sched:      1033


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 150 457
Exec per second median:    2 188 936
Exec per second max:       2 215 619

== Median report:
Microseconds per exec:          2.284214
Exec per second:                 2188936
Exec per second per thread:       437787
Mutex contention count:          7961744
Thread  0: exec:      9918314, sched:         0
Thread  1: exec:     10034842, sched:         0
Thread  2: exec:     10037178, sched:         0
Thread  3: exec:      9973650, sched:         0
Thread  4: exec:     10036016, sched:         0
```
</details>

---
**Nano load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 7 301 031 tasks per second;
* x11.241 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       7 093 127
Exec per second median:    7 301 031
Exec per second max:       9 223 545

== Median report:
Microseconds per exec:          1.369670
Exec per second:                 7301031
Exec per second per thread:       730103
Mutex contention count:            10917
Thread  0: exec:       998949, sched:       121
Thread  1: exec:       992347, sched:       116
Thread  2: exec:      1008955, sched:       121
Thread  3: exec:      1026008, sched:       120
Thread  4: exec:       971381, sched:       118
Thread  5: exec:       995493, sched:       119
Thread  6: exec:       990243, sched:       121
Thread  7: exec:       987765, sched:       118
Thread  8: exec:      1005927, sched:       115
Thread  9: exec:      1022932, sched:       117


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         601 528
Exec per second median:      649 529
Exec per second max:         672 996

== Median report:
Microseconds per exec:         15.395765
Exec per second:                  649529
Exec per second per thread:        64952
Mutex contention count:          1746260
Thread  0: exec:       989271, sched:         0
Thread  1: exec:       944610, sched:         0
Thread  2: exec:       979794, sched:         0
Thread  3: exec:      1016919, sched:         0
Thread  4: exec:       975784, sched:         0
Thread  5: exec:      1023286, sched:         0
Thread  6: exec:      1067195, sched:         0
Thread  7: exec:      1026636, sched:         0
Thread  8: exec:       977297, sched:         0
Thread  9: exec:       999208, sched:         0
```
</details>

---
**Nano load, 10 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 10 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 6 891 497 tasks per second;
* x3.093 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       6 672 728
Exec per second median:    6 891 497
Exec per second max:       7 078 826

== Median report:
Microseconds per exec:          1.451063
Exec per second:                 6891497
Exec per second per thread:       689149
Mutex contention count:            52665
Thread  0: exec:      4990528, sched:       602
Thread  1: exec:      4918937, sched:       609
Thread  2: exec:      4967876, sched:       623
Thread  3: exec:      5079868, sched:       625
Thread  4: exec:      5010651, sched:       612
Thread  5: exec:      5066302, sched:       646
Thread  6: exec:      5027478, sched:       626
Thread  7: exec:      4921022, sched:       649
Thread  8: exec:      4995832, sched:       600
Thread  9: exec:      5021506, sched:       638


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 188 798
Exec per second median:    2 227 971
Exec per second max:       2 247 306

== Median report:
Microseconds per exec:          4.488387
Exec per second:                 2227971
Exec per second per thread:       222797
Mutex contention count:         11504262
Thread  0: exec:      5021331, sched:         0
Thread  1: exec:      4955119, sched:         0
Thread  2: exec:      5021631, sched:         0
Thread  3: exec:      4975714, sched:         0
Thread  4: exec:      5010859, sched:         0
Thread  5: exec:      5001878, sched:         0
Thread  6: exec:      5026558, sched:         0
Thread  7: exec:      4994898, sched:         0
Thread  8: exec:      5013212, sched:         0
Thread  9: exec:      4978800, sched:         0
```
</details>

---
**Micro load, 1 thread, 1 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 1 -tasks 1000000 -exes 1
Run count: 5
Summary:
* 459 950 tasks per second;
* x1.041 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         458 424
Exec per second median:      459 950
Exec per second max:         466 204

== Median report:
Microseconds per exec:          2.174149
Exec per second:                  459950
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         436 923
Exec per second median:      441 652
Exec per second max:         444 012

== Median report:
Microseconds per exec:          2.264223
Exec per second:                  441652
Mutex contention count:            13616
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 1 thread, 100 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 1 -tasks 100000 -exes 10
Run count: 5
Summary:
* 461 506 tasks per second;
* x1.010 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         455 528
Exec per second median:      461 506
Exec per second max:         465 013

== Median report:
Microseconds per exec:          2.166818
Exec per second:                  461506
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         451 814
Exec per second median:      456 859
Exec per second max:         461 051

== Median report:
Microseconds per exec:          2.188857
Exec per second:                  456859
Mutex contention count:             1331
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 2 threads, 2 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 2 -tasks 2000000 -exes 1
Run count: 5
Summary:
* 849 264 tasks per second;
* x1.061 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         840 785
Exec per second median:      849 264
Exec per second max:         856 284

== Median report:
Microseconds per exec:          2.354978
Exec per second:                  849264
Exec per second per thread:       424632
Mutex contention count:               33
Thread  0: exec:       999813, sched:       199
Thread  1: exec:      1000187, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         799 446
Exec per second median:      800 715
Exec per second max:         806 295

== Median report:
Microseconds per exec:          2.497766
Exec per second:                  800715
Exec per second per thread:       400357
Mutex contention count:           109595
Thread  0: exec:       999872, sched:         0
Thread  1: exec:      1000128, sched:         0
```
</details>

---
**Micro load, 2 threads, 200 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 2 -tasks 200000 -exes 10
Run count: 5
Summary:
* 851 239 tasks per second;
* x1.037 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         841 364
Exec per second median:      851 239
Exec per second max:         856 066

== Median report:
Microseconds per exec:          2.349515
Exec per second:                  851239
Exec per second per thread:       425619
Mutex contention count:               48
Thread  0: exec:      1000169, sched:       126
Thread  1: exec:       999831, sched:       127


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         812 184
Exec per second median:      820 476
Exec per second max:         831 108

== Median report:
Microseconds per exec:          2.437607
Exec per second:                  820476
Exec per second per thread:       410238
Mutex contention count:            21340
Thread  0: exec:      1000714, sched:         0
Thread  1: exec:       999286, sched:         0
```
</details>

---
**Micro load, 3 threads, 3 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 3 -tasks 3000000 -exes 1
Run count: 5
Summary:
* 1 157 907 tasks per second;
* x1.105 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         732 442
Exec per second median:    1 157 907
Exec per second max:       1 208 822

== Median report:
Microseconds per exec:          2.590880
Exec per second:                 1157907
Exec per second per thread:       385969
Mutex contention count:              240
Thread  0: exec:       999425, sched:       183
Thread  1: exec:       999498, sched:       184
Thread  2: exec:      1001077, sched:       199


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 019 207
Exec per second median:    1 048 210
Exec per second max:       1 064 892

== Median report:
Microseconds per exec:          2.862021
Exec per second:                 1048210
Exec per second per thread:       349403
Mutex contention count:           293150
Thread  0: exec:       999540, sched:         0
Thread  1: exec:      1001502, sched:         0
Thread  2: exec:       998958, sched:         0
```
</details>

---
**Micro load, 3 threads, 300 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 3 -tasks 300000 -exes 10
Run count: 5
Summary:
* 1 167 095 tasks per second;
* x1.026 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 145 449
Exec per second median:    1 167 095
Exec per second max:       1 189 096

== Median report:
Microseconds per exec:          2.570484
Exec per second:                 1167095
Exec per second per thread:       389031
Mutex contention count:              208
Thread  0: exec:      1000324, sched:       103
Thread  1: exec:       998571, sched:       111
Thread  2: exec:      1001105, sched:       124


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 124 878
Exec per second median:    1 137 626
Exec per second max:       1 147 061

== Median report:
Microseconds per exec:          2.637070
Exec per second:                 1137626
Exec per second per thread:       379208
Mutex contention count:            94708
Thread  0: exec:      1000486, sched:         0
Thread  1: exec:       999632, sched:         0
Thread  2: exec:       999882, sched:         0
```
</details>

---
**Micro load, 5 threads, 5 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 5 -tasks 5000000 -exes 1
Run count: 5
Summary:
* 1 480 423 tasks per second;
* x1.396 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 407 021
Exec per second median:    1 480 423
Exec per second max:       1 514 540

== Median report:
Microseconds per exec:          3.377412
Exec per second:                 1480423
Exec per second per thread:       296084
Mutex contention count:             1226
Thread  0: exec:      1007941, sched:       192
Thread  1: exec:       995711, sched:       193
Thread  2: exec:      1004114, sched:       192
Thread  3: exec:       996363, sched:       190
Thread  4: exec:       995871, sched:       193


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 042 837
Exec per second median:    1 060 721
Exec per second max:       1 095 813

== Median report:
Microseconds per exec:          4.713774
Exec per second:                 1060721
Exec per second per thread:       212144
Mutex contention count:           964113
Thread  0: exec:      1003354, sched:         0
Thread  1: exec:      1002591, sched:         0
Thread  2: exec:       999200, sched:         0
Thread  3: exec:       994788, sched:         0
Thread  4: exec:      1000067, sched:         0
```
</details>

---
**Micro load, 5 threads, 500 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 5 -tasks 500000 -exes 10
Run count: 5
Summary:
* 1 444 579 tasks per second;
* x1.416 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 426 066
Exec per second median:    1 444 579
Exec per second max:       1 479 545

== Median report:
Microseconds per exec:          3.461215
Exec per second:                 1444579
Exec per second per thread:       288915
Mutex contention count:             1236
Thread  0: exec:      1007169, sched:       192
Thread  1: exec:       993975, sched:       188
Thread  2: exec:      1003340, sched:       190
Thread  3: exec:       998467, sched:       185
Thread  4: exec:       997049, sched:       189


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         980 282
Exec per second median:    1 020 541
Exec per second max:       1 024 401

== Median report:
Microseconds per exec:          4.899360
Exec per second:                 1020541
Exec per second per thread:       204108
Mutex contention count:           995647
Thread  0: exec:      1002044, sched:         0
Thread  1: exec:       991295, sched:         0
Thread  2: exec:       989664, sched:         0
Thread  3: exec:       998547, sched:         0
Thread  4: exec:      1018450, sched:         0
```
</details>

---
**Micro load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 1 626 402 tasks per second;
* x2.387 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 476 891
Exec per second median:    1 626 402
Exec per second max:       1 641 728

== Median report:
Microseconds per exec:          6.148538
Exec per second:                 1626402
Exec per second per thread:       162640
Mutex contention count:             5273
Thread  0: exec:       995554, sched:       193
Thread  1: exec:      1003995, sched:       177
Thread  2: exec:       999410, sched:       184
Thread  3: exec:       998178, sched:       186
Thread  4: exec:       998179, sched:       184
Thread  5: exec:      1001941, sched:       179
Thread  6: exec:       996666, sched:       189
Thread  7: exec:      1002108, sched:       176
Thread  8: exec:      1003386, sched:       180
Thread  9: exec:      1000583, sched:       182


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         633 756
Exec per second median:      681 301
Exec per second max:         728 004

== Median report:
Microseconds per exec:         14.677788
Exec per second:                  681301
Exec per second per thread:        68130
Mutex contention count:          2177786
Thread  0: exec:      1023206, sched:         0
Thread  1: exec:      1014296, sched:         0
Thread  2: exec:       976660, sched:         0
Thread  3: exec:       965363, sched:         0
Thread  4: exec:      1004673, sched:         0
Thread  5: exec:      1000151, sched:         0
Thread  6: exec:       976677, sched:         0
Thread  7: exec:       965995, sched:         0
Thread  8: exec:      1036895, sched:         0
Thread  9: exec:      1036084, sched:         0
```
</details>

---
**Micro load, 10 threads, 1 000 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 10 -tasks 1000000 -exes 10
Run count: 5
Summary:
* 1 616 423 tasks per second;
* x1.376 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 607 319
Exec per second median:    1 616 423
Exec per second max:       1 628 726

== Median report:
Microseconds per exec:          6.186497
Exec per second:                 1616423
Exec per second per thread:       161642
Mutex contention count:             5733
Thread  0: exec:      1002050, sched:       182
Thread  1: exec:      1002821, sched:       180
Thread  2: exec:       998073, sched:       190
Thread  3: exec:      1002021, sched:       185
Thread  4: exec:       995178, sched:       188
Thread  5: exec:      1000130, sched:       178
Thread  6: exec:       997862, sched:       186
Thread  7: exec:      1001822, sched:       177
Thread  8: exec:      1002459, sched:       181
Thread  9: exec:       997584, sched:       182


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 165 079
Exec per second median:    1 174 891
Exec per second max:       1 187 728

== Median report:
Microseconds per exec:          8.511427
Exec per second:                 1174891
Exec per second per thread:       117489
Mutex contention count:          2853306
Thread  0: exec:      1003299, sched:         0
Thread  1: exec:      1005297, sched:         0
Thread  2: exec:       987773, sched:         0
Thread  3: exec:       994517, sched:         0
Thread  4: exec:       996273, sched:         0
Thread  5: exec:      1000274, sched:         0
Thread  6: exec:      1000594, sched:         0
Thread  7: exec:      1004297, sched:         0
Thread  8: exec:      1005990, sched:         0
Thread  9: exec:      1001686, sched:         0
```
</details>
