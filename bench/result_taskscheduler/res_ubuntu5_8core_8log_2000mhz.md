**OS**: Ubuntu 5.4.0-42-generic<br>
**CPU**: Intel Core Processor (Haswell, no TSX, IBRS) 2.0GHz, 8 cores, 1 thread per core

---
**Nano load, 1 thread, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 1 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 8 614 223 tasks per second;
* x2.980 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       7 035 418
Exec per second median:    8 614 223
Exec per second max:       9 123 559

== Median report:
Microseconds per exec:          0.116087
Exec per second:                 8614223
Mutex contention count:                0
Thread  0: exec:     10000000, sched:      2001


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 745 023
Exec per second median:    2 890 826
Exec per second max:       3 297 389

== Median report:
Microseconds per exec:          0.345922
Exec per second:                 2890826
Mutex contention count:          3723604
Thread  0: exec:     10000000, sched:         0
```
</details>

---
**Nano load, 1 thread, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 1 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 10 864 411 tasks per second;
* x1.210 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      10 210 268
Exec per second median:   10 864 411
Exec per second max:      11 237 897

== Median report:
Microseconds per exec:          0.092044
Exec per second:                10864411
Mutex contention count:                0
Thread  0: exec:     50000000, sched:     10003


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       8 708 394
Exec per second median:    8 982 195
Exec per second max:       9 694 881

== Median report:
Microseconds per exec:          0.111331
Exec per second:                 8982195
Mutex contention count:          2385962
Thread  0: exec:     50000000, sched:         0
```
</details>

---
**Nano load, 2 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 2 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 5 682 078 tasks per second;
* x1.939 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 997 658
Exec per second median:    5 682 078
Exec per second max:       7 007 215

== Median report:
Microseconds per exec:          0.351984
Exec per second:                 5682078
Exec per second per thread:      2841039
Mutex contention count:              527
Thread  0: exec:      4563666, sched:       475
Thread  1: exec:      5436334, sched:       526


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 674 474
Exec per second median:    2 930 149
Exec per second max:       2 983 323

== Median report:
Microseconds per exec:          0.682559
Exec per second:                 2930149
Exec per second per thread:      1465074
Mutex contention count:          2744822
Thread  0: exec:      4935920, sched:         0
Thread  1: exec:      5064080, sched:         0
```
</details>

---
**Nano load, 2 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 2 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 6 405 521 tasks per second;
* x2.720 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       6 194 226
Exec per second median:    6 405 521
Exec per second max:       6 723 705

== Median report:
Microseconds per exec:          0.312231
Exec per second:                 6405521
Exec per second per thread:      3202760
Mutex contention count:             3932
Thread  0: exec:     25086784, sched:      2461
Thread  1: exec:     24913216, sched:      2550


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 904 571
Exec per second median:    2 355 014
Exec per second max:       2 961 630

== Median report:
Microseconds per exec:          0.849252
Exec per second:                 2355014
Exec per second per thread:      1177507
Mutex contention count:         28775511
Thread  0: exec:     25112443, sched:         0
Thread  1: exec:     24887557, sched:         0
```
</details>

---
**Nano load, 3 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 3 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 3 608 070 tasks per second;
* x2.077 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 537 096
Exec per second median:    3 608 070
Exec per second max:       4 264 305

== Median report:
Microseconds per exec:          0.831469
Exec per second:                 3608070
Exec per second per thread:      1202690
Mutex contention count:             2800
Thread  0: exec:      3174038, sched:       399
Thread  1: exec:      3447665, sched:       399
Thread  2: exec:      3378297, sched:       395


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 238 339
Exec per second median:    1 737 461
Exec per second max:       2 160 093

== Median report:
Microseconds per exec:          1.726657
Exec per second:                 1737461
Exec per second per thread:       579153
Mutex contention count:          3724202
Thread  0: exec:      3262237, sched:         0
Thread  1: exec:      3196832, sched:         0
Thread  2: exec:      3540931, sched:         0
```
</details>

---
**Nano load, 3 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 3 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 4 357 617 tasks per second;
* x1.345 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 074 684
Exec per second median:    4 357 617
Exec per second max:       4 464 403

== Median report:
Microseconds per exec:          0.688450
Exec per second:                 4357617
Exec per second per thread:      1452539
Mutex contention count:            14642
Thread  0: exec:     16479652, sched:      1769
Thread  1: exec:     16822236, sched:      1787
Thread  2: exec:     16698112, sched:      1700


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       3 189 444
Exec per second median:    3 240 415
Exec per second max:       3 404 827

== Median report:
Microseconds per exec:          0.925807
Exec per second:                 3240415
Exec per second per thread:      1080138
Mutex contention count:         18074298
Thread  0: exec:     16513349, sched:         0
Thread  1: exec:     16671956, sched:         0
Thread  2: exec:     16814695, sched:         0
```
</details>

---
**Nano load, 5 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 5 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 4 101 520 tasks per second;
* x3.926 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 906 244
Exec per second median:    4 101 520
Exec per second max:       4 289 813

== Median report:
Microseconds per exec:          1.219060
Exec per second:                 4101520
Exec per second per thread:       820304
Mutex contention count:             6733
Thread  0: exec:      1992656, sched:       199
Thread  1: exec:      2031532, sched:       179
Thread  2: exec:      2031902, sched:       183
Thread  3: exec:      1967024, sched:       201
Thread  4: exec:      1976886, sched:       188


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         928 776
Exec per second median:    1 044 663
Exec per second max:       1 187 262

== Median report:
Microseconds per exec:          4.786232
Exec per second:                 1044663
Exec per second per thread:       208932
Mutex contention count:          4089530
Thread  0: exec:      2019695, sched:         0
Thread  1: exec:      2025559, sched:         0
Thread  2: exec:      1965145, sched:         0
Thread  3: exec:      2013528, sched:         0
Thread  4: exec:      1976073, sched:         0
```
</details>

---
**Nano load, 5 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 5 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 4 530 907 tasks per second;
* x2.303 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 404 943
Exec per second median:    4 530 907
Exec per second max:       4 638 347

== Median report:
Microseconds per exec:          1.103532
Exec per second:                 4530907
Exec per second per thread:       906181
Mutex contention count:            31602
Thread  0: exec:      9838016, sched:      1008
Thread  1: exec:      9702084, sched:       958
Thread  2: exec:     10235646, sched:       884
Thread  3: exec:      9973600, sched:       940
Thread  4: exec:     10250654, sched:       924


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 886 652
Exec per second median:    1 967 278
Exec per second max:       2 185 074

== Median report:
Microseconds per exec:          2.541582
Exec per second:                 1967278
Exec per second per thread:       393455
Mutex contention count:         29218076
Thread  0: exec:     10005031, sched:         0
Thread  1: exec:      9882813, sched:         0
Thread  2: exec:     10519824, sched:         0
Thread  3: exec:      9820709, sched:         0
Thread  4: exec:      9771623, sched:         0
```
</details>

---
**Nano load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 4 453 202 tasks per second;
* x6.543 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 870 200
Exec per second median:    4 453 202
Exec per second max:       4 599 019

== Median report:
Microseconds per exec:          2.245575
Exec per second:                 4453202
Exec per second per thread:       445320
Mutex contention count:            10449
Thread  0: exec:       941571, sched:       127
Thread  1: exec:      1027177, sched:       129
Thread  2: exec:       980090, sched:       142
Thread  3: exec:      1026942, sched:       140
Thread  4: exec:       968351, sched:       128
Thread  5: exec:       960910, sched:       134
Thread  6: exec:      1073172, sched:       142
Thread  7: exec:      1056927, sched:       135
Thread  8: exec:      1024930, sched:       132
Thread  9: exec:       939930, sched:       123


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         640 829
Exec per second median:      680 562
Exec per second max:         713 597

== Median report:
Microseconds per exec:         14.693737
Exec per second:                  680562
Exec per second per thread:        68056
Mutex contention count:          4125773
Thread  0: exec:      1003279, sched:         0
Thread  1: exec:       999853, sched:         0
Thread  2: exec:      1002222, sched:         0
Thread  3: exec:      1004800, sched:         0
Thread  4: exec:       996291, sched:         0
Thread  5: exec:       994285, sched:         0
Thread  6: exec:       998000, sched:         0
Thread  7: exec:      1002339, sched:         0
Thread  8: exec:      1000226, sched:         0
Thread  9: exec:       998705, sched:         0
```
</details>

---
**Nano load, 10 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 10 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 4 737 819 tasks per second;
* x2.946 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 686 025
Exec per second median:    4 737 819
Exec per second max:       4 841 362

== Median report:
Microseconds per exec:          2.110676
Exec per second:                 4737819
Exec per second per thread:       473781
Mutex contention count:            55482
Thread  0: exec:      4911238, sched:       533
Thread  1: exec:      5233063, sched:       559
Thread  2: exec:      4620084, sched:       535
Thread  3: exec:      4797957, sched:       531
Thread  4: exec:      4856219, sched:       558
Thread  5: exec:      5194408, sched:       583
Thread  6: exec:      5180633, sched:       612
Thread  7: exec:      5214963, sched:       560
Thread  8: exec:      4827454, sched:       585
Thread  9: exec:      5163981, sched:       580


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 461 674
Exec per second median:    1 608 478
Exec per second max:       1 825 966

== Median report:
Microseconds per exec:          6.217056
Exec per second:                 1608478
Exec per second per thread:       160847
Mutex contention count:         33697929
Thread  0: exec:      5001474, sched:         0
Thread  1: exec:      4990484, sched:         0
Thread  2: exec:      5000253, sched:         0
Thread  3: exec:      5002941, sched:         0
Thread  4: exec:      4972646, sched:         0
Thread  5: exec:      4970155, sched:         0
Thread  6: exec:      5034575, sched:         0
Thread  7: exec:      4993926, sched:         0
Thread  8: exec:      5010806, sched:         0
Thread  9: exec:      5022740, sched:         0
```
</details>

---
**Micro load, 1 thread, 1 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 1 -tasks 1000000 -exes 1
Run count: 5
Summary:
* 336 168 tasks per second;
* x1.023 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         334 789
Exec per second median:      336 168
Exec per second max:         338 110

== Median report:
Microseconds per exec:          2.974700
Exec per second:                  336168
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         323 018
Exec per second median:      328 692
Exec per second max:         330 086

== Median report:
Microseconds per exec:          3.042361
Exec per second:                  328692
Mutex contention count:            21659
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 1 thread, 100 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 1 -tasks 100000 -exes 10
Run count: 5
Summary:
* 334 865 tasks per second;
* x1.031 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         329 350
Exec per second median:      334 865
Exec per second max:         337 398

== Median report:
Microseconds per exec:          2.986271
Exec per second:                  334865
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         304 609
Exec per second median:      324 686
Exec per second max:         330 486

== Median report:
Microseconds per exec:          3.079898
Exec per second:                  324686
Mutex contention count:             2764
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 2 threads, 2 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 2 -tasks 2000000 -exes 1
Run count: 5
Summary:
* 618 906 tasks per second;
* x1.020 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         605 159
Exec per second median:      618 906
Exec per second max:         620 006

== Median report:
Microseconds per exec:          3.231504
Exec per second:                  618906
Exec per second per thread:       309453
Mutex contention count:               75
Thread  0: exec:      1009067, sched:       200
Thread  1: exec:       990933, sched:       198


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         581 553
Exec per second median:      606 548
Exec per second max:         618 010

== Median report:
Microseconds per exec:          3.297346
Exec per second:                  606548
Exec per second per thread:       303274
Mutex contention count:           130770
Thread  0: exec:       999922, sched:         0
Thread  1: exec:      1000078, sched:         0
```
</details>

---
**Micro load, 2 threads, 200 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 2 -tasks 200000 -exes 10
Run count: 5
Summary:
* 629 275 tasks per second;
* x1.050 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         623 829
Exec per second median:      629 275
Exec per second max:         629 776

== Median report:
Microseconds per exec:          3.178261
Exec per second:                  629275
Exec per second per thread:       314637
Mutex contention count:               49
Thread  0: exec:      1000005, sched:       137
Thread  1: exec:       999995, sched:       126


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         597 218
Exec per second median:      599 591
Exec per second max:         607 633

== Median report:
Microseconds per exec:          3.335605
Exec per second:                  599591
Exec per second per thread:       299795
Mutex contention count:            64290
Thread  0: exec:      1001853, sched:         0
Thread  1: exec:       998147, sched:         0
```
</details>

---
**Micro load, 3 threads, 3 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 3 -tasks 3000000 -exes 1
Run count: 5
Summary:
* 923 728 tasks per second;
* x1.042 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         896 541
Exec per second median:      923 728
Exec per second max:         930 386

== Median report:
Microseconds per exec:          3.247709
Exec per second:                  923728
Exec per second per thread:       307909
Mutex contention count:              214
Thread  0: exec:       997758, sched:       196
Thread  1: exec:       992694, sched:       196
Thread  2: exec:      1009548, sched:       199


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         803 962
Exec per second median:      886 871
Exec per second max:         895 145

== Median report:
Microseconds per exec:          3.382679
Exec per second:                  886871
Exec per second per thread:       295623
Mutex contention count:           375688
Thread  0: exec:       995280, sched:         0
Thread  1: exec:      1007245, sched:         0
Thread  2: exec:       997475, sched:         0
```
</details>

---
**Micro load, 3 threads, 300 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 3 -tasks 300000 -exes 10
Run count: 5
Summary:
* 925 970 tasks per second;
* x1.031 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         919 282
Exec per second median:      925 970
Exec per second max:         929 451

== Median report:
Microseconds per exec:          3.239845
Exec per second:                  925970
Exec per second per thread:       308656
Mutex contention count:              191
Thread  0: exec:      1011677, sched:       111
Thread  1: exec:       976718, sched:       134
Thread  2: exec:      1011605, sched:       110


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         855 067
Exec per second median:      898 388
Exec per second max:         902 777

== Median report:
Microseconds per exec:          3.339313
Exec per second:                  898388
Exec per second per thread:       299462
Mutex contention count:           137491
Thread  0: exec:       985665, sched:         0
Thread  1: exec:      1007862, sched:         0
Thread  2: exec:      1006482, sched:         0
```
</details>

---
**Micro load, 5 threads, 5 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 5 -tasks 5000000 -exes 1
Run count: 5
Summary:
* 1 512 188 tasks per second;
* x1.099 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 449 855
Exec per second median:    1 512 188
Exec per second max:       1 520 925

== Median report:
Microseconds per exec:          3.306466
Exec per second:                 1512188
Exec per second per thread:       302437
Mutex contention count:              982
Thread  0: exec:      1005691, sched:       194
Thread  1: exec:      1010253, sched:       198
Thread  2: exec:       981168, sched:       191
Thread  3: exec:       990603, sched:       196
Thread  4: exec:      1012285, sched:       200


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 202 340
Exec per second median:    1 375 997
Exec per second max:       1 390 226

== Median report:
Microseconds per exec:          3.633728
Exec per second:                 1375997
Exec per second per thread:       275199
Mutex contention count:          1336517
Thread  0: exec:      1002170, sched:         0
Thread  1: exec:       992464, sched:         0
Thread  2: exec:       993297, sched:         0
Thread  3: exec:      1001920, sched:         0
Thread  4: exec:      1010149, sched:         0
```
</details>

---
**Micro load, 5 threads, 500 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 5 -tasks 500000 -exes 10
Run count: 5
Summary:
* 1 514 713 tasks per second;
* x1.201 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 501 270
Exec per second median:    1 514 713
Exec per second max:       1 520 517

== Median report:
Microseconds per exec:          3.300953
Exec per second:                 1514713
Exec per second per thread:       302942
Mutex contention count:              924
Thread  0: exec:      1003180, sched:       152
Thread  1: exec:       988535, sched:       157
Thread  2: exec:      1003424, sched:       131
Thread  3: exec:      1006244, sched:       150
Thread  4: exec:       998617, sched:       132


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 242 250
Exec per second median:    1 261 476
Exec per second max:       1 358 974

== Median report:
Microseconds per exec:          3.963608
Exec per second:                 1261476
Exec per second per thread:       252295
Mutex contention count:          1835390
Thread  0: exec:      1002640, sched:         0
Thread  1: exec:       996583, sched:         0
Thread  2: exec:       999563, sched:         0
Thread  3: exec:      1007663, sched:         0
Thread  4: exec:       993551, sched:         0
```
</details>

---
**Micro load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 2 245 534 tasks per second;
* x1.479 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       2 199 797
Exec per second median:    2 245 534
Exec per second max:       2 314 554

== Median report:
Microseconds per exec:          4.453282
Exec per second:                 2245534
Exec per second per thread:       224553
Mutex contention count:             7378
Thread  0: exec:      1151457, sched:       209
Thread  1: exec:       986839, sched:       183
Thread  2: exec:       940130, sched:       178
Thread  3: exec:       810839, sched:       145
Thread  4: exec:       976754, sched:       176
Thread  5: exec:      1017383, sched:       185
Thread  6: exec:       955471, sched:       175
Thread  7: exec:      1054762, sched:       193
Thread  8: exec:      1127073, sched:       197
Thread  9: exec:       979292, sched:       172


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 328 386
Exec per second median:    1 518 767
Exec per second max:       1 601 842

== Median report:
Microseconds per exec:          6.584285
Exec per second:                 1518767
Exec per second per thread:       151876
Mutex contention count:          5510207
Thread  0: exec:      1031680, sched:         0
Thread  1: exec:      1002910, sched:         0
Thread  2: exec:       965060, sched:         0
Thread  3: exec:      1017824, sched:         0
Thread  4: exec:      1022622, sched:         0
Thread  5: exec:       976266, sched:         0
Thread  6: exec:      1010888, sched:         0
Thread  7: exec:       995755, sched:         0
Thread  8: exec:       966645, sched:         0
Thread  9: exec:      1010350, sched:         0
```
</details>

---
**Micro load, 10 threads, 1 000 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 10 -tasks 1000000 -exes 10
Run count: 5
Summary:
* 2 334 199 tasks per second;
* x1.703 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       2 310 192
Exec per second median:    2 334 199
Exec per second max:       2 355 176

== Median report:
Microseconds per exec:          4.284125
Exec per second:                 2334199
Exec per second per thread:       233419
Mutex contention count:             7801
Thread  0: exec:      1007272, sched:       170
Thread  1: exec:      1065767, sched:       187
Thread  2: exec:      1089411, sched:       205
Thread  3: exec:      1098217, sched:       188
Thread  4: exec:       977044, sched:       171
Thread  5: exec:       918342, sched:       163
Thread  6: exec:       988926, sched:       181
Thread  7: exec:       888834, sched:       160
Thread  8: exec:       944485, sched:       163
Thread  9: exec:      1021702, sched:       175


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 248 076
Exec per second median:    1 370 820
Exec per second max:       1 543 729

== Median report:
Microseconds per exec:          7.294901
Exec per second:                 1370820
Exec per second per thread:       137082
Mutex contention count:          7247553
Thread  0: exec:      1001747, sched:         0
Thread  1: exec:      1035436, sched:         0
Thread  2: exec:       998649, sched:         0
Thread  3: exec:       979776, sched:         0
Thread  4: exec:      1011898, sched:         0
Thread  5: exec:      1022698, sched:         0
Thread  6: exec:      1019912, sched:         0
Thread  7: exec:      1005882, sched:         0
Thread  8: exec:       966916, sched:         0
Thread  9: exec:       957086, sched:         0
```
</details>
