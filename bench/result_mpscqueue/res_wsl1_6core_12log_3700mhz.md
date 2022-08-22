**OS**: WSL 1 Linux 4.4.0-19041-Microsoft on Windows 10 Enterprise 19042<br>
**CPU**: Intel(R) Xeon(R) W-2135 CPU @ 3.70GHz, 3696 Mhz, 6 Core(s), 12 Logical Processor(s)

---
**Empty load, 1 sender thread, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 1 -items 15000000 -signal 0
Run count: 3
Summary:
* 13 295 662 items per second;
* x1.711 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      12 041 870
Items/sec median:   13 295 662
Items/sec max:      15 122 537

== Median report:
Items/sec:                      13295662
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       7 631 125
Items/sec median:    7 770 815
Items/sec max:       8 065 952

== Median report:
Items/sec:                       7770815
Mutex contentions/sec:           1372570
```
</details>

---
**Empty load, 1 sender thread, 35 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 1 -items 35000000 -signal 1
Run count: 3
Summary:
* 36 952 964 items per second;
* x2.758 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      35 181 416
Items/sec median:   36 952 964
Items/sec max:      38 859 424

== Median report:
Items/sec:                      36952964
Mutex contentions/sec:            136994


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:      13 360 571
Items/sec median:   13 396 676
Items/sec max:      15 282 672

== Median report:
Items/sec:                      13396676
Mutex contentions/sec:            244819
```
</details>

---
**Empty load, 2 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 2 -items 15000000 -signal 0
Run count: 3
Summary:
* 10 494 533 items per second;
* x1.053 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      10 481 061
Items/sec median:   10 494 533
Items/sec max:      10 942 647

== Median report:
Items/sec:                      10494533
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       9 871 094
Items/sec median:    9 965 151
Items/sec max:      10 320 381

== Median report:
Items/sec:                       9965151
Mutex contentions/sec:           1002271
```
</details>

---
**Empty load, 2 sender threads, 30 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 2 -items 30000000 -signal 1
Run count: 3
Summary:
* 34 426 741 items per second;
* x2.562 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      31 891 914
Items/sec median:   34 426 741
Items/sec max:      36 681 040

== Median report:
Items/sec:                      34426741
Mutex contentions/sec:            141647


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:      13 327 977
Items/sec median:   13 436 887
Items/sec max:      14 640 120

== Median report:
Items/sec:                      13436887
Mutex contentions/sec:            598828
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 0
Run count: 3
Summary:
* 11 294 288 items per second;
* x1.368 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      10 650 577
Items/sec median:   11 294 288
Items/sec max:      12 018 434

== Median report:
Items/sec:                      11294288
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       8 048 621
Items/sec median:    8 256 025
Items/sec max:       8 425 951

== Median report:
Items/sec:                       8256025
Mutex contentions/sec:           1146743
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 1
Run count: 3
Summary:
* 18 367 328 items per second;
* x1.618 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      18 180 101
Items/sec median:   18 367 328
Items/sec max:      21 851 847

== Median report:
Items/sec:                      18367328
Mutex contentions/sec:            128583


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:      10 850 703
Items/sec median:   11 353 071
Items/sec max:      11 470 897

== Median report:
Items/sec:                      11353071
Mutex contentions/sec:            951259
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 12 019 769 items per second;
* x1.730 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      11 571 944
Items/sec median:   12 019 769
Items/sec max:      12 949 623

== Median report:
Items/sec:                      12019769
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       6 622 298
Items/sec median:    6 946 917
Items/sec max:       7 078 691

== Median report:
Items/sec:                       6946917
Mutex contentions/sec:           1015046
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 1
Run count: 3
Summary:
* 15 223 307 items per second;
* x1.845 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      15 155 036
Items/sec median:   15 223 307
Items/sec max:      15 966 031

== Median report:
Items/sec:                      15223307
Mutex contentions/sec:             82153


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       8 132 674
Items/sec median:    8 249 828
Items/sec max:       8 981 831

== Median report:
Items/sec:                       8249828
Mutex contentions/sec:            767397
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 12 207 646 items per second;
* x2.317 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      11 864 696
Items/sec median:   12 207 646
Items/sec max:      12 799 691

== Median report:
Items/sec:                      12207646
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 174 979
Items/sec median:    5 269 082
Items/sec max:       5 558 804

== Median report:
Items/sec:                       5269082
Mutex contentions/sec:            899745
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 15 452 430 items per second;
* x2.600 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      13 759 202
Items/sec median:   15 452 430
Items/sec max:      15 589 981

== Median report:
Items/sec:                      15452430
Mutex contentions/sec:             12419


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 878 989
Items/sec median:    5 942 110
Items/sec max:       6 024 699

== Median report:
Items/sec:                       5942110
Mutex contentions/sec:            821436
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 0
Run count: 3
Summary:
* 2 170 663 items per second;
* x1.152 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 152 315
Items/sec median:    2 170 663
Items/sec max:       2 234 198

== Median report:
Items/sec:                       2170663
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 851 159
Items/sec median:    1 884 925
Items/sec max:       1 975 188

== Median report:
Items/sec:                       1884925
Mutex contentions/sec:            236874
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 962 420 items per second;
* x1.164 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 904 516
Items/sec median:    1 962 420
Items/sec max:       2 029 878

== Median report:
Items/sec:                       1962420
Mutex contentions/sec:             39060


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 652 484
Items/sec median:    1 685 436
Items/sec max:       1 741 450

== Median report:
Items/sec:                       1685436
Mutex contentions/sec:            110615
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 0
Run count: 3
Summary:
* 3 943 396 items per second;
* x1.319 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 914 086
Items/sec median:    3 943 396
Items/sec max:       3 966 163

== Median report:
Items/sec:                       3943396
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 966 286
Items/sec median:    2 989 895
Items/sec max:       3 029 180

== Median report:
Items/sec:                       2989895
Mutex contentions/sec:            693485
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 1
Run count: 3
Summary:
* 3 789 206 items per second;
* x1.383 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 705 924
Items/sec median:    3 789 206
Items/sec max:       3 953 184

== Median report:
Items/sec:                       3789206
Mutex contentions/sec:             71404


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 447 913
Items/sec median:    2 740 382
Items/sec max:       2 782 419

== Median report:
Items/sec:                       2740382
Mutex contentions/sec:            443545
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 0
Run count: 3
Summary:
* 5 555 864 items per second;
* x1.994 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 127 257
Items/sec median:    5 555 864
Items/sec max:       5 559 465

== Median report:
Items/sec:                       5555864
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 734 608
Items/sec median:    2 786 903
Items/sec max:       3 200 039

== Median report:
Items/sec:                       2786903
Mutex contentions/sec:            980239
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 1
Run count: 3
Summary:
* 5 214 825 items per second;
* x1.564 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 069 990
Items/sec median:    5 214 825
Items/sec max:       5 414 512

== Median report:
Items/sec:                       5214825
Mutex contentions/sec:             73519


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 158 590
Items/sec median:    3 334 914
Items/sec max:       3 367 472

== Median report:
Items/sec:                       3334914
Mutex contentions/sec:            771096
```
</details>

---
**Nano load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 7 758 071 items per second;
* x2.550 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 718 955
Items/sec median:    7 758 071
Items/sec max:       7 895 171

== Median report:
Items/sec:                       7758071
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 484 822
Items/sec median:    3 042 885
Items/sec max:       3 070 932

== Median report:
Items/sec:                       3042885
Mutex contentions/sec:           1150742
```
</details>

---
**Nano load, 5 sender threads, 13 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 5 -items 13000000 -signal 1
Run count: 3
Summary:
* 7 339 567 items per second;
* x2.560 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 205 621
Items/sec median:    7 339 567
Items/sec max:       7 719 329

== Median report:
Items/sec:                       7339567
Mutex contentions/sec:             40513


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 841 270
Items/sec median:    2 866 752
Items/sec max:       3 346 178

== Median report:
Items/sec:                       2866752
Mutex contentions/sec:            924395
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 9 850 847 items per second;
* x4.062 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 267 966
Items/sec median:    9 850 847
Items/sec max:      10 124 852

== Median report:
Items/sec:                       9850847
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 384 556
Items/sec median:    2 425 357
Items/sec max:       2 436 491

== Median report:
Items/sec:                       2425357
Mutex contentions/sec:            979920
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 10 150 186 items per second;
* x3.929 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      10 109 688
Items/sec median:   10 150 186
Items/sec max:      10 724 002

== Median report:
Items/sec:                      10150186
Mutex contentions/sec:              5062


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 471 544
Items/sec median:    2 583 205
Items/sec max:       2 939 602

== Median report:
Items/sec:                       2583205
Mutex contentions/sec:            924361
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 0
Run count: 3
Summary:
* 553 297 items per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         518 874
Items/sec median:      553 297
Items/sec max:         564 659

== Median report:
Items/sec:                        553297
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         528 705
Items/sec median:      551 840
Items/sec max:         553 504

== Median report:
Items/sec:                        551840
Mutex contentions/sec:             13404
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 1
Run count: 3
Summary:
* 550 902 items per second;
* x1.060 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         549 892
Items/sec median:      550 902
Items/sec max:         578 864

== Median report:
Items/sec:                        550902
Mutex contentions/sec:              5057


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         499 669
Items/sec median:      519 843
Items/sec max:         530 576

== Median report:
Items/sec:                        519843
Mutex contentions/sec:             14757
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 0
Run count: 3
Summary:
* 1 108 206 items per second;
* x1.053 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 093 804
Items/sec median:    1 108 206
Items/sec max:       1 139 909

== Median report:
Items/sec:                       1108206
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 049 961
Items/sec median:    1 052 537
Items/sec max:       1 065 690

== Median report:
Items/sec:                       1052537
Mutex contentions/sec:            102737
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 1
Run count: 3
Summary:
* 1 120 688 items per second;
* x1.075 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 079 749
Items/sec median:    1 120 688
Items/sec max:       1 140 253

== Median report:
Items/sec:                       1120688
Mutex contentions/sec:              5321


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 025 483
Items/sec median:    1 042 700
Items/sec max:       1 058 799

== Median report:
Items/sec:                       1042700
Mutex contentions/sec:             89965
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 0
Run count: 3
Summary:
* 1 526 733 items per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 519 153
Items/sec median:    1 526 733
Items/sec max:       1 549 500

== Median report:
Items/sec:                       1526733
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 506 092
Items/sec median:    1 513 857
Items/sec max:       1 517 871

== Median report:
Items/sec:                       1513857
Mutex contentions/sec:            229851
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 1
Run count: 3
Summary:
* 1 527 986 items per second;
* x1.062 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 522 558
Items/sec median:    1 527 986
Items/sec max:       1 560 618

== Median report:
Items/sec:                       1527986
Mutex contentions/sec:              3578


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 398 561
Items/sec median:    1 438 571
Items/sec max:       1 464 542

== Median report:
Items/sec:                       1438571
Mutex contentions/sec:            223931
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 0
Run count: 3
Summary:
* 2 402 663 items per second;
* x1.111 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 393 608
Items/sec median:    2 402 663
Items/sec max:       2 439 574

== Median report:
Items/sec:                       2402663
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 905 024
Items/sec median:    2 162 063
Items/sec max:       2 209 206

== Median report:
Items/sec:                       2162063
Mutex contentions/sec:            415983
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 1
Run count: 3
Summary:
* 2 329 843 items per second;
* x1.329 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 304 774
Items/sec median:    2 329 843
Items/sec max:       2 334 210

== Median report:
Items/sec:                       2329843
Mutex contentions/sec:               446


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 709 693
Items/sec median:    1 753 225
Items/sec max:       1 858 557

== Median report:
Items/sec:                       1753225
Mutex contentions/sec:            466441
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 0
Run count: 3
Summary:
* 2 986 761 items per second;
* x1.236 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 720 453
Items/sec median:    2 986 761
Items/sec max:       3 378 011

== Median report:
Items/sec:                       2986761
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 292 539
Items/sec median:    2 416 020
Items/sec max:       2 494 033

== Median report:
Items/sec:                       2416020
Mutex contentions/sec:            802169
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 1
Run count: 3
Summary:
* 3 365 989 items per second;
* x1.389 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 238 077
Items/sec median:    3 365 989
Items/sec max:       3 594 368

== Median report:
Items/sec:                       3365989
Mutex contentions/sec:               380


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 271 772
Items/sec median:    2 422 661
Items/sec max:       2 507 058

== Median report:
Items/sec:                       2422661
Mutex contentions/sec:            764530
```
</details>
