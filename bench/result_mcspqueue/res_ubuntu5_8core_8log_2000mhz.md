**OS**: Ubuntu 5.4.0-42-generic<br>
**CPU**: Intel Core Processor (Haswell, no TSX, IBRS) 2.0GHz, 8 cores, 1 thread per core

---
**Operation push, 100 000 000 items**

```
Command line args: -op push -items 100000000
Run count: 5
Summary:
* 37 399 121 pops per second;
* x1.321 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      28 213 003
Items per sec median:   37 399 121
Items per sec max:      37 609 030

== Median report:
Items per sec:                37399121
Mutex contention count:              0


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      28 234 008
Items per sec median:   28 303 196
Items per sec max:      28 441 695

== Median report:
Items per sec:                28303196
Mutex contention count:              0
```
</details>

---
**Operation push-pop, 1 thread, 10 000 000 items**

```
Command line args: -op push-pop -load empty -threads 1 -items 10000000
Run count: 5
Summary:
* 4 436 585 pops per second;
* x0.939 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       4 178 861
Items per sec median:    4 436 585
Items per sec max:       4 450 931

== Median report:
Items per sec:                 4436585
Mutex contention count:           1497
Thread  0: pop:     10000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       3 277 171
Items per sec median:    4 722 339
Items per sec max:       4 957 217

== Median report:
Items per sec:                 4722339
Mutex contention count:        2416183
Thread  0: pop:     10000000
```
</details>

---
**Operation push-pop, 1 thread, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 1 -items 1000000
Run count: 5
Summary:
* 346 643 pops per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         342 161
Items per sec median:      346 643
Items per sec max:         347 984

== Median report:
Items per sec:                  346643
Mutex contention count:              1
Thread  0: pop:      1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         340 559
Items per sec median:      343 455
Items per sec max:         344 961

== Median report:
Items per sec:                  343455
Mutex contention count:          18623
Thread  0: pop:      1000000
```
</details>

---
**Operation push-pop, 2 threads, 10 000 000 items**

```
Command line args: -op push-pop -load empty -threads 2 -items 10000000
Run count: 5
Summary:
* 3 512 461 pops per second;
* x0.931 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 388 682
Items per sec median:    3 512 461
Items per sec max:       3 553 776

== Median report:
Items per sec:                 3512461
Items per sec per thread:      1756230
Mutex contention count:           4444
Thread  0: pop:      5025507
Thread  1: pop:      4974493


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       3 315 443
Items per sec median:    3 774 174
Items per sec max:       3 975 927

== Median report:
Items per sec:                 3774174
Items per sec per thread:      1887087
Mutex contention count:        3284691
Thread  0: pop:      4965497
Thread  1: pop:      5034503
```
</details>

---
**Operation push-pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 676 225 pops per second;
* x1.028 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         673 315
Items per sec median:      676 225
Items per sec max:         677 325

== Median report:
Items per sec:                  676225
Items per sec per thread:       338112
Mutex contention count:           1247
Thread  0: pop:       499076
Thread  1: pop:       500924


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         657 026
Items per sec median:      657 751
Items per sec max:         668 282

== Median report:
Items per sec:                  657751
Items per sec per thread:       328875
Mutex contention count:          52230
Thread  0: pop:       508416
Thread  1: pop:       491584
```
</details>

---
**Operation push-pop, 3 threads, 5 000 000 items**

```
Command line args: -op push-pop -load empty -threads 3 -items 5000000
Run count: 5
Summary:
* 2 963 327 pops per second;
* x1.263 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 916 901
Items per sec median:    2 963 327
Items per sec max:       2 986 214

== Median report:
Items per sec:                 2963327
Items per sec per thread:       987775
Mutex contention count:           3476
Thread  0: pop:      1678309
Thread  1: pop:      1660769
Thread  2: pop:      1660922


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 113 803
Items per sec median:    2 346 792
Items per sec max:       2 464 123

== Median report:
Items per sec:                 2346792
Items per sec per thread:       782264
Mutex contention count:        2371222
Thread  0: pop:      1575730
Thread  1: pop:      1821180
Thread  2: pop:      1603090
```
</details>

---
**Operation push-pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 008 802 pops per second;
* x1.027 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 000 158
Items per sec median:    1 008 802
Items per sec max:       1 017 091

== Median report:
Items per sec:                 1008802
Items per sec per thread:       336267
Mutex contention count:            608
Thread  0: pop:       997276
Thread  1: pop:      1002343
Thread  2: pop:      1000381


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         979 989
Items per sec median:      982 203
Items per sec max:         987 127

== Median report:
Items per sec:                  982203
Items per sec per thread:       327401
Mutex contention count:         231173
Thread  0: pop:      1000832
Thread  1: pop:      1007360
Thread  2: pop:       991808
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items**

```
Command line args: -op push-pop -load empty -threads 5 -items 5000000
Run count: 5
Summary:
* 2 244 334 pops per second;
* x2.027 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 219 433
Items per sec median:    2 244 334
Items per sec max:       2 996 240

== Median report:
Items per sec:                 2244334
Items per sec per thread:       448866
Mutex contention count:           6504
Thread  0: pop:       982519
Thread  1: pop:      1018964
Thread  2: pop:       968394
Thread  3: pop:      1034980
Thread  4: pop:       995143


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 022 116
Items per sec median:    1 107 259
Items per sec max:       1 317 661

== Median report:
Items per sec:                 1107259
Items per sec per thread:       221451
Mutex contention count:        5518594
Thread  0: pop:       977035
Thread  1: pop:      1022948
Thread  2: pop:       969346
Thread  3: pop:      1042951
Thread  4: pop:       987720
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 1 661 292 pops per second;
* x1.061 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 638 981
Items per sec median:    1 661 292
Items per sec max:       1 675 983

== Median report:
Items per sec:                 1661292
Items per sec per thread:       332258
Mutex contention count:           1345
Thread  0: pop:       998966
Thread  1: pop:       972670
Thread  2: pop:      1006717
Thread  3: pop:      1012509
Thread  4: pop:      1009138


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 512 236
Items per sec median:    1 566 387
Items per sec max:       1 576 914

== Median report:
Items per sec:                 1566387
Items per sec per thread:       313277
Mutex contention count:         856704
Thread  0: pop:       987882
Thread  1: pop:      1001208
Thread  2: pop:      1000025
Thread  3: pop:      1015519
Thread  4: pop:       995366
```
</details>

---
**Operation push-pop, 10 threads, 2 000 000 items**

```
Command line args: -op push-pop -load empty -threads 10 -items 2000000
Run count: 5
Summary:
* 1 200 716 pops per second;
* x2.409 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         899 935
Items per sec median:    1 200 716
Items per sec max:       1 475 047

== Median report:
Items per sec:                 1200716
Items per sec per thread:       120071
Mutex contention count:           4222
Thread  0: pop:       259698
Thread  1: pop:       152562
Thread  2: pop:       262129
Thread  3: pop:       183812
Thread  4: pop:       171403
Thread  5: pop:       166621
Thread  6: pop:       199290
Thread  7: pop:       127472
Thread  8: pop:       246174
Thread  9: pop:       230839


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         489 123
Items per sec median:      498 528
Items per sec max:         510 748

== Median report:
Items per sec:                  498528
Items per sec per thread:        49852
Mutex contention count:        4564207
Thread  0: pop:       198584
Thread  1: pop:       197796
Thread  2: pop:       195904
Thread  3: pop:       202193
Thread  4: pop:       197770
Thread  5: pop:       196374
Thread  6: pop:       204100
Thread  7: pop:       192960
Thread  8: pop:       210930
Thread  9: pop:       203389
```
</details>

---
**Operation push-pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 2 615 607 pops per second;
* x1.310 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 502 282
Items per sec median:    2 615 607
Items per sec max:       2 624 642

== Median report:
Items per sec:                 2615607
Items per sec per thread:       261560
Mutex contention count:           3171
Thread  0: pop:       583623
Thread  1: pop:       489207
Thread  2: pop:       414597
Thread  3: pop:       554092
Thread  4: pop:       499463
Thread  5: pop:       548372
Thread  6: pop:       590565
Thread  7: pop:       486211
Thread  8: pop:       341256
Thread  9: pop:       492614


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 717 407
Items per sec median:    1 996 613
Items per sec max:       2 233 380

== Median report:
Items per sec:                 1996613
Items per sec per thread:       199661
Mutex contention count:        1834708
Thread  0: pop:       505059
Thread  1: pop:       480017
Thread  2: pop:       484688
Thread  3: pop:       489259
Thread  4: pop:       479537
Thread  5: pop:       519007
Thread  6: pop:       519481
Thread  7: pop:       517381
Thread  8: pop:       519495
Thread  9: pop:       486076
```
</details>

---
**Operation pop, 1 thread, 50 000 000 items**

```
Command line args: -op pop -load empty -threads 1 -items 50000000
Run count: 5
Summary:
* 34 860 285 pops per second;
* x1.425 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      34 434 125
Items per sec median:   34 860 285
Items per sec max:      35 060 097

== Median report:
Items per sec:                34860285
Mutex contention count:              0
Thread  0: pop:     50000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      24 211 481
Items per sec median:   24 464 102
Items per sec max:      24 789 851

== Median report:
Items per sec:                24464102
Mutex contention count:              0
Thread  0: pop:     50000000
```
</details>

---
**Operation pop, 1 thread, 1 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 1 -items 1000000
Run count: 5
Summary:
* 348 050 pops per second;
* x1.010 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         343 793
Items per sec median:      348 050
Items per sec max:         349 079

== Median report:
Items per sec:                  348050
Mutex contention count:              0
Thread  0: pop:      1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         342 722
Items per sec median:      344 437
Items per sec max:         346 269

== Median report:
Items per sec:                  344437
Mutex contention count:              0
Thread  0: pop:      1000000
```
</details>

---
**Operation pop, 2 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 2 -items 10000000
Run count: 5
Summary:
* 10 379 659 pops per second;
* x1.418 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       9 088 837
Items per sec median:   10 379 659
Items per sec max:      11 777 250

== Median report:
Items per sec:                10379659
Items per sec per thread:      5189829
Mutex contention count:           2962
Thread  0: pop:      5003573
Thread  1: pop:      4996427


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       6 833 513
Items per sec median:    7 319 837
Items per sec max:       7 814 742

== Median report:
Items per sec:                 7319837
Items per sec per thread:      3659918
Mutex contention count:        1509349
Thread  0: pop:      4951732
Thread  1: pop:      5048268
```
</details>

---
**Operation pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 677 641 pops per second;
* x1.015 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         666 058
Items per sec median:      677 641
Items per sec max:         680 061

== Median report:
Items per sec:                  677641
Items per sec per thread:       338820
Mutex contention count:            686
Thread  0: pop:       501546
Thread  1: pop:       498454


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         656 580
Items per sec median:      667 779
Items per sec max:         680 889

== Median report:
Items per sec:                  667779
Items per sec per thread:       333889
Mutex contention count:          11785
Thread  0: pop:       500209
Thread  1: pop:       499791
```
</details>

---
**Operation pop, 3 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 3 -items 10000000
Run count: 5
Summary:
* 8 943 159 pops per second;
* x0.914 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       8 807 525
Items per sec median:    8 943 159
Items per sec max:       9 576 120

== Median report:
Items per sec:                 8943159
Items per sec per thread:      2981053
Mutex contention count:           3870
Thread  0: pop:      3242865
Thread  1: pop:      3532007
Thread  2: pop:      3225128


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       9 228 849
Items per sec median:    9 783 505
Items per sec max:      10 241 599

== Median report:
Items per sec:                 9783505
Items per sec per thread:      3261168
Mutex contention count:        1340039
Thread  0: pop:      3785100
Thread  1: pop:      3140916
Thread  2: pop:      3073984
```
</details>

---
**Operation pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 000 912 pops per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         994 451
Items per sec median:    1 000 912
Items per sec max:       1 010 018

== Median report:
Items per sec:                 1000912
Items per sec per thread:       333637
Mutex contention count:            219
Thread  0: pop:       984258
Thread  1: pop:      1014406
Thread  2: pop:      1001336


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         970 679
Items per sec median:      994 414
Items per sec max:       1 007 012

== Median report:
Items per sec:                  994414
Items per sec per thread:       331471
Mutex contention count:          46568
Thread  0: pop:       989450
Thread  1: pop:       999205
Thread  2: pop:      1011345
```
</details>

---
**Operation pop, 5 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 5 -items 10000000
Run count: 5
Summary:
* 7 740 226 pops per second;
* x1.286 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       6 135 363
Items per sec median:    7 740 226
Items per sec max:       8 038 499

== Median report:
Items per sec:                 7740226
Items per sec per thread:      1548045
Mutex contention count:           7967
Thread  0: pop:      2074972
Thread  1: pop:      1861030
Thread  2: pop:      2029451
Thread  3: pop:      1956987
Thread  4: pop:      2077560


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       5 654 584
Items per sec median:    6 019 405
Items per sec max:       7 251 139

== Median report:
Items per sec:                 6019405
Items per sec per thread:      1203881
Mutex contention count:        2118710
Thread  0: pop:      2213994
Thread  1: pop:      1923644
Thread  2: pop:      1900500
Thread  3: pop:      1948188
Thread  4: pop:      2013674
```
</details>

---
**Operation pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 1 682 168 pops per second;
* x1.024 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 663 344
Items per sec median:    1 682 168
Items per sec max:       1 683 284

== Median report:
Items per sec:                 1682168
Items per sec per thread:       336433
Mutex contention count:           1944
Thread  0: pop:      1007998
Thread  1: pop:       994527
Thread  2: pop:       998755
Thread  3: pop:       992443
Thread  4: pop:      1006277


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 585 132
Items per sec median:    1 642 088
Items per sec max:       1 664 501

== Median report:
Items per sec:                 1642088
Items per sec per thread:       328417
Mutex contention count:         239504
Thread  0: pop:      1002570
Thread  1: pop:      1007750
Thread  2: pop:      1000752
Thread  3: pop:      1004955
Thread  4: pop:       983973
```
</details>

---
**Operation pop, 10 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 10 -items 10000000
Run count: 5
Summary:
* 6 592 012 pops per second;
* x1.194 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       6 145 025
Items per sec median:    6 592 012
Items per sec max:       6 611 450

== Median report:
Items per sec:                 6592012
Items per sec per thread:       659201
Mutex contention count:          13997
Thread  0: pop:      1003880
Thread  1: pop:      1048599
Thread  2: pop:      1000879
Thread  3: pop:      1100434
Thread  4: pop:      1014526
Thread  5: pop:       978654
Thread  6: pop:       807980
Thread  7: pop:      1062586
Thread  8: pop:      1025341
Thread  9: pop:       957121


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       4 951 768
Items per sec median:    5 521 919
Items per sec max:       5 575 410

== Median report:
Items per sec:                 5521919
Items per sec per thread:       552191
Mutex contention count:        2178102
Thread  0: pop:       967913
Thread  1: pop:      1051936
Thread  2: pop:       983984
Thread  3: pop:      1010591
Thread  4: pop:      1017731
Thread  5: pop:       990911
Thread  6: pop:       972107
Thread  7: pop:      1005671
Thread  8: pop:      1000213
Thread  9: pop:       998943
```
</details>

---
**Operation pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 2 623 914 pops per second;
* x1.079 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 585 709
Items per sec median:    2 623 914
Items per sec max:       2 680 123

== Median report:
Items per sec:                 2623914
Items per sec per thread:       262391
Mutex contention count:           2916
Thread  0: pop:       590842
Thread  1: pop:       485267
Thread  2: pop:       505986
Thread  3: pop:       386043
Thread  4: pop:       535275
Thread  5: pop:       403731
Thread  6: pop:       559262
Thread  7: pop:       583417
Thread  8: pop:       441432
Thread  9: pop:       508745


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 277 033
Items per sec median:    2 430 776
Items per sec max:       2 567 134

== Median report:
Items per sec:                 2430776
Items per sec per thread:       243077
Mutex contention count:         604764
Thread  0: pop:       502599
Thread  1: pop:       519619
Thread  2: pop:       469851
Thread  3: pop:       495810
Thread  4: pop:       517356
Thread  5: pop:       483739
Thread  6: pop:       480575
Thread  7: pop:       499541
Thread  8: pop:       516489
Thread  9: pop:       514421
```
</details>
