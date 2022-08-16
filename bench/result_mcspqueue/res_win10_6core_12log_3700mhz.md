**OS**: Windows 10 Enterprise 19042<br>
**CPU**: Intel(R) Xeon(R) W-2135 CPU @ 3.70GHz, 3696 Mhz, 6 Core(s), 12 Logical Processor(s)

---
**Operation push, 100 000 000 items**

```
Command line args: -op push -subqsize 5000 -items 100000000
Run count: 5
Summary:
* 63 350 211 pops per second;
* x1.316 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      59 502 254
Items per sec median:   63 350 211
Items per sec max:      65 805 344

== Median report:
Items per sec:                63350211
Mutex contention count:              0


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      45 474 113
Items per sec median:   48 136 125
Items per sec max:      48 658 679

== Median report:
Items per sec:                48136125
Mutex contention count:              0
```
</details>

---
**Operation push-pop, 1 thread, 10 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 1 -items 10000000
Run count: 5
Summary:
* 11 005 578 pops per second;
* x2.514 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       8 156 148
Items per sec median:   11 005 578
Items per sec max:      12 503 019

== Median report:
Items per sec:                11005578
Mutex contention count:             25
Thread  0: pop:    10000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       3 440 483
Items per sec median:    4 377 302
Items per sec max:       5 554 461

== Median report:
Items per sec:                 4377302
Mutex contention count:        2353288
Thread  0: pop:    10000000
```
</details>

---
**Operation push-pop, 1 thread, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 1 -items 1000000
Run count: 5
Summary:
* 634 320 pops per second;
* x1.013 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         627 450
Items per sec median:      634 320
Items per sec max:         673 330

== Median report:
Items per sec:                  634320
Mutex contention count:              1
Thread  0: pop:     1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         602 116
Items per sec median:      626 316
Items per sec max:         643 057

== Median report:
Items per sec:                  626316
Mutex contention count:          19727
Thread  0: pop:     1000000
```
</details>

---
**Operation push-pop, 2 threads, 10 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 2 -items 10000000
Run count: 5
Summary:
* 7 533 474 pops per second;
* x3.551 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       6 129 638
Items per sec median:    7 533 474
Items per sec max:       7 919 353

== Median report:
Items per sec:                 7533474
Items per sec per thread:      3766737
Mutex contention count:           2241
Thread  0: pop:     5093713
Thread  1: pop:     4906287


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 364 002
Items per sec median:    2 121 707
Items per sec max:       2 876 155

== Median report:
Items per sec:                 2121707
Items per sec per thread:      1060853
Mutex contention count:        3183883
Thread  0: pop:     4366537
Thread  1: pop:     5633463
```
</details>

---
**Operation push-pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 1 197 293 pops per second;
* x1.112 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         446 167
Items per sec median:    1 197 293
Items per sec max:       1 208 936

== Median report:
Items per sec:                 1197293
Items per sec per thread:       598646
Mutex contention count:            588
Thread  0: pop:      498477
Thread  1: pop:      501523


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         434 104
Items per sec median:    1 076 561
Items per sec max:       1 187 879

== Median report:
Items per sec:                 1076561
Items per sec per thread:       538280
Mutex contention count:          72196
Thread  0: pop:      490037
Thread  1: pop:      509963
```
</details>

---
**Operation push-pop, 3 threads, 5 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 3 -items 5000000
Run count: 5
Summary:
* 3 521 498 pops per second;
* x2.141 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 483 421
Items per sec median:    3 521 498
Items per sec max:       4 404 057

== Median report:
Items per sec:                 3521498
Items per sec per thread:      1173832
Mutex contention count:           2807
Thread  0: pop:     1620476
Thread  1: pop:     1588478
Thread  2: pop:     1791046


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 048 416
Items per sec median:    1 644 543
Items per sec max:       1 759 854

== Median report:
Items per sec:                 1644543
Items per sec per thread:       548181
Mutex contention count:        5299034
Thread  0: pop:     1670141
Thread  1: pop:     1675425
Thread  2: pop:     1654434
```
</details>

---
**Operation push-pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 646 064 pops per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         660 065
Items per sec median:    1 646 064
Items per sec max:       1 676 473

== Median report:
Items per sec:                 1646064
Items per sec per thread:       548688
Mutex contention count:           2796
Thread  0: pop:      958309
Thread  1: pop:      995659
Thread  2: pop:     1046032


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 443 834
Items per sec median:    1 637 934
Items per sec max:       1 758 563

== Median report:
Items per sec:                 1637934
Items per sec per thread:       545978
Mutex contention count:         440813
Thread  0: pop:      995370
Thread  1: pop:      970514
Thread  2: pop:     1034116
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 5 -items 5000000
Run count: 5
Summary:
* 2 200 533 pops per second;
* x3.355 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 926 360
Items per sec median:    2 200 533
Items per sec max:       4 118 430

== Median report:
Items per sec:                 2200533
Items per sec per thread:       440106
Mutex contention count:           9069
Thread  0: pop:      958859
Thread  1: pop:     1054741
Thread  2: pop:      967689
Thread  3: pop:      946060
Thread  4: pop:     1072651


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         334 299
Items per sec median:      655 890
Items per sec max:         859 467

== Median report:
Items per sec:                  655890
Items per sec per thread:       131178
Mutex contention count:       11541449
Thread  0: pop:     1003053
Thread  1: pop:      973739
Thread  2: pop:      973606
Thread  3: pop:     1003410
Thread  4: pop:     1046192
```
</details>

---
**Operation push-pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 2 000 520 pops per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 870 824
Items per sec median:    2 000 520
Items per sec max:       2 558 511

== Median report:
Items per sec:                 2000520
Items per sec per thread:       400104
Mutex contention count:           1256
Thread  0: pop:     1248040
Thread  1: pop:     1327940
Thread  2: pop:      558368
Thread  3: pop:      596233
Thread  4: pop:     1269419


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 617 838
Items per sec median:    2 018 817
Items per sec max:       2 266 462

== Median report:
Items per sec:                 2018817
Items per sec per thread:       403763
Mutex contention count:        2029028
Thread  0: pop:      963655
Thread  1: pop:     1019238
Thread  2: pop:     1010977
Thread  3: pop:     1009606
Thread  4: pop:      996524
```
</details>

---
**Operation push-pop, 10 threads, 2 000 000 items**

```
Command line args: -op push-pop -subqsize 5000 -load empty -threads 10 -items 2000000
Run count: 5
Summary:
* 1 340 926 pops per second;
* x4.702 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 242 155
Items per sec median:    1 340 926
Items per sec max:       1 571 121

== Median report:
Items per sec:                 1340926
Items per sec per thread:       134092
Mutex contention count:           7296
Thread  0: pop:      200936
Thread  1: pop:      200725
Thread  2: pop:      206928
Thread  3: pop:      196728
Thread  4: pop:      193570
Thread  5: pop:      188774
Thread  6: pop:      195687
Thread  7: pop:      223575
Thread  8: pop:      193961
Thread  9: pop:      199116


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         198 549
Items per sec median:      285 188
Items per sec max:         348 263

== Median report:
Items per sec:                  285188
Items per sec per thread:        28518
Mutex contention count:        7489556
Thread  0: pop:      217682
Thread  1: pop:      176562
Thread  2: pop:      180891
Thread  3: pop:      215583
Thread  4: pop:      216883
Thread  5: pop:      219606
Thread  6: pop:      217807
Thread  7: pop:      179521
Thread  8: pop:      163749
Thread  9: pop:      211716
```
</details>

---
**Operation push-pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op push-pop -subqsize 5000 -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 3 808 834 pops per second;
* x3.748 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 524 949
Items per sec median:    3 808 834
Items per sec max:       3 871 953

== Median report:
Items per sec:                 3808834
Items per sec per thread:       380883
Mutex contention count:           7756
Thread  0: pop:      510638
Thread  1: pop:      485130
Thread  2: pop:      494699
Thread  3: pop:      503852
Thread  4: pop:      505414
Thread  5: pop:      494846
Thread  6: pop:      504586
Thread  7: pop:      498106
Thread  8: pop:      499208
Thread  9: pop:      503521


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         353 993
Items per sec median:    1 016 332
Items per sec max:       2 526 882

== Median report:
Items per sec:                 1016332
Items per sec per thread:       101633
Mutex contention count:        6546987
Thread  0: pop:      513098
Thread  1: pop:      451568
Thread  2: pop:      453091
Thread  3: pop:      567924
Thread  4: pop:      567667
Thread  5: pop:      523297
Thread  6: pop:      515209
Thread  7: pop:      448830
Thread  8: pop:      445453
Thread  9: pop:      513863
```
</details>

---
**Operation pop, 1 thread, 50 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 1 -items 50000000
Run count: 5
Summary:
* 36 501 047 pops per second;
* x0.887 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:      34 478 064
Items per sec median:   36 501 047
Items per sec max:      38 585 677

== Median report:
Items per sec:                36501047
Mutex contention count:              0
Thread  0: pop:    50000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:      35 826 725
Items per sec median:   41 159 689
Items per sec max:      42 507 540

== Median report:
Items per sec:                41159689
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
* 662 522 pops per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         660 211
Items per sec median:      662 522
Items per sec max:         685 522

== Median report:
Items per sec:                  662522
Mutex contention count:              0
Thread  0: pop:     1000000


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:         621 872
Items per sec median:      662 670
Items per sec max:         674 936

== Median report:
Items per sec:                  662670
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
* 8 281 989 pops per second;
* x1.124 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       7 584 847
Items per sec median:    8 281 989
Items per sec max:      11 274 112

== Median report:
Items per sec:                 8281989
Items per sec per thread:      4140994
Mutex contention count:           3599
Thread  0: pop:     5195760
Thread  1: pop:     4804240


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       6 586 232
Items per sec median:    7 369 282
Items per sec max:       7 770 802

== Median report:
Items per sec:                 7369282
Items per sec per thread:      3684641
Mutex contention count:        2545567
Thread  0: pop:     4846775
Thread  1: pop:     5153225
```
</details>

---
**Operation pop, 2 threads, 1 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 2 -items 1000000
Run count: 5
Summary:
* 1 144 540 pops per second;
* x0.928 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:         447 792
Items per sec median:    1 144 540
Items per sec max:       1 210 579

== Median report:
Items per sec:                 1144540
Items per sec per thread:       572270
Mutex contention count:            105
Thread  0: pop:      511350
Thread  1: pop:      488650


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 170 593
Items per sec median:    1 233 708
Items per sec max:       1 270 545

== Median report:
Items per sec:                 1233708
Items per sec per thread:       616854
Mutex contention count:          22469
Thread  0: pop:      504367
Thread  1: pop:      495633
```
</details>

---
**Operation pop, 3 threads, 10 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 3 -items 10000000
Run count: 5
Summary:
* 4 840 437 pops per second;
* x1.137 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       4 590 684
Items per sec median:    4 840 437
Items per sec max:       5 251 827

== Median report:
Items per sec:                 4840437
Items per sec per thread:      1613479
Mutex contention count:           3819
Thread  0: pop:     3253299
Thread  1: pop:     3512210
Thread  2: pop:     3234491


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       4 027 493
Items per sec median:    4 256 530
Items per sec max:       5 582 601

== Median report:
Items per sec:                 4256530
Items per sec per thread:      1418843
Mutex contention count:        3566770
Thread  0: pop:     3545135
Thread  1: pop:     3170779
Thread  2: pop:     3284086
```
</details>

---
**Operation pop, 3 threads, 3 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 3 -items 3000000
Run count: 5
Summary:
* 1 620 917 pops per second;
* x0.929 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 025 006
Items per sec median:    1 620 917
Items per sec max:       1 651 898

== Median report:
Items per sec:                 1620917
Items per sec per thread:       540305
Mutex contention count:           6545
Thread  0: pop:     1009251
Thread  1: pop:      995173
Thread  2: pop:      995576


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 707 644
Items per sec median:    1 745 499
Items per sec max:       1 794 792

== Median report:
Items per sec:                 1745499
Items per sec per thread:       581833
Mutex contention count:         156067
Thread  0: pop:     1003115
Thread  1: pop:      999971
Thread  2: pop:      996914
```
</details>

---
**Operation pop, 5 threads, 10 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 5 -items 10000000
Run count: 5
Summary:
* 4 855 220 pops per second;
* x1.525 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 963 284
Items per sec median:    4 855 220
Items per sec max:       4 962 086

== Median report:
Items per sec:                 4855220
Items per sec per thread:       971044
Mutex contention count:           8004
Thread  0: pop:     1965063
Thread  1: pop:     2000596
Thread  2: pop:     2044526
Thread  3: pop:     2026546
Thread  4: pop:     1963269


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 704 423
Items per sec median:    3 183 587
Items per sec max:       3 704 255

== Median report:
Items per sec:                 3183587
Items per sec per thread:       636717
Mutex contention count:        4528595
Thread  0: pop:     1928375
Thread  1: pop:     1919774
Thread  2: pop:     1948145
Thread  3: pop:     2001532
Thread  4: pop:     2202174
```
</details>

---
**Operation pop, 5 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 5 -items 5000000
Run count: 5
Summary:
* 2 081 747 pops per second;
* x0.764 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       1 711 658
Items per sec median:    2 081 747
Items per sec max:       2 379 214

== Median report:
Items per sec:                 2081747
Items per sec per thread:       416349
Mutex contention count:           3773
Thread  0: pop:     1267854
Thread  1: pop:     1286267
Thread  2: pop:      543144
Thread  3: pop:      572856
Thread  4: pop:     1329879


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 644 624
Items per sec median:    2 723 825
Items per sec max:       2 942 336

== Median report:
Items per sec:                 2723825
Items per sec per thread:       544765
Mutex contention count:         569260
Thread  0: pop:     1007411
Thread  1: pop:     1014787
Thread  2: pop:     1014485
Thread  3: pop:     1012175
Thread  4: pop:      951142
```
</details>

---
**Operation pop, 10 threads, 10 000 000 items**

```
Command line args: -op pop -subqsize 5000 -load empty -threads 10 -items 10000000
Run count: 5
Summary:
* 3 742 102 pops per second;
* x1.737 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       3 590 221
Items per sec median:    3 742 102
Items per sec max:       4 505 796

== Median report:
Items per sec:                 3742102
Items per sec per thread:       374210
Mutex contention count:          17849
Thread  0: pop:      988983
Thread  1: pop:     1004134
Thread  2: pop:      988245
Thread  3: pop:     1000902
Thread  4: pop:     1017938
Thread  5: pop:     1024231
Thread  6: pop:      974420
Thread  7: pop:      998102
Thread  8: pop:     1012735
Thread  9: pop:      990310


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       1 974 936
Items per sec median:    2 154 763
Items per sec max:       2 425 529

== Median report:
Items per sec:                 2154763
Items per sec per thread:       215476
Mutex contention count:        4829456
Thread  0: pop:      945033
Thread  1: pop:      990885
Thread  2: pop:      992646
Thread  3: pop:     1037701
Thread  4: pop:     1074013
Thread  5: pop:      977913
Thread  6: pop:      944199
Thread  7: pop:      975516
Thread  8: pop:      988254
Thread  9: pop:     1073840
```
</details>

---
**Operation pop, 10 threads, 5 000 000 items, each is processed**

```
Command line args: -op pop -subqsize 5000 -load micro -threads 10 -items 5000000
Run count: 5
Summary:
* 3 573 259 pops per second;
* x0.983 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MCSP queue
== Aggregated report:
Items per sec min:       2 935 152
Items per sec median:    3 573 259
Items per sec max:       3 880 997

== Median report:
Items per sec:                 3573259
Items per sec per thread:       357325
Mutex contention count:           4782
Thread  0: pop:      562989
Thread  1: pop:      534011
Thread  2: pop:      544656
Thread  3: pop:      549241
Thread  4: pop:      548409
Thread  5: pop:      545215
Thread  6: pop:      524588
Thread  7: pop:      544433
Thread  8: pop:      324644
Thread  9: pop:      321814


#### Trivial MCSP queue
== Aggregated report:
Items per sec min:       2 488 137
Items per sec median:    3 636 275
Items per sec max:       3 857 045

== Median report:
Items per sec:                 3636275
Items per sec per thread:       363627
Mutex contention count:        1586933
Thread  0: pop:      499909
Thread  1: pop:      491162
Thread  2: pop:      495569
Thread  3: pop:      512105
Thread  4: pop:      502929
Thread  5: pop:      500327
Thread  6: pop:      503297
Thread  7: pop:      490056
Thread  8: pop:      503584
Thread  9: pop:      501062
```
</details>
