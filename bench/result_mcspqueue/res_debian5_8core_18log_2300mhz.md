**OS**: Debian 5.10.136-1<br>
**CPU**: Intel(R) Xeon(R) CPU @ 2.30GHz, 8 CPUs, 2 threads per core

---
**Operation push, 100 000 000 items**

```
Command line args: -op push -subqsize 5000 -items 100000000
Run count: 5
Summary:
* 43 279 542 pops per second;
* x1.226 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      42 579 257
Items per sec median:   43 279 542
Items per sec max:      44 604 976

== Median report:
Items per sec:                43279542
Mutex contention count:              0


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      35 119 949
Items per sec median:   35 308 668
Items per sec max:      35 435 622

== Median report:
Items per sec:                35308668
Mutex contention count:              0
```
</details>

---
**Operation push-pop, 1 thread, 10 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 1 -items 10000000
Run count: 5
Summary:
* 4 289 950 pops per second;
* x0.572 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 878 508
Items per sec median:    4 289 950
Items per sec max:       4 631 437

== Median report:
Items per sec:                 4289950
Mutex contention count:            592
Thread  0: pop:    10000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       6 189 104
Items per sec median:    7 502 137
Items per sec max:       7 698 258

== Median report:
Items per sec:                 7502137
Mutex contention count:         806826
Thread  0: pop:    10000000
```
</details>

---
**Operation push-pop, 1 thread, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 1 -items 1000000
Run count: 5
Summary:
* 297 509 pops per second;
* x0.967 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         296 600
Items per sec median:      297 509
Items per sec max:         298 145

== Median report:
Items per sec:                  297509
Mutex contention count:              2
Thread  0: pop:     1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         304 216
Items per sec median:      307 674
Items per sec max:         321 130

== Median report:
Items per sec:                  307674
Mutex contention count:          10196
Thread  0: pop:     1000000
```
</details>

---
**Operation push-pop, 2 threads, 10 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 2 -items 10000000
Run count: 5
Summary:
* 2 895 562 pops per second;
* x0.568 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 859 689
Items per sec median:    2 895 562
Items per sec max:       2 934 185

== Median report:
Items per sec:                 2895562
Items per sec per thread:      1447781
Mutex contention count:           3484
Thread  0: pop:     4920910
Thread  1: pop:     5079090


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       4 673 011
Items per sec median:    5 094 472
Items per sec max:       5 504 210

== Median report:
Items per sec:                 5094472
Items per sec per thread:      2547236
Mutex contention count:        1614191
Thread  0: pop:     4971985
Thread  1: pop:     5028015
```
</details>

---
**Operation push-pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 584 180 pops per second;
* x0.989 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         582 205
Items per sec median:      584 180
Items per sec max:         587 437

== Median report:
Items per sec:                  584180
Items per sec per thread:       292090
Mutex contention count:            389
Thread  0: pop:      498549
Thread  1: pop:      501451


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         584 852
Items per sec median:      590 451
Items per sec max:         593 332

== Median report:
Items per sec:                  590451
Items per sec per thread:       295225
Mutex contention count:          31558
Thread  0: pop:      500775
Thread  1: pop:      499225
```
</details>

---
**Operation push-pop, 3 threads, 5 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 3 -items 5000000
Run count: 5
Summary:
* 2 501 912 pops per second;
* x0.894 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 474 134
Items per sec median:    2 501 912
Items per sec max:       2 529 489

== Median report:
Items per sec:                 2501912
Items per sec per thread:       833970
Mutex contention count:           2981
Thread  0: pop:     1666895
Thread  1: pop:     1660548
Thread  2: pop:     1672557


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 548 088
Items per sec median:    2 799 102
Items per sec max:       3 079 837

== Median report:
Items per sec:                 2799102
Items per sec per thread:       933034
Mutex contention count:        1846081
Thread  0: pop:     1716951
Thread  1: pop:     1671894
Thread  2: pop:     1611155
```
</details>

---
**Operation push-pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 881 845 pops per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         878 733
Items per sec median:      881 845
Items per sec max:         908 916

== Median report:
Items per sec:                  881845
Items per sec per thread:       293948
Mutex contention count:            845
Thread  0: pop:      999604
Thread  1: pop:      996990
Thread  2: pop:     1003406


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         865 467
Items per sec median:      880 813
Items per sec max:         891 237

== Median report:
Items per sec:                  880813
Items per sec per thread:       293604
Mutex contention count:         213605
Thread  0: pop:      997936
Thread  1: pop:      999984
Thread  2: pop:     1002080
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 5 -items 5000000
Run count: 5
Summary:
* 2 477 505 pops per second;
* x2.599 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 473 080
Items per sec median:    2 477 505
Items per sec max:       2 500 203

== Median report:
Items per sec:                 2477505
Items per sec per thread:       495501
Mutex contention count:           5415
Thread  0: pop:      238064
Thread  1: pop:      497963
Thread  2: pop:      496457
Thread  3: pop:     3521030
Thread  4: pop:      246486


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         941 650
Items per sec median:      953 074
Items per sec max:       1 412 913

== Median report:
Items per sec:                  953074
Items per sec per thread:       190614
Mutex contention count:        5430313
Thread  0: pop:      925437
Thread  1: pop:      984077
Thread  2: pop:     1089345
Thread  3: pop:     1015206
Thread  4: pop:      985935
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 1 291 296 pops per second;
* x1.053 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 268 950
Items per sec median:    1 291 296
Items per sec max:       1 345 461

== Median report:
Items per sec:                 1291296
Items per sec per thread:       258259
Mutex contention count:           2195
Thread  0: pop:      809323
Thread  1: pop:     1130329
Thread  2: pop:     1130581
Thread  3: pop:     1120444
Thread  4: pop:      809323


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 202 656
Items per sec median:    1 226 579
Items per sec max:       1 261 497

== Median report:
Items per sec:                 1226579
Items per sec per thread:       245315
Mutex contention count:         827380
Thread  0: pop:     1062140
Thread  1: pop:     1163806
Thread  2: pop:      889933
Thread  3: pop:      821479
Thread  4: pop:     1062642
```
</details>

---
**Operation push-pop, 10 threads, 2 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 10 -items 2000000
Run count: 5
Summary:
* 1 681 984 pops per second;
* x4.549 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 399 903
Items per sec median:    1 681 984
Items per sec max:       1 882 366

== Median report:
Items per sec:                 1681984
Items per sec per thread:       168198
Mutex contention count:           3831
Thread  0: pop:       63087
Thread  1: pop:       76204
Thread  2: pop:       78914
Thread  3: pop:       92154
Thread  4: pop:       97730
Thread  5: pop:      670470
Thread  6: pop:       67037
Thread  7: pop:       81109
Thread  8: pop:      102737
Thread  9: pop:      670558


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         366 083
Items per sec median:      369 745
Items per sec max:         394 362

== Median report:
Items per sec:                  369745
Items per sec per thread:        36974
Mutex contention count:        4971699
Thread  0: pop:      201096
Thread  1: pop:      199712
Thread  2: pop:      197987
Thread  3: pop:      202268
Thread  4: pop:      200554
Thread  5: pop:      198995
Thread  6: pop:      199164
Thread  7: pop:      202207
Thread  8: pop:      199686
Thread  9: pop:      198331
```
</details>

---
**Operation push-pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 1 637 581 pops per second;
* x1.116 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 630 357
Items per sec median:    1 637 581
Items per sec max:       1 643 252

== Median report:
Items per sec:                 1637581
Items per sec per thread:       163758
Mutex contention count:           3773
Thread  0: pop:      561038
Thread  1: pop:      440745
Thread  2: pop:      496466
Thread  3: pop:      467969
Thread  4: pop:      496473
Thread  5: pop:      481810
Thread  6: pop:      579512
Thread  7: pop:      513449
Thread  8: pop:      514393
Thread  9: pop:      448145


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 438 824
Items per sec median:    1 467 084
Items per sec max:       1 498 180

== Median report:
Items per sec:                 1467084
Items per sec per thread:       146708
Mutex contention count:        1358865
Thread  0: pop:      495732
Thread  1: pop:      517676
Thread  2: pop:      489418
Thread  3: pop:      512463
Thread  4: pop:      473710
Thread  5: pop:      513329
Thread  6: pop:      487317
Thread  7: pop:      510657
Thread  8: pop:      490417
Thread  9: pop:      509281
```
</details>

---
**Operation pop, 1 thread, 50 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 1 -items 50000000
Run count: 5
Summary:
* 40 939 240 pops per second;
* x1.470 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      40 832 587
Items per sec median:   40 939 240
Items per sec max:      41 239 913

== Median report:
Items per sec:                40939240
Mutex contention count:              0
Thread  0: pop:    50000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      27 253 821
Items per sec median:   27 850 348
Items per sec max:      29 507 602

== Median report:
Items per sec:                27850348
Mutex contention count:              0
Thread  0: pop:    50000000
```
</details>

---
**Operation pop, 1 thread, 1 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 1 -items 1000000
Run count: 5
Summary:
* 302 726 pops per second;
* x0.967 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         301 797
Items per sec median:      302 726
Items per sec max:         303 865

== Median report:
Items per sec:                  302726
Mutex contention count:              0
Thread  0: pop:     1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         309 146
Items per sec median:      312 922
Items per sec max:         339 390

== Median report:
Items per sec:                  312922
Mutex contention count:              0
Thread  0: pop:     1000000
```
</details>

---
**Operation pop, 2 threads, 10 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 2 -items 10000000
Run count: 5
Summary:
* 8 234 736 pops per second;
* x0.813 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       8 040 828
Items per sec median:    8 234 736
Items per sec max:       8 676 790

== Median report:
Items per sec:                 8234736
Items per sec per thread:      4117368
Mutex contention count:           1545
Thread  0: pop:     5040179
Thread  1: pop:     4959821


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      10 019 997
Items per sec median:   10 122 771
Items per sec max:      10 875 374

== Median report:
Items per sec:                10122771
Items per sec per thread:      5061385
Mutex contention count:         752438
Thread  0: pop:     4809900
Thread  1: pop:     5190100
```
</details>

---
**Operation pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 584 861 pops per second;
* x0.989 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         579 448
Items per sec median:      584 861
Items per sec max:         619 609

== Median report:
Items per sec:                  584861
Items per sec per thread:       292430
Mutex contention count:             85
Thread  0: pop:      500470
Thread  1: pop:      499530


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         590 503
Items per sec median:      591 598
Items per sec max:         599 837

== Median report:
Items per sec:                  591598
Items per sec per thread:       295799
Mutex contention count:           7103
Thread  0: pop:      500719
Thread  1: pop:      499281
```
</details>

---
**Operation pop, 3 threads, 10 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 3 -items 10000000
Run count: 5
Summary:
* 6 967 231 pops per second;
* x0.596 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       6 862 982
Items per sec median:    6 967 231
Items per sec max:       7 032 436

== Median report:
Items per sec:                 6967231
Items per sec per thread:      2322410
Mutex contention count:           4217
Thread  0: pop:     3332296
Thread  1: pop:     3313974
Thread  2: pop:     3353730


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      11 037 480
Items per sec median:   11 685 030
Items per sec max:      12 966 784

== Median report:
Items per sec:                11685030
Items per sec per thread:      3895010
Mutex contention count:         803478
Thread  0: pop:     3374302
Thread  1: pop:     3222897
Thread  2: pop:     3402801
```
</details>

---
**Operation pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 869 900 pops per second;
* x0.989 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         866 535
Items per sec median:      869 900
Items per sec max:         874 423

== Median report:
Items per sec:                  869900
Items per sec per thread:       289966
Mutex contention count:            850
Thread  0: pop:      998019
Thread  1: pop:     1000758
Thread  2: pop:     1001223


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         878 002
Items per sec median:      879 952
Items per sec max:         890 747

== Median report:
Items per sec:                  879952
Items per sec per thread:       293317
Mutex contention count:          77952
Thread  0: pop:     1001899
Thread  1: pop:     1001333
Thread  2: pop:      996768
```
</details>

---
**Operation pop, 5 threads, 10 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 5 -items 10000000
Run count: 5
Summary:
* 6 373 217 pops per second;
* x0.945 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       6 361 785
Items per sec median:    6 373 217
Items per sec max:       6 381 439

== Median report:
Items per sec:                 6373217
Items per sec per thread:      1274643
Mutex contention count:           7851
Thread  0: pop:     2494396
Thread  1: pop:     1287929
Thread  2: pop:     2478128
Thread  3: pop:     2454644
Thread  4: pop:     1284903


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       6 672 321
Items per sec median:    6 746 774
Items per sec max:       7 602 021

== Median report:
Items per sec:                 6746774
Items per sec per thread:      1349354
Mutex contention count:        1658434
Thread  0: pop:     1511927
Thread  1: pop:     1873143
Thread  2: pop:     2182823
Thread  3: pop:     2248790
Thread  4: pop:     2183317
```
</details>

---
**Operation pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 1 269 133 pops per second;
* x1.011 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 267 322
Items per sec median:    1 269 133
Items per sec max:       1 285 826

== Median report:
Items per sec:                 1269133
Items per sec per thread:       253826
Mutex contention count:           1387
Thread  0: pop:     1133678
Thread  1: pop:      802390
Thread  2: pop:     1130020
Thread  3: pop:     1131717
Thread  4: pop:      802195


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 249 289
Items per sec median:    1 255 411
Items per sec max:       1 286 375

== Median report:
Items per sec:                 1255411
Items per sec per thread:       251082
Mutex contention count:         551099
Thread  0: pop:      927168
Thread  1: pop:     1003071
Thread  2: pop:     1138070
Thread  3: pop:     1005083
Thread  4: pop:      926608
```
</details>

---
**Operation pop, 10 threads, 10 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 10 -items 10000000
Run count: 5
Summary:
* 6 160 327 pops per second;
* x1.378 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       6 043 202
Items per sec median:    6 160 327
Items per sec max:       6 172 225

== Median report:
Items per sec:                 6160327
Items per sec per thread:       616032
Mutex contention count:          13934
Thread  0: pop:      943374
Thread  1: pop:      838527
Thread  2: pop:     1002510
Thread  3: pop:     1215144
Thread  4: pop:     1022098
Thread  5: pop:      932859
Thread  6: pop:     1051537
Thread  7: pop:      942236
Thread  8: pop:     1011164
Thread  9: pop:     1040551


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       4 007 166
Items per sec median:    4 471 966
Items per sec max:       4 814 128

== Median report:
Items per sec:                 4471966
Items per sec per thread:       447196
Mutex contention count:        2344179
Thread  0: pop:      999391
Thread  1: pop:     1006921
Thread  2: pop:      999368
Thread  3: pop:      992523
Thread  4: pop:     1007016
Thread  5: pop:      996812
Thread  6: pop:      995306
Thread  7: pop:      989731
Thread  8: pop:     1015651
Thread  9: pop:      997281
```
</details>

---
**Operation pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 1 621 446 pops per second;
* x1.046 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 618 380
Items per sec median:    1 621 446
Items per sec max:       1 630 448

== Median report:
Items per sec:                 1621446
Items per sec per thread:       162144
Mutex contention count:           3584
Thread  0: pop:      526771
Thread  1: pop:      526458
Thread  2: pop:      443396
Thread  3: pop:      467349
Thread  4: pop:      518136
Thread  5: pop:      501236
Thread  6: pop:      545743
Thread  7: pop:      481053
Thread  8: pop:      538500
Thread  9: pop:      451358


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 538 968
Items per sec median:    1 550 729
Items per sec max:       1 605 052

== Median report:
Items per sec:                 1550729
Items per sec per thread:       155072
Mutex contention count:         648883
Thread  0: pop:      495175
Thread  1: pop:      503441
Thread  2: pop:      505642
Thread  3: pop:      507796
Thread  4: pop:      505901
Thread  5: pop:      511291
Thread  6: pop:      488973
Thread  7: pop:      498643
Thread  8: pop:      495773
Thread  9: pop:      487365
```
</details>
