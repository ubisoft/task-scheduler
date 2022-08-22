**OS**: Debian 5.10.136-1<br>
**CPU**: Intel(R) Xeon(R) CPU @ 2.30GHz, 8 CPUs, 2 threads per core

---
**Empty load, 1 sender thread, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 1 -items 15000000 -signal 0
Run count: 3
Summary:
* 7 705 728 items per second;
* x1.440 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       7 591 033
Items/sec median:    7 705 728
Items/sec max:       8 847 535

== Median report:
Items/sec:                       7705728
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 271 907
Items/sec median:    5 351 487
Items/sec max:       5 393 516

== Median report:
Items/sec:                       5351487
Mutex contentions/sec:            738021
```
</details>

---
**Empty load, 1 sender thread, 35 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 1 -items 35000000 -signal 1
Run count: 3
Summary:
* 15 613 559 items per second;
* x2.006 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      14 847 523
Items/sec median:   15 613 559
Items/sec max:      18 627 582

== Median report:
Items/sec:                      15613559
Mutex contentions/sec:            220628


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       7 714 601
Items/sec median:    7 782 624
Items/sec max:       7 859 746

== Median report:
Items/sec:                       7782624
Mutex contentions/sec:            341299
```
</details>

---
**Empty load, 2 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 2 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 928 587 items per second;
* almost same as trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 442 838
Items/sec median:    5 928 587
Items/sec max:       6 317 967

== Median report:
Items/sec:                       5928587
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       5 852 244
Items/sec median:    5 892 986
Items/sec max:       5 985 714

== Median report:
Items/sec:                       5892986
Mutex contentions/sec:            852121
```
</details>

---
**Empty load, 2 sender threads, 30 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 2 -items 30000000 -signal 1
Run count: 3
Summary:
* 20 890 617 items per second;
* x1.682 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:      15 894 260
Items/sec median:   20 890 617
Items/sec max:      22 735 405

== Median report:
Items/sec:                      20890617
Mutex contentions/sec:            124143


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:      12 285 673
Items/sec median:   12 423 663
Items/sec max:      13 130 110

== Median report:
Items/sec:                      12423663
Mutex contentions/sec:            483314
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 781 008 items per second;
* x1.091 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 513 685
Items/sec median:    5 781 008
Items/sec max:       7 052 695

== Median report:
Items/sec:                       5781008
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 919 053
Items/sec median:    5 299 312
Items/sec max:       5 394 572

== Median report:
Items/sec:                       5299312
Mutex contentions/sec:            985709
```
</details>

---
**Empty load, 3 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 3 -items 15000000 -signal 1
Run count: 3
Summary:
* 8 996 752 items per second;
* x1.292 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       8 691 336
Items/sec median:    8 996 752
Items/sec max:       9 595 206

== Median report:
Items/sec:                       8996752
Mutex contentions/sec:            107656


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       6 790 814
Items/sec median:    6 965 736
Items/sec max:       7 169 081

== Median report:
Items/sec:                       6965736
Mutex contentions/sec:            807992
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 6 228 605 items per second;
* x1.392 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 834 526
Items/sec median:    6 228 605
Items/sec max:       6 427 870

== Median report:
Items/sec:                       6228605
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 280 285
Items/sec median:    4 475 813
Items/sec max:       4 507 992

== Median report:
Items/sec:                       4475813
Mutex contentions/sec:           1030822
```
</details>

---
**Empty load, 5 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 5 -items 15000000 -signal 1
Run count: 3
Summary:
* 8 701 576 items per second;
* x1.546 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       8 636 154
Items/sec median:    8 701 576
Items/sec max:       9 000 168

== Median report:
Items/sec:                       8701576
Mutex contentions/sec:            122598


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       4 501 446
Items/sec median:    5 628 837
Items/sec max:       5 927 816

== Median report:
Items/sec:                       5628837
Mutex contentions/sec:           1026223
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 7 162 857 items per second;
* x1.755 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       6 609 564
Items/sec median:    7 162 857
Items/sec max:       7 617 793

== Median report:
Items/sec:                       7162857
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 604 450
Items/sec median:    4 081 335
Items/sec max:       4 171 728

== Median report:
Items/sec:                       4081335
Mutex contentions/sec:           1052112
```
</details>

---
**Empty load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load empty -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 8 764 427 items per second;
* x2.093 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       8 400 280
Items/sec median:    8 764 427
Items/sec max:       9 015 952

== Median report:
Items/sec:                       8764427
Mutex contentions/sec:             22996


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       3 923 682
Items/sec median:    4 188 200
Items/sec max:       4 482 165

== Median report:
Items/sec:                       4188200
Mutex contentions/sec:            998054
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 0
Run count: 3
Summary:
* 1 215 601 items per second;
* x1.051 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 210 569
Items/sec median:    1 215 601
Items/sec max:       1 227 493

== Median report:
Items/sec:                       1215601
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 147 960
Items/sec median:    1 157 157
Items/sec max:       1 166 127

== Median report:
Items/sec:                       1157157
Mutex contentions/sec:             64911
```
</details>

---
**Nano load, 1 sender thread, 5 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 1 -items 5000000 -signal 1
Run count: 3
Summary:
* 977 190 items per second;
* x1.231 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         962 433
Items/sec median:      977 190
Items/sec max:         981 676

== Median report:
Items/sec:                        977190
Mutex contentions/sec:             23579


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         793 378
Items/sec median:      793 675
Items/sec max:         799 358

== Median report:
Items/sec:                        793675
Mutex contentions/sec:             76672
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 0
Run count: 3
Summary:
* 2 333 474 items per second;
* x1.242 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 306 168
Items/sec median:    2 333 474
Items/sec max:       2 346 250

== Median report:
Items/sec:                       2333474
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 777 391
Items/sec median:    1 879 122
Items/sec max:       2 093 917

== Median report:
Items/sec:                       1879122
Mutex contentions/sec:            379614
```
</details>

---
**Nano load, 2 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 2 -items 10000000 -signal 1
Run count: 3
Summary:
* 2 067 365 items per second;
* x1.260 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       2 001 506
Items/sec median:    2 067 365
Items/sec max:       2 076 801

== Median report:
Items/sec:                       2067365
Mutex contentions/sec:             95530


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 579 687
Items/sec median:    1 640 149
Items/sec max:       1 641 640

== Median report:
Items/sec:                       1640149
Mutex contentions/sec:            301854
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 0
Run count: 3
Summary:
* 3 379 849 items per second;
* x1.545 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 139 664
Items/sec median:    3 379 849
Items/sec max:       3 383 055

== Median report:
Items/sec:                       3379849
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 974 220
Items/sec median:    2 187 762
Items/sec max:       2 449 609

== Median report:
Items/sec:                       2187762
Mutex contentions/sec:            732295
```
</details>

---
**Nano load, 3 sender threads, 12 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 3 -items 12000000 -signal 1
Run count: 3
Summary:
* 3 092 279 items per second;
* x1.466 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 027 160
Items/sec median:    3 092 279
Items/sec max:       3 093 512

== Median report:
Items/sec:                       3092279
Mutex contentions/sec:             54642


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 086 775
Items/sec median:    2 109 325
Items/sec max:       2 180 805

== Median report:
Items/sec:                       2109325
Mutex contentions/sec:            527917
```
</details>

---
**Nano load, 5 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 5 -items 15000000 -signal 0
Run count: 3
Summary:
* 4 000 720 items per second;
* x1.859 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 704 418
Items/sec median:    4 000 720
Items/sec max:       4 462 767

== Median report:
Items/sec:                       4000720
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 003 476
Items/sec median:    2 151 963
Items/sec max:       2 255 181

== Median report:
Items/sec:                       2151963
Mutex contentions/sec:            907122
```
</details>

---
**Nano load, 5 sender threads, 13 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 5 -items 13000000 -signal 1
Run count: 3
Summary:
* 3 945 121 items per second;
* x1.761 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       3 763 295
Items/sec median:    3 945 121
Items/sec max:       3 987 377

== Median report:
Items/sec:                       3945121
Mutex contentions/sec:             10529


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 130 697
Items/sec median:    2 240 372
Items/sec max:       2 365 803

== Median report:
Items/sec:                       2240372
Mutex contentions/sec:            791417
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, busy-loop receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 0
Run count: 3
Summary:
* 5 791 610 items per second;
* x2.766 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 781 360
Items/sec median:    5 791 610
Items/sec max:       5 944 651

== Median report:
Items/sec:                       5791610
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 064 555
Items/sec median:    2 093 734
Items/sec max:       2 178 468

== Median report:
Items/sec:                       2093734
Mutex contentions/sec:            997654
```
</details>

---
**Nano load, 10 sender threads, 15 000 000 items, blocking receiver**

```
Command line args: -load nano -senders 10 -items 15000000 -signal 1
Run count: 3
Summary:
* 5 871 803 items per second;
* x2.622 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       5 781 438
Items/sec median:    5 871 803
Items/sec max:       5 901 123

== Median report:
Items/sec:                       5871803
Mutex contentions/sec:              2834


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       2 215 301
Items/sec median:    2 239 533
Items/sec max:       2 496 720

== Median report:
Items/sec:                       2239533
Mutex contentions/sec:            969370
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 0
Run count: 3
Summary:
* 294 079 items per second;
* x1.047 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         293 404
Items/sec median:      294 079
Items/sec max:         294 445

== Median report:
Items/sec:                        294079
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         280 640
Items/sec median:      280 826
Items/sec max:         281 359

== Median report:
Items/sec:                        280826
Mutex contentions/sec:              5082
```
</details>

---
**Micro load, 1 sender thread, 1 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 1 -items 1000000 -signal 1
Run count: 3
Summary:
* 266 586 items per second;
* x1.017 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         266 358
Items/sec median:      266 586
Items/sec max:         268 218

== Median report:
Items/sec:                        266586
Mutex contentions/sec:             11013


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         247 216
Items/sec median:      262 096
Items/sec max:         262 105

== Median report:
Items/sec:                        262096
Mutex contentions/sec:              3769
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 0
Run count: 3
Summary:
* 581 801 items per second;
* x1.055 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         571 851
Items/sec median:      581 801
Items/sec max:         586 223

== Median report:
Items/sec:                        581801
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         545 139
Items/sec median:      551 483
Items/sec max:         556 775

== Median report:
Items/sec:                        551483
Mutex contentions/sec:             24080
```
</details>

---
**Micro load, 2 sender threads, 2 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 2 -items 2000000 -signal 1
Run count: 3
Summary:
* 565 424 items per second;
* x1.047 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         565 239
Items/sec median:      565 424
Items/sec max:         570 112

== Median report:
Items/sec:                        565424
Mutex contentions/sec:              5054


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         538 517
Items/sec median:      540 130
Items/sec max:         540 833

== Median report:
Items/sec:                        540130
Mutex contentions/sec:             26153
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 0
Run count: 3
Summary:
* 873 549 items per second;
* x1.061 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         872 133
Items/sec median:      873 549
Items/sec max:         874 591

== Median report:
Items/sec:                        873549
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         821 780
Items/sec median:      823 012
Items/sec max:         824 428

== Median report:
Items/sec:                        823012
Mutex contentions/sec:             36287
```
</details>

---
**Micro load, 3 sender threads, 3 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 3 -items 3000000 -signal 1
Run count: 3
Summary:
* 852 746 items per second;
* x1.052 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:         843 735
Items/sec median:      852 746
Items/sec max:         853 563

== Median report:
Items/sec:                        852746
Mutex contentions/sec:               896


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         807 500
Items/sec median:      810 608
Items/sec max:         813 109

== Median report:
Items/sec:                        810608
Mutex contentions/sec:             59737
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 0
Run count: 3
Summary:
* 1 116 437 items per second;
* x1.103 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 050 910
Items/sec median:    1 116 437
Items/sec max:       1 125 720

== Median report:
Items/sec:                       1116437
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         987 485
Items/sec median:    1 012 479
Items/sec max:       1 047 032

== Median report:
Items/sec:                       1012479
Mutex contentions/sec:            134985
```
</details>

---
**Micro load, 5 sender threads, 5 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 5 -items 5000000 -signal 1
Run count: 3
Summary:
* 1 100 970 items per second;
* x1.110 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 031 678
Items/sec median:    1 100 970
Items/sec max:       1 105 803

== Median report:
Items/sec:                       1100970
Mutex contentions/sec:               260


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:         954 743
Items/sec median:      991 445
Items/sec max:       1 034 963

== Median report:
Items/sec:                        991445
Mutex contentions/sec:            154006
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, busy-loop receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 0
Run count: 3
Summary:
* 1 436 581 items per second;
* x1.061 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 426 837
Items/sec median:    1 436 581
Items/sec max:       1 441 313

== Median report:
Items/sec:                       1436581
Mutex contentions/sec:                 0


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 352 137
Items/sec median:    1 353 690
Items/sec max:       1 356 052

== Median report:
Items/sec:                       1353690
Mutex contentions/sec:            159237
```
</details>

---
**Micro load, 10 sender threads, 10 000 000 items, blocking receiver**

```
Command line args: -load micro -senders 10 -items 10000000 -signal 1
Run count: 3
Summary:
* 1 438 111 items per second;
* x1.067 of trivial queue;
```

<details><summary>Details</summary>

```
#### Canon MPSC queue
== Aggregated report:
Items/sec min:       1 424 377
Items/sec median:    1 438 111
Items/sec max:       1 458 051

== Median report:
Items/sec:                       1438111
Mutex contentions/sec:               162


#### Trivial MPSC queue
== Aggregated report:
Items/sec min:       1 332 403
Items/sec median:    1 347 526
Items/sec max:       1 361 530

== Median report:
Items/sec:                       1347526
Mutex contentions/sec:            155386
```
</details>
