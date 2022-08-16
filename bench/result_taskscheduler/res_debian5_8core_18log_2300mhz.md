**OS**: Debian 5.10.136-1<br>
**CPU**: Intel(R) Xeon(R) CPU @ 2.30GHz, 8 CPUs, 2 threads per core

---
**Nano load, 1 thread, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 1 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 10 900 839 tasks per second;
* x2.262 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      10 845 545
Exec per second median:   10 900 839
Exec per second max:      10 971 261

== Median report:
Microseconds per exec:          0.091736
Exec per second:                10900839
Mutex contention count:                0
Thread  0: exec:     10000000, sched:      2001


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       4 684 777
Exec per second median:    4 819 654
Exec per second max:       5 781 148

== Median report:
Microseconds per exec:          0.207484
Exec per second:                 4819654
Mutex contention count:          1308292
Thread  0: exec:     10000000, sched:         0
```
</details>

---
**Nano load, 1 thread, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 1 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 13 956 268 tasks per second;
* x1.097 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      13 937 651
Exec per second median:   13 956 268
Exec per second max:      13 993 690

== Median report:
Microseconds per exec:          0.071652
Exec per second:                13956268
Mutex contention count:                0
Thread  0: exec:     50000000, sched:     10003


#### Trivial task scheduler
== Aggregated report:
Exec per second min:      12 477 586
Exec per second median:   12 720 568
Exec per second max:      12 869 624

== Median report:
Microseconds per exec:          0.078613
Exec per second:                12720568
Mutex contention count:           728594
Thread  0: exec:     50000000, sched:         0
```
</details>

---
**Nano load, 2 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 2 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 5 752 058 tasks per second;
* x2.048 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       5 234 205
Exec per second median:    5 752 058
Exec per second max:       6 214 292

== Median report:
Microseconds per exec:          0.347702
Exec per second:                 5752058
Exec per second per thread:      2876029
Mutex contention count:              505
Thread  0: exec:      4847211, sched:       501
Thread  1: exec:      5152789, sched:       502


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 553 028
Exec per second median:    2 808 951
Exec per second max:       2 844 181

== Median report:
Microseconds per exec:          0.712010
Exec per second:                 2808951
Exec per second per thread:      1404475
Mutex contention count:          1996662
Thread  0: exec:      5025945, sched:         0
Thread  1: exec:      4974055, sched:         0
```
</details>

---
**Nano load, 2 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 2 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 6 103 899 tasks per second;
* x1.661 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       6 059 088
Exec per second median:    6 103 899
Exec per second max:       6 210 380

== Median report:
Microseconds per exec:          0.327659
Exec per second:                 6103899
Exec per second per thread:      3051949
Mutex contention count:             2777
Thread  0: exec:     24780651, sched:      2447
Thread  1: exec:     25219349, sched:      2558


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       3 605 480
Exec per second median:    3 675 253
Exec per second max:       3 842 853

== Median report:
Microseconds per exec:          0.544180
Exec per second:                 3675253
Exec per second per thread:      1837626
Mutex contention count:         10074660
Thread  0: exec:     25130465, sched:         0
Thread  1: exec:     24869535, sched:         0
```
</details>

---
**Nano load, 3 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 3 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 3 241 193 tasks per second;
* x1.438 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 208 623
Exec per second median:    3 241 193
Exec per second max:       3 434 153

== Median report:
Microseconds per exec:          0.925585
Exec per second:                 3241193
Exec per second per thread:      1080397
Mutex contention count:             2692
Thread  0: exec:      3227432, sched:       516
Thread  1: exec:      3477465, sched:       573
Thread  2: exec:      3295103, sched:       549


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 147 729
Exec per second median:    2 254 697
Exec per second max:       2 323 242

== Median report:
Microseconds per exec:          1.330555
Exec per second:                 2254697
Exec per second per thread:       751565
Mutex contention count:          2535765
Thread  0: exec:      3310988, sched:         0
Thread  1: exec:      3227903, sched:         0
Thread  2: exec:      3461109, sched:         0
```
</details>

---
**Nano load, 3 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 3 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 3 204 461 tasks per second;
* x0.867 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 109 489
Exec per second median:    3 204 461
Exec per second max:       3 238 054

== Median report:
Microseconds per exec:          0.936195
Exec per second:                 3204461
Exec per second per thread:      1068153
Mutex contention count:            14224
Thread  0: exec:     17051285, sched:      2004
Thread  1: exec:     16392588, sched:      1941
Thread  2: exec:     16556127, sched:      1920


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       3 538 313
Exec per second median:    3 694 283
Exec per second max:       3 987 420

== Median report:
Microseconds per exec:          0.812065
Exec per second:                 3694283
Exec per second per thread:      1231427
Mutex contention count:         11371039
Thread  0: exec:     16571207, sched:         0
Thread  1: exec:     16831612, sched:         0
Thread  2: exec:     16597181, sched:         0
```
</details>

---
**Nano load, 5 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 5 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 4 062 770 tasks per second;
* x3.067 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 002 704
Exec per second median:    4 062 770
Exec per second max:       4 186 195

== Median report:
Microseconds per exec:          1.230687
Exec per second:                 4062770
Exec per second per thread:       812554
Mutex contention count:             6550
Thread  0: exec:      2058467, sched:       288
Thread  1: exec:      1914210, sched:       280
Thread  2: exec:      1903784, sched:       283
Thread  3: exec:      2032467, sched:       290
Thread  4: exec:      2091072, sched:       281


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 197 102
Exec per second median:    1 324 685
Exec per second max:       1 370 353

== Median report:
Microseconds per exec:          3.774481
Exec per second:                 1324685
Exec per second per thread:       264937
Mutex contention count:          3046444
Thread  0: exec:      2023513, sched:         0
Thread  1: exec:      1981661, sched:         0
Thread  2: exec:      2012319, sched:         0
Thread  3: exec:      1983991, sched:         0
Thread  4: exec:      1998516, sched:         0
```
</details>

---
**Nano load, 5 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 5 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 4 145 020 tasks per second;
* x1.613 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 100 982
Exec per second median:    4 145 020
Exec per second max:       4 174 843

== Median report:
Microseconds per exec:          1.206267
Exec per second:                 4145020
Exec per second per thread:       829004
Mutex contention count:            32721
Thread  0: exec:     10594088, sched:      1075
Thread  1: exec:      9542987, sched:      1070
Thread  2: exec:      9478958, sched:      1137
Thread  3: exec:     10177797, sched:       996
Thread  4: exec:     10206170, sched:      1029


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 394 435
Exec per second median:    2 570 127
Exec per second max:       2 587 269

== Median report:
Microseconds per exec:          1.945429
Exec per second:                 2570127
Exec per second per thread:       514025
Mutex contention count:         18944626
Thread  0: exec:      9930900, sched:         0
Thread  1: exec:      9943797, sched:         0
Thread  2: exec:     10119411, sched:         0
Thread  3: exec:     10222775, sched:         0
Thread  4: exec:      9783117, sched:         0
```
</details>

---
**Nano load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 5 245 283 tasks per second;
* x7.559 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       5 178 666
Exec per second median:    5 245 283
Exec per second max:       5 347 713

== Median report:
Microseconds per exec:          1.906475
Exec per second:                 5245283
Exec per second per thread:       524528
Mutex contention count:            11139
Thread  0: exec:      1027557, sched:       139
Thread  1: exec:       979232, sched:       112
Thread  2: exec:       963236, sched:       120
Thread  3: exec:       952250, sched:       117
Thread  4: exec:      1093130, sched:       134
Thread  5: exec:       988072, sched:       105
Thread  6: exec:      1009299, sched:       115
Thread  7: exec:       998264, sched:       109
Thread  8: exec:      1013099, sched:       107
Thread  9: exec:       975861, sched:       122


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         668 834
Exec per second median:      693 894
Exec per second max:         706 131

== Median report:
Microseconds per exec:         14.411404
Exec per second:                  693894
Exec per second per thread:        69389
Mutex contention count:          3482168
Thread  0: exec:       995799, sched:         0
Thread  1: exec:       997990, sched:         0
Thread  2: exec:      1000061, sched:         0
Thread  3: exec:      1000670, sched:         0
Thread  4: exec:       996777, sched:         0
Thread  5: exec:       999899, sched:         0
Thread  6: exec:       995110, sched:         0
Thread  7: exec:      1009253, sched:         0
Thread  8: exec:      1006581, sched:         0
Thread  9: exec:       997860, sched:         0
```
</details>

---
**Nano load, 10 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 10 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 5 145 754 tasks per second;
* x2.847 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       5 057 523
Exec per second median:    5 145 754
Exec per second max:       5 206 640

== Median report:
Microseconds per exec:          1.943349
Exec per second:                 5145754
Exec per second per thread:       514575
Mutex contention count:            59241
Thread  0: exec:      4907783, sched:       439
Thread  1: exec:      4890007, sched:       464
Thread  2: exec:      4975117, sched:       437
Thread  3: exec:      5030181, sched:       461
Thread  4: exec:      4920863, sched:       450
Thread  5: exec:      5239290, sched:       447
Thread  6: exec:      5042325, sched:       441
Thread  7: exec:      4983493, sched:       436
Thread  8: exec:      4982167, sched:       452
Thread  9: exec:      5028774, sched:       489


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 630 040
Exec per second median:    1 807 259
Exec per second max:       1 826 360

== Median report:
Microseconds per exec:          5.533239
Exec per second:                 1807259
Exec per second per thread:       180725
Mutex contention count:         26964485
Thread  0: exec:      5019065, sched:         0
Thread  1: exec:      4998773, sched:         0
Thread  2: exec:      4991449, sched:         0
Thread  3: exec:      4992324, sched:         0
Thread  4: exec:      4989952, sched:         0
Thread  5: exec:      4993463, sched:         0
Thread  6: exec:      5001976, sched:         0
Thread  7: exec:      4992929, sched:         0
Thread  8: exec:      5005829, sched:         0
Thread  9: exec:      5014240, sched:         0
```
</details>

---
**Micro load, 1 thread, 1 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 1 -tasks 1000000 -exes 1
Run count: 5
Summary:
* 292 816 tasks per second;
* x1.011 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         292 417
Exec per second median:      292 816
Exec per second max:         293 702

== Median report:
Microseconds per exec:          3.415105
Exec per second:                  292816
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         288 790
Exec per second median:      289 646
Exec per second max:         290 353

== Median report:
Microseconds per exec:          3.452484
Exec per second:                  289646
Mutex contention count:            12375
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 1 thread, 100 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 1 -tasks 100000 -exes 10
Run count: 5
Summary:
* 294 305 tasks per second;
* x1.018 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         293 771
Exec per second median:      294 305
Exec per second max:         294 676

== Median report:
Microseconds per exec:          3.397834
Exec per second:                  294305
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         287 083
Exec per second median:      289 172
Exec per second max:         289 486

== Median report:
Microseconds per exec:          3.458140
Exec per second:                  289172
Mutex contention count:             2224
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 2 threads, 2 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 2 -tasks 2000000 -exes 1
Run count: 5
Summary:
* 545 038 tasks per second;
* almost same as trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         542 890
Exec per second median:      545 038
Exec per second max:         555 359

== Median report:
Microseconds per exec:          3.669465
Exec per second:                  545038
Exec per second per thread:       272519
Mutex contention count:               38
Thread  0: exec:      1001625, sched:       201
Thread  1: exec:       998375, sched:       200


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         539 638
Exec per second median:      542 852
Exec per second max:         545 757

== Median report:
Microseconds per exec:          3.684242
Exec per second:                  542852
Exec per second per thread:       271426
Mutex contention count:            90240
Thread  0: exec:       999175, sched:         0
Thread  1: exec:      1000825, sched:         0
```
</details>

---
**Micro load, 2 threads, 200 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 2 -tasks 200000 -exes 10
Run count: 5
Summary:
* 543 799 tasks per second;
* almost same as trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         542 651
Exec per second median:      543 799
Exec per second max:         545 231

== Median report:
Microseconds per exec:          3.677825
Exec per second:                  543799
Exec per second per thread:       271899
Mutex contention count:               60
Thread  0: exec:      1000302, sched:       162
Thread  1: exec:       999698, sched:       162


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         536 016
Exec per second median:      538 507
Exec per second max:         542 662

== Median report:
Microseconds per exec:          3.713966
Exec per second:                  538507
Exec per second per thread:       269253
Mutex contention count:            46435
Thread  0: exec:      1000445, sched:         0
Thread  1: exec:       999555, sched:         0
```
</details>

---
**Micro load, 3 threads, 3 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 3 -tasks 3000000 -exes 1
Run count: 5
Summary:
* 798 876 tasks per second;
* x1.015 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         796 750
Exec per second median:      798 876
Exec per second max:         815 191

== Median report:
Microseconds per exec:          3.755272
Exec per second:                  798876
Exec per second per thread:       266292
Mutex contention count:              230
Thread  0: exec:      1002219, sched:       197
Thread  1: exec:       996694, sched:       199
Thread  2: exec:      1001087, sched:       197


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         780 037
Exec per second median:      786 779
Exec per second max:         798 311

== Median report:
Microseconds per exec:          3.813014
Exec per second:                  786779
Exec per second per thread:       262259
Mutex contention count:           274039
Thread  0: exec:      1004010, sched:         0
Thread  1: exec:       999655, sched:         0
Thread  2: exec:       996335, sched:         0
```
</details>

---
**Micro load, 3 threads, 300 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 3 -tasks 300000 -exes 10
Run count: 5
Summary:
* 795 751 tasks per second;
* x1.016 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         791 122
Exec per second median:      795 751
Exec per second max:         797 693

== Median report:
Microseconds per exec:          3.770021
Exec per second:                  795751
Exec per second per thread:       265250
Mutex contention count:              246
Thread  0: exec:       999564, sched:       113
Thread  1: exec:      1000668, sched:       143
Thread  2: exec:       999768, sched:       132


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         778 077
Exec per second median:      783 177
Exec per second max:         784 939

== Median report:
Microseconds per exec:          3.830551
Exec per second:                  783177
Exec per second per thread:       261059
Mutex contention count:           178434
Thread  0: exec:       997296, sched:         0
Thread  1: exec:      1002968, sched:         0
Thread  2: exec:       999736, sched:         0
```
</details>

---
**Micro load, 5 threads, 5 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 5 -tasks 5000000 -exes 1
Run count: 5
Summary:
* 1 161 434 tasks per second;
* x1.082 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 157 508
Exec per second median:    1 161 434
Exec per second max:       1 162 562

== Median report:
Microseconds per exec:          4.305020
Exec per second:                 1161434
Exec per second per thread:       232286
Mutex contention count:             1338
Thread  0: exec:      1121328, sched:       220
Thread  1: exec:      1122262, sched:       213
Thread  2: exec:      1123390, sched:       213
Thread  3: exec:       818052, sched:       155
Thread  4: exec:       814968, sched:       155


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 055 315
Exec per second median:    1 073 257
Exec per second max:       1 100 588

== Median report:
Microseconds per exec:          4.658716
Exec per second:                 1073257
Exec per second per thread:       214651
Mutex contention count:           974035
Thread  0: exec:      1117601, sched:         0
Thread  1: exec:       844219, sched:         0
Thread  2: exec:      1099535, sched:         0
Thread  3: exec:       817388, sched:         0
Thread  4: exec:      1121257, sched:         0
```
</details>

---
**Micro load, 5 threads, 500 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 5 -tasks 500000 -exes 10
Run count: 5
Summary:
* 1 158 933 tasks per second;
* x1.131 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 154 131
Exec per second median:    1 158 933
Exec per second max:       1 166 099

== Median report:
Microseconds per exec:          4.314311
Exec per second:                 1158933
Exec per second per thread:       231786
Mutex contention count:             1241
Thread  0: exec:      1121592, sched:       183
Thread  1: exec:      1121445, sched:       166
Thread  2: exec:       817511, sched:       130
Thread  3: exec:       817509, sched:       124
Thread  4: exec:      1121943, sched:       152


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 022 402
Exec per second median:    1 024 298
Exec per second max:       1 038 736

== Median report:
Microseconds per exec:          4.881391
Exec per second:                 1024298
Exec per second per thread:       204859
Mutex contention count:          1206998
Thread  0: exec:      1037684, sched:         0
Thread  1: exec:       930819, sched:         0
Thread  2: exec:       965225, sched:         0
Thread  3: exec:      1118056, sched:         0
Thread  4: exec:       948216, sched:         0
```
</details>

---
**Micro load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 1 487 141 tasks per second;
* x1.131 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 484 389
Exec per second median:    1 487 141
Exec per second max:       1 498 801

== Median report:
Microseconds per exec:          6.724311
Exec per second:                 1487141
Exec per second per thread:       148714
Mutex contention count:             5130
Thread  0: exec:       845989, sched:       155
Thread  1: exec:      1101429, sched:       208
Thread  2: exec:      1076818, sched:       194
Thread  3: exec:      1021264, sched:       193
Thread  4: exec:       939637, sched:       164
Thread  5: exec:      1087337, sched:       202
Thread  6: exec:       888654, sched:       164
Thread  7: exec:       954051, sched:       169
Thread  8: exec:      1039081, sched:       196
Thread  9: exec:      1045740, sched:       189


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 299 046
Exec per second median:    1 314 312
Exec per second max:       1 362 360

== Median report:
Microseconds per exec:          7.608542
Exec per second:                 1314312
Exec per second per thread:       131431
Mutex contention count:          2734060
Thread  0: exec:      1021401, sched:         0
Thread  1: exec:       986922, sched:         0
Thread  2: exec:       995247, sched:         0
Thread  3: exec:      1019426, sched:         0
Thread  4: exec:      1013511, sched:         0
Thread  5: exec:       995660, sched:         0
Thread  6: exec:       977170, sched:         0
Thread  7: exec:       994436, sched:         0
Thread  8: exec:       994217, sched:         0
Thread  9: exec:      1002010, sched:         0
```
</details>

---
**Micro load, 10 threads, 1 000 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 10 -tasks 1000000 -exes 10
Run count: 5
Summary:
* 1 483 665 tasks per second;
* x1.256 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 469 723
Exec per second median:    1 483 665
Exec per second max:       1 487 213

== Median report:
Microseconds per exec:          6.740065
Exec per second:                 1483665
Exec per second per thread:       148366
Mutex contention count:             6122
Thread  0: exec:      1020385, sched:       186
Thread  1: exec:       926899, sched:       166
Thread  2: exec:       956463, sched:       178
Thread  3: exec:       985803, sched:       182
Thread  4: exec:      1028302, sched:       185
Thread  5: exec:       990468, sched:       180
Thread  6: exec:      1046349, sched:       185
Thread  7: exec:      1008534, sched:       183
Thread  8: exec:      1034058, sched:       186
Thread  9: exec:      1002739, sched:       180


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 153 418
Exec per second median:    1 181 704
Exec per second max:       1 245 664

== Median report:
Microseconds per exec:          8.462351
Exec per second:                 1181704
Exec per second per thread:       118170
Mutex contention count:          4266640
Thread  0: exec:      1012467, sched:         0
Thread  1: exec:       999722, sched:         0
Thread  2: exec:       992588, sched:         0
Thread  3: exec:       979555, sched:         0
Thread  4: exec:      1006874, sched:         0
Thread  5: exec:       989280, sched:         0
Thread  6: exec:       990767, sched:         0
Thread  7: exec:       997966, sched:         0
Thread  8: exec:      1025377, sched:         0
Thread  9: exec:      1005404, sched:         0
```
</details>
