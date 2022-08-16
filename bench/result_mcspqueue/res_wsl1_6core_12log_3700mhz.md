**OS**: WSL 1 Linux 4.4.0-19041-Microsoft on Windows 10 Enterprise 19042<br>
**CPU**: Intel(R) Xeon(R) W-2135 CPU @ 3.70GHz, 3696 Mhz, 6 Core(s), 12 Logical Processor(s)

---
**Operation push, 100 000 000 items**

```
Command line args: -op push -items 100000000
Run count: 5
Summary:
* 54 487 096 pops per second;
* x0.950 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      51 846 239
Items per sec median:   54 487 096
Items per sec max:      55 520 213

== Median report:
Items per sec:                54487096
Mutex contention count:              0


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      53 794 716
Items per sec median:   57 381 310
Items per sec max:      58 395 466

== Median report:
Items per sec:                57381310
Mutex contention count:              0
```
</details>

---
**Operation push-pop, 1 thread, 10 000 000 items**

```
Command line args: -op push-pop -load empty -threads 1 -items 10000000
Run count: 5
Summary:
* 4 927 865 pops per second;
* x0.478 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       4 877 296
Items per sec median:    4 927 865
Items per sec max:       6 193 789

== Median report:
Items per sec:                 4927865
Mutex contention count:            813
Thread  0: pop:     10000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       9 856 711
Items per sec median:   10 313 873
Items per sec max:      10 881 308

== Median report:
Items per sec:                10313873
Mutex contention count:         714488
Thread  0: pop:     10000000
```
</details>

---
**Operation push-pop, 1 thread, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 1 -items 1000000
Run count: 5
Summary:
* 634 788 pops per second;
* x1.027 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         611 769
Items per sec median:      634 788
Items per sec max:         661 179

== Median report:
Items per sec:                  634788
Mutex contention count:              2
Thread  0: pop:      1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         572 505
Items per sec median:      618 303
Items per sec max:         622 372

== Median report:
Items per sec:                  618303
Mutex contention count:          15949
Thread  0: pop:      1000000
```
</details>

---
**Operation push-pop, 2 threads, 10 000 000 items**

```
Command line args: -op push-pop -load empty -threads 2 -items 10000000
Run count: 5
Summary:
* 3 637 258 pops per second;
* x0.485 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 490 524
Items per sec median:    3 637 258
Items per sec max:       3 998 133

== Median report:
Items per sec:                 3637258
Items per sec per thread:      1818629
Mutex contention count:           4558
Thread  0: pop:      4849362
Thread  1: pop:      5150638


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       7 199 900
Items per sec median:    7 500 622
Items per sec max:       8 371 339

== Median report:
Items per sec:                 7500622
Items per sec per thread:      3750311
Mutex contention count:        1031938
Thread  0: pop:      4982507
Thread  1: pop:      5017493
```
</details>

---
**Operation push-pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 1 221 883 pops per second;
* x1.114 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 160 173
Items per sec median:    1 221 883
Items per sec max:       1 253 450

== Median report:
Items per sec:                 1221883
Items per sec per thread:       610941
Mutex contention count:            652
Thread  0: pop:       500557
Thread  1: pop:       499443


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 030 719
Items per sec median:    1 097 324
Items per sec max:       1 159 896

== Median report:
Items per sec:                 1097324
Items per sec per thread:       548662
Mutex contention count:          57929
Thread  0: pop:       495500
Thread  1: pop:       504500
```
</details>

---
**Operation push-pop, 3 threads, 5 000 000 items**

```
Command line args: -op push-pop -load empty -threads 3 -items 5000000
Run count: 5
Summary:
* 3 223 277 pops per second;
* x0.732 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 163 850
Items per sec median:    3 223 277
Items per sec max:       3 332 748

== Median report:
Items per sec:                 3223277
Items per sec per thread:      1074425
Mutex contention count:           3412
Thread  0: pop:      1760934
Thread  1: pop:      1596055
Thread  2: pop:      1643011


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       3 756 549
Items per sec median:    4 404 881
Items per sec max:       4 565 896

== Median report:
Items per sec:                 4404881
Items per sec per thread:      1468293
Mutex contention count:        1167216
Thread  0: pop:      1672372
Thread  1: pop:      1646649
Thread  2: pop:      1680979
```
</details>

---
**Operation push-pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 679 113 pops per second;
* x1.078 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 616 853
Items per sec median:    1 679 113
Items per sec max:       1 750 309

== Median report:
Items per sec:                 1679113
Items per sec per thread:       559704
Mutex contention count:           1306
Thread  0: pop:       997032
Thread  1: pop:       978852
Thread  2: pop:      1024116


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 543 999
Items per sec median:    1 557 499
Items per sec max:       1 704 331

== Median report:
Items per sec:                 1557499
Items per sec per thread:       519166
Mutex contention count:         359019
Thread  0: pop:      1006434
Thread  1: pop:       988048
Thread  2: pop:      1005518
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items**

```
Command line args: -op push-pop -load empty -threads 5 -items 5000000
Run count: 5
Summary:
* 2 685 289 pops per second;
* x1.355 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 580 678
Items per sec median:    2 685 289
Items per sec max:       6 024 055

== Median report:
Items per sec:                 2685289
Items per sec per thread:       537057
Mutex contention count:           6612
Thread  0: pop:       988140
Thread  1: pop:      1043381
Thread  2: pop:      1011773
Thread  3: pop:       907842
Thread  4: pop:      1048864


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 744 302
Items per sec median:    1 981 897
Items per sec max:       2 072 108

== Median report:
Items per sec:                 1981897
Items per sec per thread:       396379
Mutex contention count:        2658551
Thread  0: pop:       994302
Thread  1: pop:       989287
Thread  2: pop:       997568
Thread  3: pop:       998770
Thread  4: pop:      1020073
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 2 660 780 pops per second;
* x1.139 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 575 787
Items per sec median:    2 660 780
Items per sec max:       2 709 953

== Median report:
Items per sec:                 2660780
Items per sec per thread:       532156
Mutex contention count:           2677
Thread  0: pop:       989433
Thread  1: pop:      1001510
Thread  2: pop:      1009178
Thread  3: pop:      1016034
Thread  4: pop:       983845


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 227 839
Items per sec median:    2 335 730
Items per sec max:       2 435 008

== Median report:
Items per sec:                 2335730
Items per sec per thread:       467146
Mutex contention count:        1314824
Thread  0: pop:      1007049
Thread  1: pop:      1001560
Thread  2: pop:       996228
Thread  3: pop:       987845
Thread  4: pop:      1007318
```
</details>

---
**Operation push-pop, 10 threads, 2 000 000 items**

```
Command line args: -op push-pop -load empty -threads 10 -items 2000000
Run count: 5
Summary:
* 2 132 336 pops per second;
* x3.323 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 050 648
Items per sec median:    2 132 336
Items per sec max:       2 199 167

== Median report:
Items per sec:                 2132336
Items per sec per thread:       213233
Mutex contention count:           6474
Thread  0: pop:       153460
Thread  1: pop:       187223
Thread  2: pop:       151818
Thread  3: pop:       219617
Thread  4: pop:       250822
Thread  5: pop:       298276
Thread  6: pop:       171071
Thread  7: pop:       137621
Thread  8: pop:       191381
Thread  9: pop:       238711


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         609 356
Items per sec median:      641 654
Items per sec max:         686 088

== Median report:
Items per sec:                  641654
Items per sec per thread:        64165
Mutex contention count:        2931604
Thread  0: pop:       204287
Thread  1: pop:       202522
Thread  2: pop:       197780
Thread  3: pop:       199017
Thread  4: pop:       200085
Thread  5: pop:       199339
Thread  6: pop:       201376
Thread  7: pop:       199404
Thread  8: pop:       198556
Thread  9: pop:       197634
```
</details>

---
**Operation push-pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 4 270 134 pops per second;
* x2.255 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       4 036 473
Items per sec median:    4 270 134
Items per sec max:       4 309 165

== Median report:
Items per sec:                 4270134
Items per sec per thread:       427013
Mutex contention count:           5144
Thread  0: pop:       502953
Thread  1: pop:       492271
Thread  2: pop:       504923
Thread  3: pop:       499509
Thread  4: pop:       502597
Thread  5: pop:       498697
Thread  6: pop:       506964
Thread  7: pop:       486588
Thread  8: pop:       500633
Thread  9: pop:       504865


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 506 354
Items per sec median:    1 893 700
Items per sec max:       2 766 924

== Median report:
Items per sec:                 1893700
Items per sec per thread:       189370
Mutex contention count:        2797598
Thread  0: pop:       503579
Thread  1: pop:       501670
Thread  2: pop:       498691
Thread  3: pop:       500105
Thread  4: pop:       496170
Thread  5: pop:       495647
Thread  6: pop:       500098
Thread  7: pop:       503271
Thread  8: pop:       500885
Thread  9: pop:       499884
```
</details>

---
**Operation pop, 1 thread, 50 000 000 items**

```
Command line args: -op pop -load empty -threads 1 -items 50000000
Run count: 5
Summary:
* 58 905 491 pops per second;
* x1.315 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      53 191 800
Items per sec median:   58 905 491
Items per sec max:      61 957 054

== Median report:
Items per sec:                58905491
Mutex contention count:              0
Thread  0: pop:     50000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      43 315 192
Items per sec median:   44 794 546
Items per sec max:      46 250 948

== Median report:
Items per sec:                44794546
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
* 636 272 pops per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         617 207
Items per sec median:      636 272
Items per sec max:         658 181

== Median report:
Items per sec:                  636272
Mutex contention count:              0
Thread  0: pop:      1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         601 670
Items per sec median:      630 457
Items per sec max:         647 727

== Median report:
Items per sec:                  630457
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
* 12 703 565 pops per second;
* x0.821 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      10 303 341
Items per sec median:   12 703 565
Items per sec max:      20 123 789

== Median report:
Items per sec:                12703565
Items per sec per thread:      6351782
Mutex contention count:           2044
Thread  0: pop:      4906577
Thread  1: pop:      5093423


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      15 380 365
Items per sec median:   15 471 656
Items per sec max:      16 052 876

== Median report:
Items per sec:                15471656
Items per sec per thread:      7735828
Mutex contention count:         643799
Thread  0: pop:      5125651
Thread  1: pop:      4874349
```
</details>

---
**Operation pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 1 209 365 pops per second;
* x1.050 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 134 543
Items per sec median:    1 209 365
Items per sec max:       1 245 289

== Median report:
Items per sec:                 1209365
Items per sec per thread:       604682
Mutex contention count:            662
Thread  0: pop:       504161
Thread  1: pop:       495839


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 133 837
Items per sec median:    1 151 903
Items per sec max:       1 201 008

== Median report:
Items per sec:                 1151903
Items per sec per thread:       575951
Mutex contention count:           8045
Thread  0: pop:       517579
Thread  1: pop:       482421
```
</details>

---
**Operation pop, 3 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 3 -items 10000000
Run count: 5
Summary:
* 9 370 780 pops per second;
* x0.503 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       9 314 807
Items per sec median:    9 370 780
Items per sec max:       9 744 725

== Median report:
Items per sec:                 9370780
Items per sec per thread:      3123593
Mutex contention count:           4207
Thread  0: pop:      3284545
Thread  1: pop:      3348342
Thread  2: pop:      3367113


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      17 334 065
Items per sec median:   18 638 880
Items per sec max:      18 975 537

== Median report:
Items per sec:                18638880
Items per sec per thread:      6212960
Mutex contention count:         541283
Thread  0: pop:      3302070
Thread  1: pop:      3291138
Thread  2: pop:      3406792
```
</details>

---
**Operation pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 693 257 pops per second;
* x1.080 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 628 039
Items per sec median:    1 693 257
Items per sec max:       1 749 270

== Median report:
Items per sec:                 1693257
Items per sec per thread:       564419
Mutex contention count:            642
Thread  0: pop:       996109
Thread  1: pop:      1010422
Thread  2: pop:       993469


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 522 283
Items per sec median:    1 568 249
Items per sec max:       1 679 632

== Median report:
Items per sec:                 1568249
Items per sec per thread:       522749
Mutex contention count:         184764
Thread  0: pop:       973158
Thread  1: pop:      1015667
Thread  2: pop:      1011175
```
</details>

---
**Operation pop, 5 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 5 -items 10000000
Run count: 5
Summary:
* 8 185 511 pops per second;
* x0.724 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       8 095 537
Items per sec median:    8 185 511
Items per sec max:       8 486 300

== Median report:
Items per sec:                 8185511
Items per sec per thread:      1637102
Mutex contention count:           8491
Thread  0: pop:      1974008
Thread  1: pop:      2047181
Thread  2: pop:      1991954
Thread  3: pop:      1996395
Thread  4: pop:      1990462


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      10 390 030
Items per sec median:   11 299 138
Items per sec max:      13 118 669

== Median report:
Items per sec:                11299138
Items per sec per thread:      2259827
Mutex contention count:        1025565
Thread  0: pop:      2013894
Thread  1: pop:      1991738
Thread  2: pop:      2013118
Thread  3: pop:      2056166
Thread  4: pop:      1925084
```
</details>

---
**Operation pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 2 656 935 pops per second;
* x1.034 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 600 790
Items per sec median:    2 656 935
Items per sec max:       2 692 869

== Median report:
Items per sec:                 2656935
Items per sec per thread:       531387
Mutex contention count:           1173
Thread  0: pop:      1015977
Thread  1: pop:       993959
Thread  2: pop:      1000832
Thread  3: pop:       985984
Thread  4: pop:      1003248


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 519 669
Items per sec median:    2 569 257
Items per sec max:       2 661 170

== Median report:
Items per sec:                 2569257
Items per sec per thread:       513851
Mutex contention count:         725023
Thread  0: pop:      1004199
Thread  1: pop:      1001626
Thread  2: pop:      1009167
Thread  3: pop:       982609
Thread  4: pop:      1002399
```
</details>

---
**Operation pop, 10 threads, 10 000 000 items**

```
Command line args: -op pop -load empty -threads 10 -items 10000000
Run count: 5
Summary:
* 8 058 396 pops per second;
* x1.150 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       8 011 682
Items per sec median:    8 058 396
Items per sec max:       8 222 650

== Median report:
Items per sec:                 8058396
Items per sec per thread:       805839
Mutex contention count:          17444
Thread  0: pop:       989070
Thread  1: pop:       988770
Thread  2: pop:       985349
Thread  3: pop:       958625
Thread  4: pop:      1076650
Thread  5: pop:      1059969
Thread  6: pop:       988398
Thread  7: pop:      1000654
Thread  8: pop:       972048
Thread  9: pop:       980467


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       6 690 393
Items per sec median:    7 009 968
Items per sec max:       7 642 347

== Median report:
Items per sec:                 7009968
Items per sec per thread:       700996
Mutex contention count:        1358491
Thread  0: pop:      1072375
Thread  1: pop:      1032527
Thread  2: pop:       953952
Thread  3: pop:       972665
Thread  4: pop:      1006468
Thread  5: pop:       999132
Thread  6: pop:      1004078
Thread  7: pop:      1054225
Thread  8: pop:       953912
Thread  9: pop:       950666
```
</details>

---
**Operation pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 4 327 847 pops per second;
* x1.356 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 693 946
Items per sec median:    4 327 847
Items per sec max:       4 434 444

== Median report:
Items per sec:                 4327847
Items per sec per thread:       432784
Mutex contention count:           5345
Thread  0: pop:       493208
Thread  1: pop:       501302
Thread  2: pop:       498380
Thread  3: pop:       502466
Thread  4: pop:       509943
Thread  5: pop:       489768
Thread  6: pop:       501439
Thread  7: pop:       486934
Thread  8: pop:       506912
Thread  9: pop:       509648


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 935 947
Items per sec median:    3 192 769
Items per sec max:       3 233 551

== Median report:
Items per sec:                 3192769
Items per sec per thread:       319276
Mutex contention count:        1467049
Thread  0: pop:       500709
Thread  1: pop:       504308
Thread  2: pop:       495820
Thread  3: pop:       494844
Thread  4: pop:       506736
Thread  5: pop:       508275
Thread  6: pop:       505293
Thread  7: pop:       497235
Thread  8: pop:       493827
Thread  9: pop:       492953
```
</details>
