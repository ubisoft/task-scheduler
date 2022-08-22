**OS**: macOS Catalina 10.15.5<br>
**CPU**: Quad-Core Intel Core i7 2.5 GHz, 4 cores, hyper-threading enabled

---
**Empty load, 1 sender thread, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 1 -items 15000000 -signal 0
Run count: 3
Summary:
* 8 962 207 items per second;
* x1.656 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       8 876 134
Items/sec median:    8 962 207
Items/sec max:       9 035 425

== Median report:
Items/sec:                       8962207
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 259 060
Items/sec median:    5 412 070
Items/sec max:       5 647 526

== Median report:
Items/sec:                       5412070
Mutex contentions/sec:             60496
```
</details>

---
**Empty load, 1 sender thread, 35 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 1 -items 35000000 -signal 1
Run count: 3
Summary:
* 25 954 432 items per second;
* x3.520 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      25 701 492
Items/sec median:   25 954 432
Items/sec max:      26 555 481

== Median report:
Items/sec:                      25954432
Mutex contentions/sec:            140592


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       7 314 514
Items/sec median:    7 373 172
Items/sec max:       7 378 610

== Median report:
Items/sec:                       7373172
Mutex contentions/sec:            116903
```
</details>

---
**Empty load, 2 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 2 -items 15000000 -signal 0
Run count: 3
Summary:
* 7 813 640 items per second;
* x1.455 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 658 739
Items/sec median:    7 813 640
Items/sec max:       8 113 025

== Median report:
Items/sec:                       7813640
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 363 281
Items/sec median:    5 369 124
Items/sec max:       5 473 121

== Median report:
Items/sec:                       5369124
Mutex contentions/sec:            449593
```
</details>

---
**Empty load, 2 sender threads, 30 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 2 -items 30000000 -signal 1
Run count: 3
Summary:
* 39 831 899 items per second;
* x12.176 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      39 259 063
Items/sec median:   39 831 899
Items/sec max:      40 813 591

== Median report:
Items/sec:                      39831899
Mutex contentions/sec:            163926


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 127 499
Items/sec median:    3 271 308
Items/sec max:       3 413 108

== Median report:
Items/sec:                       3271308
Mutex contentions/sec:            201855
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 0
Run count: 3
Summary:
* 6 094 708 items per second;
* x1.112 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 973 602
Items/sec median:    6 094 708
Items/sec max:       6 294 799

== Median report:
Items/sec:                       6094708
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 446 462
Items/sec median:    5 483 020
Items/sec max:       5 510 904

== Median report:
Items/sec:                       5483020
Mutex contentions/sec:            385926
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 1
Run count: 3
Summary:
* 11 786 758 items per second;
* x1.742 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      11 091 275
Items/sec median:   11 786 758
Items/sec max:      12 972 006

== Median report:
Items/sec:                      11786758
Mutex contentions/sec:            157727


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       6 666 335
Items/sec median:    6 768 083
Items/sec max:       6 823 039

== Median report:
Items/sec:                       6768083
Mutex contentions/sec:            383265
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 506 200 items per second;
* x1.181 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 457 996
Items/sec median:    5 506 200
Items/sec max:       5 543 512

== Median report:
Items/sec:                       5506200
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 658 244
Items/sec median:    4 663 334
Items/sec max:       4 721 054

== Median report:
Items/sec:                       4663334
Mutex contentions/sec:            438421
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 1
Run count: 3
Summary:
* 8 056 454 items per second;
* x1.699 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 937 319
Items/sec median:    8 056 454
Items/sec max:       9 055 386

== Median report:
Items/sec:                       8056454
Mutex contentions/sec:            110547


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 643 271
Items/sec median:    4 742 253
Items/sec max:       4 929 957

== Median report:
Items/sec:                       4742253
Mutex contentions/sec:            394717
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 4 859 641 items per second;
* x1.024 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 847 779
Items/sec median:    4 859 641
Items/sec max:       5 473 049

== Median report:
Items/sec:                       4859641
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 740 226
Items/sec median:    4 744 328
Items/sec max:       4 772 621

== Median report:
Items/sec:                       4744328
Mutex contentions/sec:            517450
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 5 941 101 items per second;
* x1.261 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 896 940
Items/sec median:    5 941 101
Items/sec max:       6 136 980

== Median report:
Items/sec:                       5941101
Mutex contentions/sec:              4272


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 632 920
Items/sec median:    4 711 013
Items/sec max:       4 748 328

== Median report:
Items/sec:                       4711013
Mutex contentions/sec:            517038
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 0
Run count: 3
Summary:
* 1 912 207 items per second;
* x1.316 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 911 011
Items/sec median:    1 912 207
Items/sec max:       1 921 425

== Median report:
Items/sec:                       1912207
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 443 873
Items/sec median:    1 452 969
Items/sec max:       1 457 036

== Median report:
Items/sec:                       1452969
Mutex contentions/sec:            224152
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 196 530 items per second;
* x1.192 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 195 742
Items/sec median:    1 196 530
Items/sec max:       1 211 060

== Median report:
Items/sec:                       1196530
Mutex contentions/sec:             96140


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         997 908
Items/sec median:    1 003 522
Items/sec max:       1 045 396

== Median report:
Items/sec:                       1003522
Mutex contentions/sec:            131156
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 0
Run count: 3
Summary:
* 3 657 807 items per second;
* x1.587 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 405 432
Items/sec median:    3 657 807
Items/sec max:       3 794 994

== Median report:
Items/sec:                       3657807
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 178 520
Items/sec median:    2 304 379
Items/sec max:       2 347 277

== Median report:
Items/sec:                       2304379
Mutex contentions/sec:            405448
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 1
Run count: 3
Summary:
* 3 133 524 items per second;
* x1.622 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 111 395
Items/sec median:    3 133 524
Items/sec max:       3 169 968

== Median report:
Items/sec:                       3133524
Mutex contentions/sec:             81695


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 858 875
Items/sec median:    1 931 664
Items/sec max:       1 942 431

== Median report:
Items/sec:                       1931664
Mutex contentions/sec:            335934
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 0
Run count: 3
Summary:
* 4 747 977 items per second;
* x2.429 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 740 214
Items/sec median:    4 747 977
Items/sec max:       4 803 097

== Median report:
Items/sec:                       4747977
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 935 771
Items/sec median:    1 955 071
Items/sec max:       1 964 165

== Median report:
Items/sec:                       1955071
Mutex contentions/sec:            489991
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 1
Run count: 3
Summary:
* 4 110 172 items per second;
* x2.154 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 074 996
Items/sec median:    4 110 172
Items/sec max:       4 137 613

== Median report:
Items/sec:                       4110172
Mutex contentions/sec:             53940


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 863 916
Items/sec median:    1 908 579
Items/sec max:       1 969 716

== Median report:
Items/sec:                       1908579
Mutex contentions/sec:            374383
```
</details>

---
**Nano load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 623 481 items per second;
* x3.551 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 618 177
Items/sec median:    5 623 481
Items/sec max:       5 704 972

== Median report:
Items/sec:                       5623481
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 537 585
Items/sec median:    1 583 651
Items/sec max:       1 597 334

== Median report:
Items/sec:                       1583651
Mutex contentions/sec:            477558
```
</details>

---
**Nano load, 5 sender threads, 13 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 5 -items 13000000 -signal 1
Run count: 3
Summary:
* 5 804 000 items per second;
* x3.438 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 799 370
Items/sec median:    5 804 000
Items/sec max:       5 858 680

== Median report:
Items/sec:                       5804000
Mutex contentions/sec:               731


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 664 335
Items/sec median:    1 688 081
Items/sec max:       1 694 182

== Median report:
Items/sec:                       1688081
Mutex contentions/sec:            441660
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 7 344 674 items per second;
* x3.820 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 167 824
Items/sec median:    7 344 674
Items/sec max:       7 384 562

== Median report:
Items/sec:                       7344674
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 877 455
Items/sec median:    1 922 611
Items/sec max:       1 938 450

== Median report:
Items/sec:                       1922611
Mutex contentions/sec:            623066
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 6 915 855 items per second;
* x3.665 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       6 679 731
Items/sec median:    6 915 855
Items/sec max:       6 950 961

== Median report:
Items/sec:                       6915855
Mutex contentions/sec:               343


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 871 446
Items/sec median:    1 887 214
Items/sec max:       1 912 523

== Median report:
Items/sec:                       1887214
Mutex contentions/sec:            601645
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 0
Run count: 3
Summary:
* 446 793 items per second;
* x1.044 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         443 088
Items/sec median:      446 793
Items/sec max:         448 986

== Median report:
Items/sec:                        446793
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         428 007
Items/sec median:      428 010
Items/sec max:         433 349

== Median report:
Items/sec:                        428010
Mutex contentions/sec:             30417
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 1
Run count: 3
Summary:
* 398 330 items per second;
* x1.038 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         392 922
Items/sec median:      398 330
Items/sec max:         402 262

== Median report:
Items/sec:                        398330
Mutex contentions/sec:             15285


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         383 099
Items/sec median:      383 640
Items/sec max:         389 109

== Median report:
Items/sec:                        383640
Mutex contentions/sec:             16264
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 0
Run count: 3
Summary:
* 867 092 items per second;
* x1.057 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         859 653
Items/sec median:      867 092
Items/sec max:         872 267

== Median report:
Items/sec:                        867092
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         811 599
Items/sec median:      820 339
Items/sec max:         834 015

== Median report:
Items/sec:                        820339
Mutex contentions/sec:             35789
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 1
Run count: 3
Summary:
* 831 070 items per second;
* x1.066 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         828 650
Items/sec median:      831 070
Items/sec max:         831 289

== Median report:
Items/sec:                        831070
Mutex contentions/sec:              4893


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         770 622
Items/sec median:      779 554
Items/sec max:         783 453

== Median report:
Items/sec:                        779554
Mutex contentions/sec:             64536
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 0
Run count: 3
Summary:
* 1 192 275 items per second;
* x1.074 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 168 162
Items/sec median:    1 192 275
Items/sec max:       1 195 193

== Median report:
Items/sec:                       1192275
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 109 136
Items/sec median:    1 110 598
Items/sec max:       1 111 633

== Median report:
Items/sec:                       1110598
Mutex contentions/sec:             68454
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 1
Run count: 3
Summary:
* 1 084 367 items per second;
* x1.081 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         989 383
Items/sec median:    1 084 367
Items/sec max:       1 115 983

== Median report:
Items/sec:                       1084367
Mutex contentions/sec:              3431


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         994 472
Items/sec median:    1 002 708
Items/sec max:       1 002 764

== Median report:
Items/sec:                       1002708
Mutex contentions/sec:             79486
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 0
Run count: 3
Summary:
* 1 389 746 items per second;
* x1.217 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 368 410
Items/sec median:    1 389 746
Items/sec max:       1 409 650

== Median report:
Items/sec:                       1389746
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 133 036
Items/sec median:    1 142 340
Items/sec max:       1 159 250

== Median report:
Items/sec:                       1142340
Mutex contentions/sec:            156846
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 377 680 items per second;
* x1.176 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 367 500
Items/sec median:    1 377 680
Items/sec max:       1 391 973

== Median report:
Items/sec:                       1377680
Mutex contentions/sec:               104


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 120 259
Items/sec median:    1 171 106
Items/sec max:       1 173 010

== Median report:
Items/sec:                       1171106
Mutex contentions/sec:            158911
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 0
Run count: 3
Summary:
* 1 587 438 items per second;
* x1.064 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 584 852
Items/sec median:    1 587 438
Items/sec max:       1 595 760

== Median report:
Items/sec:                       1587438
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 480 310
Items/sec median:    1 491 874
Items/sec max:       1 493 985

== Median report:
Items/sec:                       1491874
Mutex contentions/sec:            134227
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 1
Run count: 3
Summary:
* 1 574 280 items per second;
* x1.102 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 569 781
Items/sec median:    1 574 280
Items/sec max:       1 585 773

== Median report:
Items/sec:                       1574280
Mutex contentions/sec:                38


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 408 175
Items/sec median:    1 428 416
Items/sec max:       1 447 082

== Median report:
Items/sec:                       1428416
Mutex contentions/sec:            164677
```
</details>
