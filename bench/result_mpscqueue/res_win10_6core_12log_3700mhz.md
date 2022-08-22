**OS**: Windows 10 Enterprise 19042<br>
**CPU**: Intel(R) Xeon(R) W-2135 CPU @ 3.70GHz, 3696 Mhz, 6 Core(s), 12 Logical Processor(s)

---
**Empty load, 1 sender thread, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 1 -items 15000000 -signal 0
Run count: 3
Summary:
* 10 224 048 items per second;
* x2.599 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      10 170 414
Items/sec median:   10 224 048
Items/sec max:      10 300 845

== Median report:
Items/sec:                      10224048
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 906 349
Items/sec median:    3 934 130
Items/sec max:       3 970 337

== Median report:
Items/sec:                       3934130
Mutex contentions/sec:           2436290
```
</details>

---
**Empty load, 1 sender thread, 35 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 1 -items 35000000 -signal 1
Run count: 3
Summary:
* 6 371 324 items per second;
* x1.503 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 547 738
Items/sec median:    6 371 324
Items/sec max:       7 350 528

== Median report:
Items/sec:                       6371324
Mutex contentions/sec:           1161040


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 878 722
Items/sec median:    4 240 199
Items/sec max:       4 241 352

== Median report:
Items/sec:                       4240199
Mutex contentions/sec:           2064275
```
</details>

---
**Empty load, 2 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 2 -items 15000000 -signal 0
Run count: 3
Summary:
* 11 060 833 items per second;
* x4.137 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      10 708 330
Items/sec median:   11 060 833
Items/sec max:      11 164 700

== Median report:
Items/sec:                      11060833
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 613 929
Items/sec median:    2 673 536
Items/sec max:       2 675 985

== Median report:
Items/sec:                       2673536
Mutex contentions/sec:           2465841
```
</details>

---
**Empty load, 2 sender threads, 30 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 2 -items 30000000 -signal 1
Run count: 3
Summary:
* 9 825 200 items per second;
* x2.667 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 351 317
Items/sec median:    9 825 200
Items/sec max:      11 341 947

== Median report:
Items/sec:                       9825200
Mutex contentions/sec:            295132


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 304 580
Items/sec median:    3 683 924
Items/sec max:       4 446 478

== Median report:
Items/sec:                       3683924
Mutex contentions/sec:           1773013
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 0
Run count: 3
Summary:
* 10 192 716 items per second;
* x3.702 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 770 145
Items/sec median:   10 192 716
Items/sec max:      10 929 308

== Median report:
Items/sec:                      10192716
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 542 313
Items/sec median:    2 753 579
Items/sec max:       2 760 324

== Median report:
Items/sec:                       2753579
Mutex contentions/sec:           2191532
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 1
Run count: 3
Summary:
* 9 764 977 items per second;
* x3.511 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 157 550
Items/sec median:    9 764 977
Items/sec max:      10 180 965

== Median report:
Items/sec:                       9764977
Mutex contentions/sec:            291985


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 510 777
Items/sec median:    2 781 298
Items/sec max:       3 172 360

== Median report:
Items/sec:                       2781298
Mutex contentions/sec:           1769226
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 10 034 896 items per second;
* x5.042 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 702 250
Items/sec median:   10 034 896
Items/sec max:      10 099 072

== Median report:
Items/sec:                      10034896
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 719 869
Items/sec median:    1 990 231
Items/sec max:       2 463 135

== Median report:
Items/sec:                       1990231
Mutex contentions/sec:           1314046
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 1
Run count: 3
Summary:
* 10 100 352 items per second;
* x5.020 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 105 406
Items/sec median:   10 100 352
Items/sec max:      10 710 031

== Median report:
Items/sec:                      10100352
Mutex contentions/sec:            169007


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 743 165
Items/sec median:    2 011 896
Items/sec max:       2 032 650

== Median report:
Items/sec:                       2011896
Mutex contentions/sec:           1339838
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 9 543 444 items per second;
* x4.959 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 348 369
Items/sec median:    9 543 444
Items/sec max:       9 971 390

== Median report:
Items/sec:                       9543444
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 579 978
Items/sec median:    1 924 546
Items/sec max:       1 991 336

== Median report:
Items/sec:                       1924546
Mutex contentions/sec:           1565682
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 9 650 217 items per second;
* x5.843 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       9 183 619
Items/sec median:    9 650 217
Items/sec max:       9 897 729

== Median report:
Items/sec:                       9650217
Mutex contentions/sec:            106962


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 375 334
Items/sec median:    1 651 544
Items/sec max:       1 775 401

== Median report:
Items/sec:                       1651544
Mutex contentions/sec:           1250542
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 0
Run count: 3
Summary:
* 2 167 907 items per second;
* x1.030 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 074 069
Items/sec median:    2 167 907
Items/sec max:       2 187 172

== Median report:
Items/sec:                       2167907
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 031 535
Items/sec median:    2 105 593
Items/sec max:       2 157 768

== Median report:
Items/sec:                       2105593
Mutex contentions/sec:            308906
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 954 045 items per second;
* x1.244 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 916 161
Items/sec median:    1 954 045
Items/sec max:       2 030 809

== Median report:
Items/sec:                       1954045
Mutex contentions/sec:            106938


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 547 135
Items/sec median:    1 570 942
Items/sec max:       1 842 627

== Median report:
Items/sec:                       1570942
Mutex contentions/sec:            399609
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 0
Run count: 3
Summary:
* 2 267 320 items per second;
* x1.225 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 801 932
Items/sec median:    2 267 320
Items/sec max:       2 330 547

== Median report:
Items/sec:                       2267320
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 791 974
Items/sec median:    1 850 784
Items/sec max:       1 883 926

== Median report:
Items/sec:                       1850784
Mutex contentions/sec:            518334
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 1
Run count: 3
Summary:
* 2 363 134 items per second;
* x1.222 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 179 249
Items/sec median:    2 363 134
Items/sec max:       2 479 916

== Median report:
Items/sec:                       2363134
Mutex contentions/sec:             22271


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 746 268
Items/sec median:    1 933 579
Items/sec max:       2 036 699

== Median report:
Items/sec:                       1933579
Mutex contentions/sec:            610463
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 0
Run count: 3
Summary:
* 3 222 561 items per second;
* x1.413 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 890 596
Items/sec median:    3 222 561
Items/sec max:       3 610 428

== Median report:
Items/sec:                       3222561
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 169 370
Items/sec median:    2 281 322
Items/sec max:       2 285 802

== Median report:
Items/sec:                       2281322
Mutex contentions/sec:           1485227
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 1
Run count: 3
Summary:
* 3 094 360 items per second;
* x1.185 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 548 820
Items/sec median:    3 094 360
Items/sec max:       3 426 306

== Median report:
Items/sec:                       3094360
Mutex contentions/sec:             34536


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 309 628
Items/sec median:    2 611 856
Items/sec max:       2 870 076

== Median report:
Items/sec:                       2611856
Mutex contentions/sec:           1453121
```
</details>

---
**Nano load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 4 636 454 items per second;
* x1.701 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 565 885
Items/sec median:    4 636 454
Items/sec max:       5 238 564

== Median report:
Items/sec:                       4636454
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 271 031
Items/sec median:    2 726 451
Items/sec max:       2 762 251

== Median report:
Items/sec:                       2726451
Mutex contentions/sec:           2124635
```
</details>

---
**Nano load, 5 sender threads, 13 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 5 -items 13000000 -signal 1
Run count: 3
Summary:
* 5 067 593 items per second;
* x1.886 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       4 805 945
Items/sec median:    5 067 593
Items/sec max:       5 159 829

== Median report:
Items/sec:                       5067593
Mutex contentions/sec:             25886


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 038 144
Items/sec median:    2 686 458
Items/sec max:       2 792 683

== Median report:
Items/sec:                       2686458
Mutex contentions/sec:           2177480
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 8 218 350 items per second;
* x4.362 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 999 954
Items/sec median:    8 218 350
Items/sec max:       9 730 660

== Median report:
Items/sec:                       8218350
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 714 578
Items/sec median:    1 884 093
Items/sec max:       1 953 561

== Median report:
Items/sec:                       1884093
Mutex contentions/sec:           1689084
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 6 951 929 items per second;
* x4.264 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       6 763 928
Items/sec median:    6 951 929
Items/sec max:       7 687 931

== Median report:
Items/sec:                       6951929
Mutex contentions/sec:             16501


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 539 260
Items/sec median:    1 630 386
Items/sec max:       1 913 850

== Median report:
Items/sec:                       1630386
Mutex contentions/sec:           1477967
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 0
Run count: 3
Summary:
* 577 616 items per second;
* x1.090 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         510 420
Items/sec median:      577 616
Items/sec max:         593 114

== Median report:
Items/sec:                        577616
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         207 260
Items/sec median:      529 865
Items/sec max:         546 793

== Median report:
Items/sec:                        529865
Mutex contentions/sec:             17441
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 1
Run count: 3
Summary:
* 250 726 items per second;
* x0.483 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         230 252
Items/sec median:      250 726
Items/sec max:         551 843

== Median report:
Items/sec:                        250726
Mutex contentions/sec:              5435


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         223 937
Items/sec median:      518 796
Items/sec max:         567 854

== Median report:
Items/sec:                        518796
Mutex contentions/sec:             45619
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 0
Run count: 3
Summary:
* 463 255 items per second;
* x1.011 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         455 791
Items/sec median:      463 255
Items/sec max:         467 083

== Median report:
Items/sec:                        463255
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         411 144
Items/sec median:      458 217
Items/sec max:       1 025 014

== Median report:
Items/sec:                        458217
Mutex contentions/sec:             20238
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 1
Run count: 3
Summary:
* 475 314 items per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         457 621
Items/sec median:      475 314
Items/sec max:         499 520

== Median report:
Items/sec:                        475314
Mutex contentions/sec:              8673


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         438 257
Items/sec median:      478 774
Items/sec max:         482 120

== Median report:
Items/sec:                        478774
Mutex contentions/sec:             34310
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 0
Run count: 3
Summary:
* 707 291 items per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         552 411
Items/sec median:      707 291
Items/sec max:         773 508

== Median report:
Items/sec:                        707291
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         660 729
Items/sec median:      709 524
Items/sec max:         716 058

== Median report:
Items/sec:                        709524
Mutex contentions/sec:             87652
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 1
Run count: 3
Summary:
* 703 462 items per second;
* x1.066 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         690 910
Items/sec median:      703 462
Items/sec max:         707 240

== Median report:
Items/sec:                        703462
Mutex contentions/sec:              1357


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         561 264
Items/sec median:      660 042
Items/sec max:         699 193

== Median report:
Items/sec:                        660042
Mutex contentions/sec:             64475
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 0
Run count: 3
Summary:
* 1 158 732 items per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         967 149
Items/sec median:    1 158 732
Items/sec max:       1 166 351

== Median report:
Items/sec:                       1158732
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 125 910
Items/sec median:    1 152 051
Items/sec max:       1 160 388

== Median report:
Items/sec:                       1152051
Mutex contentions/sec:            145607
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 139 332 items per second;
* x1.201 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 138 935
Items/sec median:    1 139 332
Items/sec max:       1 151 434

== Median report:
Items/sec:                       1139332
Mutex contentions/sec:               342


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         881 375
Items/sec median:      948 270
Items/sec max:       1 155 496

== Median report:
Items/sec:                        948270
Mutex contentions/sec:            107679
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 0
Run count: 3
Summary:
* 2 038 612 items per second;
* x1.178 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 847 114
Items/sec median:    2 038 612
Items/sec max:       2 255 886

== Median report:
Items/sec:                       2038612
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 700 097
Items/sec median:    1 730 632
Items/sec max:       2 935 081

== Median report:
Items/sec:                       1730632
Mutex contentions/sec:            618226
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 1
Run count: 3
Summary:
* 1 841 858 items per second;
* x0.971 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 829 620
Items/sec median:    1 841 858
Items/sec max:       1 942 793

== Median report:
Items/sec:                       1841858
Mutex contentions/sec:              3535


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 830 473
Items/sec median:    1 895 948
Items/sec max:       2 213 392

== Median report:
Items/sec:                       1895948
Mutex contentions/sec:            651828
```
</details>
