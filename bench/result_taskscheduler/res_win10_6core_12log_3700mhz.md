**OS**: Windows 10 Enterprise 19042<br>
**CPU**: Intel(R) Xeon(R) W-2135 CPU @ 3.70GHz, 3696 Mhz, 6 Core(s), 12 Logical Processor(s)

---
**Nano load, 1 thread, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 1 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 12 887 807 tasks per second;
* x2.762 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      12 578 461
Exec per second median:   12 887 807
Exec per second max:      13 024 703

== Median report:
Microseconds per exec:          0.077593
Exec per second:                12887807
Mutex contention count:                0
Thread  0: exec:     10000000, sched:      2002


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       3 816 300
Exec per second median:    4 666 682
Exec per second max:       4 960 115

== Median report:
Microseconds per exec:          0.214285
Exec per second:                 4666682
Mutex contention count:          2695606
Thread  0: exec:     10000000, sched:         0
```
</details>

---
**Nano load, 1 thread, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 1 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 15 376 762 tasks per second;
* x1.015 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:      14 639 328
Exec per second median:   15 376 762
Exec per second max:      15 545 800

== Median report:
Microseconds per exec:          0.065033
Exec per second:                15376762
Mutex contention count:                0
Thread  0: exec:     50000000, sched:     10003


#### Trivial task scheduler
== Aggregated report:
Exec per second min:      14 310 742
Exec per second median:   15 142 185
Exec per second max:      16 294 672

== Median report:
Microseconds per exec:          0.066041
Exec per second:                15142185
Mutex contention count:          1956529
Thread  0: exec:     50000000, sched:         0
```
</details>

---
**Nano load, 2 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 2 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 5 780 075 tasks per second;
* x3.098 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       4 759 210
Exec per second median:    5 780 075
Exec per second max:       6 400 117

== Median report:
Microseconds per exec:          0.346016
Exec per second:                 5780075
Exec per second per thread:      2890037
Mutex contention count:              362
Thread  0: exec:      5034554, sched:       519
Thread  1: exec:      4965446, sched:       496


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 782 573
Exec per second median:    1 865 780
Exec per second max:       2 352 590

== Median report:
Microseconds per exec:          1.071937
Exec per second:                 1865780
Exec per second per thread:       932890
Mutex contention count:          9879664
Thread  0: exec:      5171067, sched:         0
Thread  1: exec:      4828933, sched:         0
```
</details>

---
**Nano load, 2 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 2 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 5 862 463 tasks per second;
* x1.875 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       5 671 524
Exec per second median:    5 862 463
Exec per second max:       5 998 227

== Median report:
Microseconds per exec:          0.341154
Exec per second:                 5862463
Exec per second per thread:      2931231
Mutex contention count:             1969
Thread  0: exec:     25311775, sched:      2525
Thread  1: exec:     24688225, sched:      2512


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       2 867 593
Exec per second median:    3 127 072
Exec per second max:       3 488 219

== Median report:
Microseconds per exec:          0.639576
Exec per second:                 3127072
Exec per second per thread:      1563536
Mutex contention count:         23507882
Thread  0: exec:     20320483, sched:         0
Thread  1: exec:     29679517, sched:         0
```
</details>

---
**Nano load, 3 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 3 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 3 865 002 tasks per second;
* x3.019 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 770 006
Exec per second median:    3 865 002
Exec per second max:       3 959 621

== Median report:
Microseconds per exec:          0.776196
Exec per second:                 3865002
Exec per second per thread:      1288334
Mutex contention count:             2625
Thread  0: exec:      3311920, sched:       513
Thread  1: exec:      3337309, sched:       520
Thread  2: exec:      3350771, sched:       515


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 143 239
Exec per second median:    1 280 213
Exec per second max:       1 679 002

== Median report:
Microseconds per exec:          2.343359
Exec per second:                 1280213
Exec per second per thread:       426737
Mutex contention count:         12188386
Thread  0: exec:      3180536, sched:         0
Thread  1: exec:      3439507, sched:         0
Thread  2: exec:      3379957, sched:         0
```
</details>

---
**Nano load, 3 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 3 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 3 720 227 tasks per second;
* x1.777 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 702 312
Exec per second median:    3 720 227
Exec per second max:       3 753 323

== Median report:
Microseconds per exec:          0.806402
Exec per second:                 3720227
Exec per second per thread:      1240075
Mutex contention count:            13912
Thread  0: exec:     16822271, sched:      1925
Thread  1: exec:     16646964, sched:      1878
Thread  2: exec:     16530765, sched:      1893


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 738 759
Exec per second median:    2 093 628
Exec per second max:       2 178 687

== Median report:
Microseconds per exec:          1.432919
Exec per second:                 2093628
Exec per second per thread:       697876
Mutex contention count:         26896788
Thread  0: exec:     17139328, sched:         0
Thread  1: exec:     16009047, sched:         0
Thread  2: exec:     16851625, sched:         0
```
</details>

---
**Nano load, 5 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 5 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 3 954 902 tasks per second;
* x4.092 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 682 985
Exec per second median:    3 954 902
Exec per second max:       4 214 412

== Median report:
Microseconds per exec:          1.264254
Exec per second:                 3954902
Exec per second per thread:       790980
Mutex contention count:             5976
Thread  0: exec:      1961471, sched:       289
Thread  1: exec:      2030655, sched:       304
Thread  2: exec:      2021416, sched:       311
Thread  3: exec:      2048713, sched:       297
Thread  4: exec:      1937745, sched:       279


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         751 424
Exec per second median:      966 416
Exec per second max:       1 143 910

== Median report:
Microseconds per exec:          5.173752
Exec per second:                  966416
Exec per second per thread:       193283
Mutex contention count:          9561752
Thread  0: exec:      1924892, sched:         0
Thread  1: exec:      2036204, sched:         0
Thread  2: exec:      1996259, sched:         0
Thread  3: exec:      2096640, sched:         0
Thread  4: exec:      1946005, sched:         0
```
</details>

---
**Nano load, 5 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 5 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 4 176 492 tasks per second;
* x2.675 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 918 095
Exec per second median:    4 176 492
Exec per second max:       4 306 252

== Median report:
Microseconds per exec:          1.197177
Exec per second:                 4176492
Exec per second per thread:       835298
Mutex contention count:            29135
Thread  0: exec:     10000978, sched:      1178
Thread  1: exec:      9992850, sched:      1157
Thread  2: exec:     10029651, sched:      1223
Thread  3: exec:      9930614, sched:      1192
Thread  4: exec:     10045907, sched:      1202


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 480 932
Exec per second median:    1 561 103
Exec per second max:       1 610 176

== Median report:
Microseconds per exec:          3.202862
Exec per second:                 1561103
Exec per second per thread:       312220
Mutex contention count:         49461485
Thread  0: exec:      9954666, sched:         0
Thread  1: exec:      9717649, sched:         0
Thread  2: exec:      9750779, sched:         0
Thread  3: exec:     10132492, sched:         0
Thread  4: exec:     10444414, sched:         0
```
</details>

---
**Nano load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load nano -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 3 795 896 tasks per second;
* x5.559 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 421 087
Exec per second median:    3 795 896
Exec per second max:       4 141 004

== Median report:
Microseconds per exec:          2.634424
Exec per second:                 3795896
Exec per second per thread:       379589
Mutex contention count:            14907
Thread  0: exec:       965405, sched:       153
Thread  1: exec:      1035933, sched:       158
Thread  2: exec:      1000434, sched:       150
Thread  3: exec:      1004392, sched:       149
Thread  4: exec:      1029581, sched:       159
Thread  5: exec:       973690, sched:       147
Thread  6: exec:      1001538, sched:       154
Thread  7: exec:      1038531, sched:       160
Thread  8: exec:       981073, sched:       154
Thread  9: exec:       969423, sched:       148


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         574 728
Exec per second median:      682 888
Exec per second max:         693 617

== Median report:
Microseconds per exec:         14.643677
Exec per second:                  682888
Exec per second per thread:        68288
Mutex contention count:          9546306
Thread  0: exec:       982869, sched:         0
Thread  1: exec:       994019, sched:         0
Thread  2: exec:       998046, sched:         0
Thread  3: exec:       985272, sched:         0
Thread  4: exec:      1030215, sched:         0
Thread  5: exec:       995291, sched:         0
Thread  6: exec:       981945, sched:         0
Thread  7: exec:       987975, sched:         0
Thread  8: exec:       992332, sched:         0
Thread  9: exec:      1052036, sched:         0
```
</details>

---
**Nano load, 10 threads, 5 000 000 tasks, each is executed 10 times**

```
Command line args: -load nano -threads 10 -tasks 5000000 -exes 10
Run count: 5
Summary:
* 3 910 435 tasks per second;
* x3.291 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       3 532 458
Exec per second median:    3 910 435
Exec per second max:       4 094 800

== Median report:
Microseconds per exec:          2.557260
Exec per second:                 3910435
Exec per second per thread:       391043
Mutex contention count:            76310
Thread  0: exec:      5009690, sched:       742
Thread  1: exec:      5051534, sched:       730
Thread  2: exec:      5035068, sched:       727
Thread  3: exec:      4912197, sched:       699
Thread  4: exec:      4988745, sched:       715
Thread  5: exec:      4978014, sched:       681
Thread  6: exec:      4964698, sched:       730
Thread  7: exec:      5083768, sched:       766
Thread  8: exec:      5031805, sched:       719
Thread  9: exec:      4944481, sched:       711


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 000 706
Exec per second median:    1 188 254
Exec per second max:       1 213 337

== Median report:
Microseconds per exec:          8.415706
Exec per second:                 1188254
Exec per second per thread:       118825
Mutex contention count:         55813783
Thread  0: exec:      4860388, sched:         0
Thread  1: exec:      4856660, sched:         0
Thread  2: exec:      4873727, sched:         0
Thread  3: exec:      5305009, sched:         0
Thread  4: exec:      5278375, sched:         0
Thread  5: exec:      4937534, sched:         0
Thread  6: exec:      4845767, sched:         0
Thread  7: exec:      4847190, sched:         0
Thread  8: exec:      4890206, sched:         0
Thread  9: exec:      5305144, sched:         0
```
</details>

---
**Micro load, 1 thread, 1 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 1 -tasks 1000000 -exes 1
Run count: 5
Summary:
* 662 744 tasks per second;
* almost same as trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         404 719
Exec per second median:      662 744
Exec per second max:         664 076

== Median report:
Microseconds per exec:          1.508877
Exec per second:                  662744
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         659 463
Exec per second median:      660 210
Exec per second max:         676 068

== Median report:
Microseconds per exec:          1.514668
Exec per second:                  660210
Mutex contention count:            15790
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 1 thread, 100 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 1 -tasks 100000 -exes 10
Run count: 5
Summary:
* 633 068 tasks per second;
* x0.961 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         619 390
Exec per second median:      633 068
Exec per second max:         660 994

== Median report:
Microseconds per exec:          1.579607
Exec per second:                  633068
Mutex contention count:                0
Thread  0: exec:      1000000, sched:       201


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         641 405
Exec per second median:      658 637
Exec per second max:         671 938

== Median report:
Microseconds per exec:          1.518287
Exec per second:                  658637
Mutex contention count:             2143
Thread  0: exec:      1000000, sched:         0
```
</details>

---
**Micro load, 2 threads, 2 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 2 -tasks 2000000 -exes 1
Run count: 5
Summary:
* 463 111 tasks per second;
* x1.022 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         450 965
Exec per second median:      463 111
Exec per second max:       1 033 477

== Median report:
Microseconds per exec:          4.318612
Exec per second:                  463111
Exec per second per thread:       231555
Mutex contention count:               22
Thread  0: exec:       947664, sched:       190
Thread  1: exec:      1052336, sched:       209


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         438 670
Exec per second median:      452 990
Exec per second max:         463 122

== Median report:
Microseconds per exec:          4.415107
Exec per second:                  452990
Exec per second per thread:       226495
Mutex contention count:            58340
Thread  0: exec:       956068, sched:         0
Thread  1: exec:      1043932, sched:         0
```
</details>

---
**Micro load, 2 threads, 200 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 2 -tasks 200000 -exes 10
Run count: 5
Summary:
* 1 024 749 tasks per second;
* x0.965 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         461 780
Exec per second median:    1 024 749
Exec per second max:       1 079 894

== Median report:
Microseconds per exec:          1.951696
Exec per second:                 1024749
Exec per second per thread:       512374
Mutex contention count:               33
Thread  0: exec:       995775, sched:       196
Thread  1: exec:      1004225, sched:       197


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         443 357
Exec per second median:    1 062 301
Exec per second max:       1 090 630

== Median report:
Microseconds per exec:          1.882705
Exec per second:                 1062301
Exec per second per thread:       531150
Mutex contention count:           121417
Thread  0: exec:      1017194, sched:         0
Thread  1: exec:       982806, sched:         0
```
</details>

---
**Micro load, 3 threads, 3 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 3 -tasks 3000000 -exes 1
Run count: 5
Summary:
* 1 009 567 tasks per second;
* almost same as trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         656 254
Exec per second median:    1 009 567
Exec per second max:       1 462 339

== Median report:
Microseconds per exec:          2.971569
Exec per second:                 1009567
Exec per second per thread:       336522
Mutex contention count:              179
Thread  0: exec:       706619, sched:       140
Thread  1: exec:       706742, sched:       141
Thread  2: exec:      1586639, sched:       311


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         703 392
Exec per second median:    1 002 818
Exec per second max:       1 031 024

== Median report:
Microseconds per exec:          2.991569
Exec per second:                 1002818
Exec per second per thread:       334272
Mutex contention count:           233034
Thread  0: exec:       705192, sched:         0
Thread  1: exec:       656666, sched:         0
Thread  2: exec:      1638142, sched:         0
```
</details>

---
**Micro load, 3 threads, 300 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 3 -tasks 300000 -exes 10
Run count: 5
Summary:
* 1 001 028 tasks per second;
* x1.444 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:         985 537
Exec per second median:    1 001 028
Exec per second max:       1 461 232

== Median report:
Microseconds per exec:          2.996917
Exec per second:                 1001028
Exec per second per thread:       333676
Mutex contention count:              141
Thread  0: exec:      1605275, sched:       317
Thread  1: exec:       718429, sched:       139
Thread  2: exec:       676296, sched:       133


#### Trivial task scheduler
== Aggregated report:
Exec per second min:         666 984
Exec per second median:      693 112
Exec per second max:       1 007 301

== Median report:
Microseconds per exec:          4.328302
Exec per second:                  693112
Exec per second per thread:       231037
Mutex contention count:           227179
Thread  0: exec:      1191883, sched:         0
Thread  1: exec:      1046705, sched:         0
Thread  2: exec:       761412, sched:         0
```
</details>

---
**Micro load, 5 threads, 5 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 5 -tasks 5000000 -exes 1
Run count: 5
Summary:
* 1 485 021 tasks per second;
* x0.854 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 133 885
Exec per second median:    1 485 021
Exec per second max:       1 790 423

== Median report:
Microseconds per exec:          3.366954
Exec per second:                 1485021
Exec per second per thread:       297004
Mutex contention count:              604
Thread  0: exec:      1433015, sched:       277
Thread  1: exec:       581104, sched:       112
Thread  2: exec:      1440639, sched:       279
Thread  3: exec:       814787, sched:       153
Thread  4: exec:       730455, sched:       137


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 270 560
Exec per second median:    1 738 852
Exec per second max:       1 771 838

== Median report:
Microseconds per exec:          2.875459
Exec per second:                 1738852
Exec per second per thread:       347770
Mutex contention count:          1248095
Thread  0: exec:      1287401, sched:         0
Thread  1: exec:      1250796, sched:         0
Thread  2: exec:       633623, sched:         0
Thread  3: exec:       596998, sched:         0
Thread  4: exec:      1231182, sched:         0
```
</details>

---
**Micro load, 5 threads, 500 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 5 -tasks 500000 -exes 10
Run count: 5
Summary:
* 1 484 860 tasks per second;
* x1.122 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 125 358
Exec per second median:    1 484 860
Exec per second max:       1 771 413

== Median report:
Microseconds per exec:          3.367319
Exec per second:                 1484860
Exec per second per thread:       296972
Mutex contention count:              911
Thread  0: exec:       809301, sched:       146
Thread  1: exec:       872364, sched:       165
Thread  2: exec:      1582678, sched:       295
Thread  3: exec:       834745, sched:       160
Thread  4: exec:       900912, sched:       171


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 078 670
Exec per second median:    1 323 912
Exec per second max:       1 968 794

== Median report:
Microseconds per exec:          3.776686
Exec per second:                 1323912
Exec per second per thread:       264782
Mutex contention count:           905090
Thread  0: exec:       958482, sched:         0
Thread  1: exec:       948162, sched:         0
Thread  2: exec:       639581, sched:         0
Thread  3: exec:      1796824, sched:         0
Thread  4: exec:       656951, sched:         0
```
</details>

---
**Micro load, 10 threads, 10 000 000 tasks, each is executed 1 time**

```
Command line args: -load micro -threads 10 -tasks 10000000 -exes 1
Run count: 5
Summary:
* 2 178 051 tasks per second;
* x1.227 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       1 990 144
Exec per second median:    2 178 051
Exec per second max:       2 357 743

== Median report:
Microseconds per exec:          4.591260
Exec per second:                 2178051
Exec per second per thread:       217805
Mutex contention count:             7703
Thread  0: exec:       988413, sched:       183
Thread  1: exec:      1518299, sched:       279
Thread  2: exec:       938639, sched:       171
Thread  3: exec:       940937, sched:       168
Thread  4: exec:       992706, sched:       178
Thread  5: exec:       950360, sched:       175
Thread  6: exec:       983418, sched:       176
Thread  7: exec:       929666, sched:       173
Thread  8: exec:       762481, sched:       143
Thread  9: exec:       995081, sched:       183


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 144 186
Exec per second median:    1 775 711
Exec per second max:       1 991 122

== Median report:
Microseconds per exec:          5.631545
Exec per second:                 1775711
Exec per second per thread:       177571
Mutex contention count:          9383440
Thread  0: exec:      1048478, sched:         0
Thread  1: exec:       764334, sched:         0
Thread  2: exec:      1038204, sched:         0
Thread  3: exec:      1083279, sched:         0
Thread  4: exec:      1071151, sched:         0
Thread  5: exec:      1030289, sched:         0
Thread  6: exec:      1042234, sched:         0
Thread  7: exec:       782963, sched:         0
Thread  8: exec:      1051460, sched:         0
Thread  9: exec:      1087608, sched:         0
```
</details>

---
**Micro load, 10 threads, 1 000 000 tasks, each is executed 10 times**

```
Command line args: -load micro -threads 10 -tasks 1000000 -exes 10
Run count: 5
Summary:
* 2 295 936 tasks per second;
* x1.394 of trivial scheduler;
```

<details><summary>Details</summary>

```
#### Canon task scheduler
== Aggregated report:
Exec per second min:       2 132 325
Exec per second median:    2 295 936
Exec per second max:       2 376 987

== Median report:
Microseconds per exec:          4.355521
Exec per second:                 2295936
Exec per second per thread:       229593
Mutex contention count:             8032
Thread  0: exec:       938416, sched:       166
Thread  1: exec:       916025, sched:       164
Thread  2: exec:       946711, sched:       171
Thread  3: exec:       903010, sched:       150
Thread  4: exec:       896154, sched:       157
Thread  5: exec:       723998, sched:       135
Thread  6: exec:       746101, sched:       134
Thread  7: exec:      1493826, sched:       273
Thread  8: exec:       937942, sched:       173
Thread  9: exec:      1497817, sched:       265


#### Trivial task scheduler
== Aggregated report:
Exec per second min:       1 058 807
Exec per second median:    1 647 369
Exec per second max:       1 973 831

== Median report:
Microseconds per exec:          6.070282
Exec per second:                 1647369
Exec per second per thread:       164736
Mutex contention count:          8080260
Thread  0: exec:      1048093, sched:         0
Thread  1: exec:      1024388, sched:         0
Thread  2: exec:      1030377, sched:         0
Thread  3: exec:       808444, sched:         0
Thread  4: exec:       824841, sched:         0
Thread  5: exec:      1064290, sched:         0
Thread  6: exec:      1066143, sched:         0
Thread  7: exec:      1011418, sched:         0
Thread  8: exec:      1035793, sched:         0
Thread  9: exec:      1086213, sched:         0
```
</details>
