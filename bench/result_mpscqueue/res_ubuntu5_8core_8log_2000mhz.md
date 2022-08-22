**OS**: Ubuntu 5.4.0-42-generic<br>
**CPU**: Intel Core Processor (Haswell, no TSX, IBRS) 2.0GHz, 8 cores, 1 thread per core

---
**Empty load, 1 sender thread, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 1 -items 15000000 -signal 0
Run count: 3
Summary:
* 4 648 531 items per second;
* x1.324 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 354 779
Items/sec median:    4 648 531
Items/sec max:       4 997 067

== Median report:
Items/sec:                       4648531
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 265 427
Items/sec median:    3 511 937
Items/sec max:       3 636 848

== Median report:
Items/sec:                       3511937
Mutex contentions/sec:           1248845
```
</details>

---
**Empty load, 1 sender thread, 35 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 1 -items 35000000 -signal 1
Run count: 3
Summary:
* 10 073 802 items per second;
* x1.460 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       8 908 222
Items/sec median:   10 073 802
Items/sec max:      10 647 978

== Median report:
Items/sec:                      10073802
Mutex contentions/sec:            177829


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       6 143 494
Items/sec median:    6 898 775
Items/sec max:       7 735 795

== Median report:
Items/sec:                       6898775
Mutex contentions/sec:            335724
```
</details>

---
**Empty load, 2 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 2 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 200 345 items per second;
* x1.048 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 742 101
Items/sec median:    5 200 345
Items/sec max:       6 001 774

== Median report:
Items/sec:                       5200345
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 755 296
Items/sec median:    4 961 844
Items/sec max:       5 163 355

== Median report:
Items/sec:                       4961844
Mutex contentions/sec:            977380
```
</details>

---
**Empty load, 2 sender threads, 30 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 2 -items 30000000 -signal 1
Run count: 3
Summary:
* 16 444 877 items per second;
* x2.046 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      14 781 275
Items/sec median:   16 444 877
Items/sec max:      16 572 705

== Median report:
Items/sec:                      16444877
Mutex contentions/sec:            355509


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       7 185 917
Items/sec median:    8 037 502
Items/sec max:       8 130 553

== Median report:
Items/sec:                       8037502
Mutex contentions/sec:            921632
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 000 723 items per second;
* x1.271 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 641 050
Items/sec median:    5 000 723
Items/sec max:       6 273 792

== Median report:
Items/sec:                       5000723
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 781 885
Items/sec median:    3 933 751
Items/sec max:       4 131 090

== Median report:
Items/sec:                       3933751
Mutex contentions/sec:           1105198
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 1
Run count: 3
Summary:
* 9 315 811 items per second;
* x1.554 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       8 312 054
Items/sec median:    9 315 811
Items/sec max:      10 284 148

== Median report:
Items/sec:                       9315811
Mutex contentions/sec:            120534


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 431 279
Items/sec median:    5 994 881
Items/sec max:       6 344 005

== Median report:
Items/sec:                       5994881
Mutex contentions/sec:            972825
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 090 921 items per second;
* x1.212 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 944 500
Items/sec median:    5 090 921
Items/sec max:       5 578 655

== Median report:
Items/sec:                       5090921
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 987 947
Items/sec median:    4 201 953
Items/sec max:       4 251 378

== Median report:
Items/sec:                       4201953
Mutex contentions/sec:           1182725
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 1
Run count: 3
Summary:
* 7 548 467 items per second;
* x1.636 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 394 290
Items/sec median:    7 548 467
Items/sec max:       7 577 972

== Median report:
Items/sec:                       7548467
Mutex contentions/sec:            177115


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 571 516
Items/sec median:    4 613 099
Items/sec max:       4 844 904

== Median report:
Items/sec:                       4613099
Mutex contentions/sec:           1113151
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 308 484 items per second;
* x1.485 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 110 949
Items/sec median:    5 308 484
Items/sec max:       6 834 300

== Median report:
Items/sec:                       5308484
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 511 950
Items/sec median:    3 574 549
Items/sec max:       3 800 108

== Median report:
Items/sec:                       3574549
Mutex contentions/sec:           1094272
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 9 428 436 items per second;
* x2.495 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 661 805
Items/sec median:    9 428 436
Items/sec max:      10 016 138

== Median report:
Items/sec:                       9428436
Mutex contentions/sec:             40131


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 725 473
Items/sec median:    3 779 212
Items/sec max:       3 893 160

== Median report:
Items/sec:                       3779212
Mutex contentions/sec:           1114745
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 0
Run count: 3
Summary:
* 1 351 938 items per second;
* x1.114 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 336 820
Items/sec median:    1 351 938
Items/sec max:       1 381 320

== Median report:
Items/sec:                       1351938
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 196 357
Items/sec median:    1 213 215
Items/sec max:       1 274 154

== Median report:
Items/sec:                       1213215
Mutex contentions/sec:             68618
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 115 889 items per second;
* x1.117 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 113 620
Items/sec median:    1 115 889
Items/sec max:       1 157 907

== Median report:
Items/sec:                       1115889
Mutex contentions/sec:             25479


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         959 140
Items/sec median:      999 134
Items/sec max:       1 039 345

== Median report:
Items/sec:                        999134
Mutex contentions/sec:            128845
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 0
Run count: 3
Summary:
* 2 534 371 items per second;
* x1.322 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 489 407
Items/sec median:    2 534 371
Items/sec max:       2 598 081

== Median report:
Items/sec:                       2534371
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 766 819
Items/sec median:    1 917 116
Items/sec max:       2 300 075

== Median report:
Items/sec:                       1917116
Mutex contentions/sec:            522437
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 1
Run count: 3
Summary:
* 2 217 706 items per second;
* x1.200 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 199 473
Items/sec median:    2 217 706
Items/sec max:       2 244 044

== Median report:
Items/sec:                       2217706
Mutex contentions/sec:             84423


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 830 510
Items/sec median:    1 848 460
Items/sec max:       1 886 433

== Median report:
Items/sec:                       1848460
Mutex contentions/sec:            371025
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 0
Run count: 3
Summary:
* 3 553 607 items per second;
* x1.417 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 543 639
Items/sec median:    3 553 607
Items/sec max:       3 793 358

== Median report:
Items/sec:                       3553607
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 243 238
Items/sec median:    2 508 640
Items/sec max:       2 620 077

== Median report:
Items/sec:                       2508640
Mutex contentions/sec:            870454
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 1
Run count: 3
Summary:
* 3 169 865 items per second;
* x1.254 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 163 947
Items/sec median:    3 169 865
Items/sec max:       3 325 796

== Median report:
Items/sec:                       3169865
Mutex contentions/sec:             41302


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 210 979
Items/sec median:    2 527 169
Items/sec max:       2 560 949

== Median report:
Items/sec:                       2527169
Mutex contentions/sec:            738156
```
</details>

---
**Nano load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 4 703 275 items per second;
* x2.059 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 347 671
Items/sec median:    4 703 275
Items/sec max:       4 748 761

== Median report:
Items/sec:                       4703275
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 165 677
Items/sec median:    2 284 092
Items/sec max:       2 401 114

== Median report:
Items/sec:                       2284092
Mutex contentions/sec:           1086942
```
</details>

---
**Nano load, 5 sender threads, 13 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 5 -items 13000000 -signal 1
Run count: 3
Summary:
* 5 321 584 items per second;
* x2.264 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 969 057
Items/sec median:    5 321 584
Items/sec max:       5 395 873

== Median report:
Items/sec:                       5321584
Mutex contentions/sec:               634


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 225 932
Items/sec median:    2 350 942
Items/sec max:       2 634 373

== Median report:
Items/sec:                       2350942
Mutex contentions/sec:            994280
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 712 454 items per second;
* x2.723 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 625 693
Items/sec median:    5 712 454
Items/sec max:       5 823 816

== Median report:
Items/sec:                       5712454
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 093 290
Items/sec median:    2 097 926
Items/sec max:       2 114 265

== Median report:
Items/sec:                       2097926
Mutex contentions/sec:           1113630
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 5 549 996 items per second;
* x2.160 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 037 410
Items/sec median:    5 549 996
Items/sec max:       5 854 885

== Median report:
Items/sec:                       5549996
Mutex contentions/sec:             10705


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 229 559
Items/sec median:    2 568 931
Items/sec max:       2 583 120

== Median report:
Items/sec:                       2568931
Mutex contentions/sec:           1161791
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 0
Run count: 3
Summary:
* 323 964 items per second;
* x1.027 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         318 205
Items/sec median:      323 964
Items/sec max:         324 739

== Median report:
Items/sec:                        323964
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         310 321
Items/sec median:      315 318
Items/sec max:         317 606

== Median report:
Items/sec:                        315318
Mutex contentions/sec:              6295
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 1
Run count: 3
Summary:
* 302 650 items per second;
* x1.015 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         296 875
Items/sec median:      302 650
Items/sec max:         314 870

== Median report:
Items/sec:                        302650
Mutex contentions/sec:             11983


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         293 082
Items/sec median:      298 193
Items/sec max:         300 748

== Median report:
Items/sec:                        298193
Mutex contentions/sec:              8700
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 0
Run count: 3
Summary:
* 643 897 items per second;
* x1.035 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         639 714
Items/sec median:      643 897
Items/sec max:         650 994

== Median report:
Items/sec:                        643897
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         618 024
Items/sec median:      622 309
Items/sec max:         642 207

== Median report:
Items/sec:                        622309
Mutex contentions/sec:             54545
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 1
Run count: 3
Summary:
* 606 670 items per second;
* x1.015 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         600 483
Items/sec median:      606 670
Items/sec max:         616 568

== Median report:
Items/sec:                        606670
Mutex contentions/sec:              1622


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         587 006
Items/sec median:      597 775
Items/sec max:         607 530

== Median report:
Items/sec:                        597775
Mutex contentions/sec:             45512
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 0
Run count: 3
Summary:
* 928 396 items per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         923 545
Items/sec median:      928 396
Items/sec max:         948 541

== Median report:
Items/sec:                        928396
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         912 321
Items/sec median:      925 022
Items/sec max:         942 752

== Median report:
Items/sec:                        925022
Mutex contentions/sec:             94885
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 1
Run count: 3
Summary:
* 916 049 items per second;
* x1.031 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         913 456
Items/sec median:      916 049
Items/sec max:         921 758

== Median report:
Items/sec:                        916049
Mutex contentions/sec:               729


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         849 797
Items/sec median:      888 542
Items/sec max:         890 953

== Median report:
Items/sec:                        888542
Mutex contentions/sec:            105445
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 0
Run count: 3
Summary:
* 1 501 615 items per second;
* x1.061 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 476 554
Items/sec median:    1 501 615
Items/sec max:       1 558 364

== Median report:
Items/sec:                       1501615
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 406 550
Items/sec median:    1 415 644
Items/sec max:       1 468 618

== Median report:
Items/sec:                       1415644
Mutex contentions/sec:            206043
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 459 847 items per second;
* x1.058 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 422 875
Items/sec median:    1 459 847
Items/sec max:       1 462 402

== Median report:
Items/sec:                       1459847
Mutex contentions/sec:               216


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 339 280
Items/sec median:    1 380 086
Items/sec max:       1 421 605

== Median report:
Items/sec:                       1380086
Mutex contentions/sec:            199421
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 0
Run count: 3
Summary:
* 1 966 659 items per second;
* x1.262 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 907 759
Items/sec median:    1 966 659
Items/sec max:       2 001 635

== Median report:
Items/sec:                       1966659
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 518 403
Items/sec median:    1 557 818
Items/sec max:       1 669 595

== Median report:
Items/sec:                       1557818
Mutex contentions/sec:            508889
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 1
Run count: 3
Summary:
* 1 950 883 items per second;
* x1.239 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 895 642
Items/sec median:    1 950 883
Items/sec max:       1 978 877

== Median report:
Items/sec:                       1950883
Mutex contentions/sec:               192


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 467 548
Items/sec median:    1 574 769
Items/sec max:       1 750 186

== Median report:
Items/sec:                       1574769
Mutex contentions/sec:            554234
```
</details>
