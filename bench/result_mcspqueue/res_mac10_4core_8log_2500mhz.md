**OS**: macOS Catalina 10.15.5<br>
**CPU**: Quad-Core Intel Core i7 2.5 GHz, 4 cores, hyper-threading enabled

---
**Operation push, 100 000 000 items**

```
Command line args: -op push -items 100000000
Run count: 5
Summary:
* 51 125 039 pops per second;
* x2.217 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      50 069 397
Items per sec median:   51 125 039
Items per sec max:      54 309 057

== Median report:
Items per sec:                51125039
Mutex contention count:              0


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      22 322 553
Items per sec median:   23 059 979
Items per sec max:      24 555 333

== Median report:
Items per sec:                23059979
Mutex contention count:              0
```
</details>

---
**Operation push-pop, 1 thread, 10 000 000 items**

```
Command line args: -op push-pop -load empty -threads 1 -items 10000000
Run count: 5
Summary:
* 8 167 148 pops per second;
* x3.585 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       7 742 839
Items per sec median:    8 167 148
Items per sec max:       8 425 212

== Median report:
Items per sec:                 8167148
Mutex contention count:           1334
Thread  0: pop:     10000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 162 583
Items per sec median:    2 278 123
Items per sec max:       2 537 537

== Median report:
Items per sec:                 2278123
Mutex contention count:          82913
Thread  0: pop:     10000000
```
</details>

---
**Operation push-pop, 1 thread, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 1 -items 1000000
Run count: 5
Summary:
* 464 290 pops per second;
* x1.050 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         455 687
Items per sec median:      464 290
Items per sec max:         468 391

== Median report:
Items per sec:                  464290
Mutex contention count:              2
Thread  0: pop:      1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         436 738
Items per sec median:      442 274
Items per sec max:         445 525

== Median report:
Items per sec:                  442274
Mutex contention count:          10190
Thread  0: pop:      1000000
```
</details>

---
**Operation push-pop, 2 threads, 10 000 000 items**

```
Command line args: -op push-pop -load empty -threads 2 -items 10000000
Run count: 5
Summary:
* 5 904 975 pops per second;
* x2.059 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       5 829 006
Items per sec median:    5 904 975
Items per sec max:       6 036 835

== Median report:
Items per sec:                 5904975
Items per sec per thread:      2952487
Mutex contention count:           3191
Thread  0: pop:      4986528
Thread  1: pop:      5013472


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 741 465
Items per sec median:    2 867 513
Items per sec max:       2 972 532

== Median report:
Items per sec:                 2867513
Items per sec per thread:      1433756
Mutex contention count:        1459702
Thread  0: pop:      5066235
Thread  1: pop:      4933765
```
</details>

---
**Operation push-pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 827 377 pops per second;
* x1.063 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         743 098
Items per sec median:      827 377
Items per sec max:         837 578

== Median report:
Items per sec:                  827377
Items per sec per thread:       413688
Mutex contention count:             23
Thread  0: pop:       500101
Thread  1: pop:       499899


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         764 182
Items per sec median:      778 337
Items per sec max:         800 130

== Median report:
Items per sec:                  778337
Items per sec per thread:       389168
Mutex contention count:          65157
Thread  0: pop:       499655
Thread  1: pop:       500345
```
</details>

---
**Operation push-pop, 3 threads, 5 000 000 items**

```
Command line args: -op push-pop -load empty -threads 3 -items 5000000
Run count: 5
Summary:
* 4 895 439 pops per second;
* x2.486 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       4 724 056
Items per sec median:    4 895 439
Items per sec max:       5 053 464

== Median report:
Items per sec:                 4895439
Items per sec per thread:      1631813
Mutex contention count:           2805
Thread  0: pop:      1684487
Thread  1: pop:      1598490
Thread  2: pop:      1717023


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 965 861
Items per sec median:    1 969 392
Items per sec max:       1 992 046

== Median report:
Items per sec:                 1969392
Items per sec per thread:       656464
Mutex contention count:         928951
Thread  0: pop:      1669439
Thread  1: pop:      1661109
Thread  2: pop:      1669452
```
</details>

---
**Operation push-pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 216 356 pops per second;
* x1.131 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 089 059
Items per sec median:    1 216 356
Items per sec max:       1 252 953

== Median report:
Items per sec:                 1216356
Items per sec per thread:       405452
Mutex contention count:            284
Thread  0: pop:       999100
Thread  1: pop:       999420
Thread  2: pop:      1001480


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 036 996
Items per sec median:    1 075 656
Items per sec max:       1 092 487

== Median report:
Items per sec:                 1075656
Items per sec per thread:       358552
Mutex contention count:         262000
Thread  0: pop:      1000613
Thread  1: pop:       999893
Thread  2: pop:       999494
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items**

```
Command line args: -op push-pop -load empty -threads 5 -items 5000000
Run count: 5
Summary:
* 4 275 783 pops per second;
* x4.516 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       4 227 202
Items per sec median:    4 275 783
Items per sec max:       4 293 388

== Median report:
Items per sec:                 4275783
Items per sec per thread:       855156
Mutex contention count:           5391
Thread  0: pop:       933020
Thread  1: pop:       959114
Thread  2: pop:      1075079
Thread  3: pop:      1018812
Thread  4: pop:      1013975


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         884 202
Items per sec median:      946 737
Items per sec max:         966 171

== Median report:
Items per sec:                  946737
Items per sec per thread:       189347
Mutex contention count:        2052468
Thread  0: pop:       993030
Thread  1: pop:      1029133
Thread  2: pop:       989138
Thread  3: pop:       988825
Thread  4: pop:       999874
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 1 551 954 pops per second;
* x1.351 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 491 412
Items per sec median:    1 551 954
Items per sec max:       1 621 454

== Median report:
Items per sec:                 1551954
Items per sec per thread:       310390
Mutex contention count:           1531
Thread  0: pop:       998045
Thread  1: pop:       991531
Thread  2: pop:      1014063
Thread  3: pop:       994737
Thread  4: pop:      1001624


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 088 034
Items per sec median:    1 148 625
Items per sec max:       1 169 232

== Median report:
Items per sec:                 1148625
Items per sec per thread:       229725
Mutex contention count:         960477
Thread  0: pop:       999027
Thread  1: pop:       993248
Thread  2: pop:      1008225
Thread  3: pop:       999209
Thread  4: pop:      1000291
```
</details>

---
**Operation push-pop, 10 threads, 2 000 000 items**

```
Command line args: -op push-pop -load empty -threads 10 -items 2000000
Run count: 5
Summary:
* 2 202 661 pops per second;
* x4.214 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 016 648
Items per sec median:    2 202 661
Items per sec max:       2 451 744

== Median report:
Items per sec:                 2202661
Items per sec per thread:       220266
Mutex contention count:           4000
Thread  0: pop:       198329
Thread  1: pop:       178094
Thread  2: pop:       173435
Thread  3: pop:       176229
Thread  4: pop:       176997
Thread  5: pop:       218608
Thread  6: pop:       216927
Thread  7: pop:       205038
Thread  8: pop:       208729
Thread  9: pop:       247614


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         507 930
Items per sec median:      522 755
Items per sec max:         542 879

== Median report:
Items per sec:                  522755
Items per sec per thread:        52275
Mutex contention count:        1852131
Thread  0: pop:       197610
Thread  1: pop:       199606
Thread  2: pop:       197971
Thread  3: pop:       199251
Thread  4: pop:       201153
Thread  5: pop:       198116
Thread  6: pop:       201213
Thread  7: pop:       201033
Thread  8: pop:       202782
Thread  9: pop:       201265
```
</details>

---
**Operation push-pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 1 736 321 pops per second;
* x2.134 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 700 941
Items per sec median:    1 736 321
Items per sec max:       1 754 103

== Median report:
Items per sec:                 1736321
Items per sec per thread:       173632
Mutex contention count:           3358
Thread  0: pop:       507105
Thread  1: pop:       507482
Thread  2: pop:       505644
Thread  3: pop:       508612
Thread  4: pop:       483318
Thread  5: pop:       484393
Thread  6: pop:       508351
Thread  7: pop:       503785
Thread  8: pop:       506993
Thread  9: pop:       484317


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         764 849
Items per sec median:      813 671
Items per sec max:         821 539

== Median report:
Items per sec:                  813671
Items per sec per thread:        81367
Mutex contention count:        2303864
Thread  0: pop:       500974
Thread  1: pop:       505065
Thread  2: pop:       500986
Thread  3: pop:       499470
Thread  4: pop:       501008
Thread  5: pop:       485390
Thread  6: pop:       492947
Thread  7: pop:       503166
Thread  8: pop:       506709
Thread  9: pop:       504285
```
</details>

---
**Operation pop, 1 thread, 50 000 000 items**

```
Command line args: -op pop -load empty -threads 1 -items 50000000
Run count: 5
Summary:
* 49 713 695 pops per second;
* x2.450 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      47 077 111
Items per sec median:   49 713 695
Items per sec max:      50 415 563

== Median report:
Items per sec:                49713695
Mutex contention count:              0
Thread  0: pop:     50000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      20 080 605
Items per sec median:   20 290 490
Items per sec max:      20 883 933

== Median report:
Items per sec:                20290490
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
* 469 665 pops per second;
* x1.014 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         466 131
Items per sec median:      469 665
Items per sec max:         471 449

== Median report:
Items per sec:                  469665
Mutex contention count:              0
Thread  0: pop:      1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         460 527
Items per sec median:      463 185
Items per sec max:         465 491

== Median report:
Items per sec:                  463185
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
* 12 677 482 pops per second;
* x1.570 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      12 628 175
Items per sec median:   12 677 482
Items per sec max:      12 756 128

== Median report:
Items per sec:                12677482
Items per sec per thread:      6338741
Mutex contention count:           1842
Thread  0: pop:      4961189
Thread  1: pop:      5038811


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       8 030 158
Items per sec median:    8 074 456
Items per sec max:       8 437 635

== Median report:
Items per sec:                 8074456
Items per sec per thread:      4037228
Mutex contention count:          22764
Thread  0: pop:      4880087
Thread  1: pop:      5119913
```
</details>

---
**Operation pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 914 326 pops per second;
* x1.035 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         911 089
Items per sec median:      914 326
Items per sec max:         916 236

== Median report:
Items per sec:                  914326
Items per sec per thread:       457163
Mutex contention count:             66
Thread  0: pop:       499829
Thread  1: pop:       500171


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         809 071
Items per sec median:      883 138
Items per sec max:         890 393

== Median report:
Items per sec:                  883138
Items per sec per thread:       441569
Mutex contention count:            895
Thread  0: pop:       500040
Thread  1: pop:       499960
```
</details>

---
**Operation pop, 3 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 3 -items 10000000
Run count: 5
Summary:
* 11 261 043 pops per second;
* x1.292 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      11 145 223
Items per sec median:   11 261 043
Items per sec max:      11 781 282

== Median report:
Items per sec:                11261043
Items per sec per thread:      3753681
Mutex contention count:           3907
Thread  0: pop:      3302733
Thread  1: pop:      3341485
Thread  2: pop:      3355782


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       8 507 165
Items per sec median:    8 718 880
Items per sec max:       8 863 955

== Median report:
Items per sec:                 8718880
Items per sec per thread:      2906293
Mutex contention count:         545161
Thread  0: pop:      3466499
Thread  1: pop:      3369518
Thread  2: pop:      3163983
```
</details>

---
**Operation pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 262 643 pops per second;
* x1.037 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 235 766
Items per sec median:    1 262 643
Items per sec max:       1 299 980

== Median report:
Items per sec:                 1262643
Items per sec per thread:       420881
Mutex contention count:            416
Thread  0: pop:      1000860
Thread  1: pop:       999322
Thread  2: pop:       999818


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 202 722
Items per sec median:    1 217 408
Items per sec max:       1 244 157

== Median report:
Items per sec:                 1217408
Items per sec per thread:       405802
Mutex contention count:          22639
Thread  0: pop:      1002051
Thread  1: pop:      1000105
Thread  2: pop:       997844
```
</details>

---
**Operation pop, 5 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 5 -items 10000000
Run count: 5
Summary:
* 8 018 506 pops per second;
* x1.357 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       7 785 627
Items per sec median:    8 018 506
Items per sec max:       8 319 534

== Median report:
Items per sec:                 8018506
Items per sec per thread:      1603701
Mutex contention count:           7475
Thread  0: pop:      2009246
Thread  1: pop:      1976485
Thread  2: pop:      2039948
Thread  3: pop:      2042214
Thread  4: pop:      1932107


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       5 717 613
Items per sec median:    5 906 831
Items per sec max:       6 084 897

== Median report:
Items per sec:                 5906831
Items per sec per thread:      1181366
Mutex contention count:         660172
Thread  0: pop:      2002994
Thread  1: pop:      1999835
Thread  2: pop:      2013174
Thread  3: pop:      1987870
Thread  4: pop:      1996127
```
</details>

---
**Operation pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 1 603 809 pops per second;
* x1.116 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 558 752
Items per sec median:    1 603 809
Items per sec max:       1 624 394

== Median report:
Items per sec:                 1603809
Items per sec per thread:       320761
Mutex contention count:           1569
Thread  0: pop:       998144
Thread  1: pop:      1006048
Thread  2: pop:       994865
Thread  3: pop:       996978
Thread  4: pop:      1003965


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 353 456
Items per sec median:    1 437 702
Items per sec max:       1 508 946

== Median report:
Items per sec:                 1437702
Items per sec per thread:       287540
Mutex contention count:         382296
Thread  0: pop:       994844
Thread  1: pop:      1000924
Thread  2: pop:      1002908
Thread  3: pop:       995491
Thread  4: pop:      1005833
```
</details>

---
**Operation pop, 10 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 10 -items 10000000
Run count: 5
Summary:
* 6 042 456 pops per second;
* x1.051 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       5 928 906
Items per sec median:    6 042 456
Items per sec max:       6 120 133

== Median report:
Items per sec:                 6042456
Items per sec per thread:       604245
Mutex contention count:          12103
Thread  0: pop:       996905
Thread  1: pop:       945819
Thread  2: pop:       983808
Thread  3: pop:      1019106
Thread  4: pop:      1050056
Thread  5: pop:       983736
Thread  6: pop:       977509
Thread  7: pop:       989158
Thread  8: pop:       992019
Thread  9: pop:      1061884


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       5 615 394
Items per sec median:    5 746 523
Items per sec max:       5 857 284

== Median report:
Items per sec:                 5746523
Items per sec per thread:       574652
Mutex contention count:         911903
Thread  0: pop:      1018591
Thread  1: pop:      1005516
Thread  2: pop:       989590
Thread  3: pop:      1006113
Thread  4: pop:       989653
Thread  5: pop:       959961
Thread  6: pop:      1003844
Thread  7: pop:      1002859
Thread  8: pop:      1012767
Thread  9: pop:      1011106
```
</details>

---
**Operation pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 1 784 397 pops per second;
* x1.038 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 744 561
Items per sec median:    1 784 397
Items per sec max:       1 829 201

== Median report:
Items per sec:                 1784397
Items per sec per thread:       178439
Mutex contention count:           3156
Thread  0: pop:       500370
Thread  1: pop:       501999
Thread  2: pop:       498826
Thread  3: pop:       499748
Thread  4: pop:       499037
Thread  5: pop:       498155
Thread  6: pop:       501238
Thread  7: pop:       499488
Thread  8: pop:       499907
Thread  9: pop:       501232


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 651 513
Items per sec median:    1 719 835
Items per sec max:       1 785 098

== Median report:
Items per sec:                 1719835
Items per sec per thread:       171983
Mutex contention count:         762523
Thread  0: pop:       502427
Thread  1: pop:       500337
Thread  2: pop:       502067
Thread  3: pop:       507158
Thread  4: pop:       498947
Thread  5: pop:       495514
Thread  6: pop:       504192
Thread  7: pop:       497979
Thread  8: pop:       492619
Thread  9: pop:       498760
```
</details>
