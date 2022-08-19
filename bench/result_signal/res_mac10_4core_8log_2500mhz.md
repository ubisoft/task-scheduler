**OS**: Operating system name and version<br>
**CPU**: Processor details

---
**Empty load, 4 senders, 1 receiver, 3 000 000 signals, optimized send**

```
Command line args: -load empty -senders 4 -receivers 1 -sigs 3000000 -each 0
Run count: 5
Summary:
* 30 688 751 items per second;
* x0.944 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      29 894 530
Items/sec median:   30 688 751
Items/sec max:      33 498 448

== Median report:
Items/sec:                      30688751
Signals/sec:                     1330105
Mutex contentions/sec:             37374
Thread sender  0:   sigs:       768709, items:     17610754
Thread sender  1:   sigs:       704396, items:     16933627
Thread sender  2:   sigs:       754425, items:     17259046
Thread sender  3:   sigs:       772470, items:     17416601
Thread receiver  0: sigs:      3000000, items:     69217263


#### Trivial Signal
== Aggregated report:
Items/sec min:      31 631 642
Items/sec median:   32 500 643
Items/sec max:      34 541 999

== Median report:
Items/sec:                      32500643
Signals/sec:                      711041
Mutex contentions/sec:            137821
Thread sender  0:   sigs:       746513, items:     34198795
Thread sender  1:   sigs:       742781, items:     34324448
Thread sender  2:   sigs:       760246, items:     34456969
Thread sender  3:   sigs:       750460, items:     34166892
Thread receiver  0: sigs:      3000000, items:    137125563
```
</details>

---
**Empty load, 1 sender, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load empty -senders 1 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 19 029 323 items per second;
* x1.063 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      18 465 782
Items/sec median:   19 029 323
Items/sec max:      20 034 454

== Median report:
Items/sec:                      19029323
Signals/sec:                      183199
Mutex contentions/sec:            149327
Thread sender  0:   sigs:      1000000, items:    103872344
Thread receiver  0: sigs:       245157, items:     25479556
Thread receiver  1: sigs:       255366, items:     26311642
Thread receiver  2: sigs:       248372, items:     25529624
Thread receiver  3: sigs:       251105, items:     26551522


#### Trivial Signal
== Aggregated report:
Items/sec min:      17 871 070
Items/sec median:   17 899 216
Items/sec max:      18 602 552

== Median report:
Items/sec:                      17899216
Signals/sec:                      196323
Mutex contentions/sec:            151415
Thread sender  0:   sigs:      1000000, items:     91172202
Thread receiver  0: sigs:       249969, items:     22709757
Thread receiver  1: sigs:       248214, items:     22605567
Thread receiver  2: sigs:       249902, items:     22793694
Thread receiver  3: sigs:       251915, items:     23063184
```
</details>

---
**Empty load, 4 senders, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load empty -senders 4 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 40 482 288 items per second;
* x1.031 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      39 790 290
Items/sec median:   40 482 288
Items/sec max:      40 976 317

== Median report:
Items/sec:                      40482288
Signals/sec:                      126273
Mutex contentions/sec:            125605
Thread sender  0:   sigs:       248936, items:     79840059
Thread sender  1:   sigs:       249847, items:     80613538
Thread sender  2:   sigs:       250149, items:     79927747
Thread sender  3:   sigs:       251068, items:     80211864
Thread receiver  0: sigs:       248593, items:     80443526
Thread receiver  1: sigs:       257707, items:     79896452
Thread receiver  2: sigs:       247051, items:     80079361
Thread receiver  3: sigs:       246649, items:     80173869


#### Trivial Signal
== Aggregated report:
Items/sec min:      39 094 006
Items/sec median:   39 250 099
Items/sec max:      39 554 866

== Median report:
Items/sec:                      39250099
Signals/sec:                      130876
Mutex contentions/sec:            136853
Thread sender  0:   sigs:       250034, items:     75066773
Thread sender  1:   sigs:       248941, items:     75053298
Thread sender  2:   sigs:       251614, items:     75033686
Thread sender  3:   sigs:       249411, items:     74748381
Thread receiver  0: sigs:       250339, items:     74557902
Thread receiver  1: sigs:       250762, items:     74727179
Thread receiver  2: sigs:       249169, items:     75301663
Thread receiver  3: sigs:       249730, items:     75315394
```
</details>

---
**Empty load, 4 senders, 1 receiver, 1 000 000 signals, non-optimized send**

```
Command line args: -load empty -senders 4 -receivers 1 -sigs 1000000 -each 1
Run count: 5
Summary:
* 36 497 355 items per second;
* x9.258 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      35 918 206
Items/sec median:   36 497 355
Items/sec max:      37 791 772

== Median report:
Items/sec:                      36497355
Signals/sec:                      110254
Mutex contentions/sec:            130634
Thread sender  0:   sigs:       248725, items:     82271543
Thread sender  1:   sigs:       250013, items:     82620145
Thread sender  2:   sigs:       249400, items:     83141865
Thread sender  3:   sigs:       251862, items:     83018591
Thread receiver  0: sigs:      1000000, items:    331028031


#### Trivial Signal
== Aggregated report:
Items/sec min:       3 922 380
Items/sec median:    3 942 408
Items/sec max:       4 007 710

== Median report:
Items/sec:                       3942408
Signals/sec:                      107170
Mutex contentions/sec:            271642
Thread sender  0:   sigs:       249173, items:      9220941
Thread sender  1:   sigs:       250122, items:      9216816
Thread sender  2:   sigs:       250602, items:      9138377
Thread sender  3:   sigs:       250103, items:      9216630
Thread receiver  0: sigs:      1000000, items:     36786461
```
</details>

---
**Empty load, 1 sender, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load empty -senders 1 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 6 432 582 items per second;
* x1.025 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       6 261 968
Items/sec median:    6 432 582
Items/sec max:       6 681 662

== Median report:
Items/sec:                       6432582
Signals/sec:                      245721
Mutex contentions/sec:            211939
Thread sender  0:   sigs:      1000000, items:     26178299
Thread receiver  0: sigs:       249879, items:      6505546
Thread receiver  1: sigs:       249487, items:      6628757
Thread receiver  2: sigs:       249272, items:      6467176
Thread receiver  3: sigs:       251362, items:      6576820


#### Trivial Signal
== Aggregated report:
Items/sec min:       6 079 872
Items/sec median:    6 274 781
Items/sec max:       6 306 157

== Median report:
Items/sec:                       6274781
Signals/sec:                       57216
Mutex contentions/sec:             46696
Thread sender  0:   sigs:      1000000, items:    109667109
Thread receiver  0: sigs:       251168, items:     28092695
Thread receiver  1: sigs:       250800, items:     28504834
Thread receiver  2: sigs:       247910, items:     27549031
Thread receiver  3: sigs:       250122, items:     25520549
```
</details>

---
**Empty load, 4 senders, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load empty -senders 4 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 34 854 964 items per second;
* x23.656 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      34 004 963
Items/sec median:   34 854 964
Items/sec max:      35 534 891

== Median report:
Items/sec:                      34854964
Signals/sec:                       86151
Mutex contentions/sec:            107835
Thread sender  0:   sigs:       250809, items:    101388159
Thread sender  1:   sigs:       250006, items:    101278910
Thread sender  2:   sigs:       248452, items:    101193600
Thread sender  3:   sigs:       250733, items:    100718383
Thread receiver  0: sigs:       236291, items:    102788277
Thread receiver  1: sigs:       255064, items:    101776482
Thread receiver  2: sigs:       253442, items:     99865699
Thread receiver  3: sigs:       255203, items:    100148594


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 456 889
Items/sec median:    1 473 380
Items/sec max:       1 499 697

== Median report:
Items/sec:                       1473380
Signals/sec:                      149787
Mutex contentions/sec:            176703
Thread sender  0:   sigs:       249089, items:      2439568
Thread sender  1:   sigs:       250994, items:      2467944
Thread sender  2:   sigs:       250080, items:      2465577
Thread sender  3:   sigs:       249837, items:      2463406
Thread receiver  0: sigs:       250499, items:      2450180
Thread receiver  1: sigs:       249974, items:      2456251
Thread receiver  2: sigs:       247882, items:      2432709
Thread receiver  3: sigs:       251645, items:      2497355
```
</details>

---
**Nano load, 4 senders, 1 receiver, 1 000 000 signals, optimized send**

```
Command line args: -load nano -senders 4 -receivers 1 -sigs 1000000 -each 0
Run count: 5
Summary:
* 6 744 396 items per second;
* x1.013 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       6 562 360
Items/sec median:    6 744 396
Items/sec max:       6 776 113

== Median report:
Items/sec:                       6744396
Signals/sec:                     1197490
Mutex contentions/sec:              1748
Thread sender  0:   sigs:       246592, items:      1381683
Thread sender  1:   sigs:       254400, items:      1439174
Thread sender  2:   sigs:       245524, items:      1382104
Thread sender  3:   sigs:       253484, items:      1434395
Thread receiver  0: sigs:      1000000, items:      5632107


#### Trivial Signal
== Aggregated report:
Items/sec min:       6 442 436
Items/sec median:    6 656 017
Items/sec max:       6 683 054

== Median report:
Items/sec:                       6656017
Signals/sec:                     1062380
Mutex contentions/sec:              1352
Thread sender  0:   sigs:       249341, items:      1562488
Thread sender  1:   sigs:       244999, items:      1600519
Thread sender  2:   sigs:       253284, items:      1553028
Thread sender  3:   sigs:       252376, items:      1550623
Thread receiver  0: sigs:      1000000, items:      6265192
```
</details>

---
**Nano load, 1 sender, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load nano -senders 1 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 441 758 items per second;
* x0.771 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         440 454
Items/sec median:      441 758
Items/sec max:         456 417

== Median report:
Items/sec:                        441758
Signals/sec:                      238825
Mutex contentions/sec:            115115
Thread sender  0:   sigs:      1000000, items:      1849711
Thread receiver  0: sigs:       247940, items:       458275
Thread receiver  1: sigs:       247446, items:       461275
Thread receiver  2: sigs:       257507, items:       474335
Thread receiver  3: sigs:       247107, items:       455826


#### Trivial Signal
== Aggregated report:
Items/sec min:         571 611
Items/sec median:      573 032
Items/sec max:         574 463

== Median report:
Items/sec:                        573032
Signals/sec:                      204814
Mutex contentions/sec:            149266
Thread sender  0:   sigs:      1000000, items:      2797810
Thread receiver  0: sigs:       249896, items:       688809
Thread receiver  1: sigs:       250158, items:       700824
Thread receiver  2: sigs:       252149, items:       719261
Thread receiver  3: sigs:       247797, items:       688916
```
</details>

---
**Nano load, 4 senders, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load nano -senders 4 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 4 547 612 items per second;
* x0.934 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       4 477 368
Items/sec median:    4 547 612
Items/sec max:       4 609 831

== Median report:
Items/sec:                       4547612
Signals/sec:                      126002
Mutex contentions/sec:            103122
Thread sender  0:   sigs:       250941, items:      9046126
Thread sender  1:   sigs:       250051, items:      9009507
Thread sender  2:   sigs:       248495, items:      8993458
Thread sender  3:   sigs:       250513, items:      9042468
Thread receiver  0: sigs:       246723, items:      8808761
Thread receiver  1: sigs:       248218, items:      9197025
Thread receiver  2: sigs:       256005, items:      8922472
Thread receiver  3: sigs:       249054, items:      9163301


#### Trivial Signal
== Aggregated report:
Items/sec min:       4 803 153
Items/sec median:    4 866 761
Items/sec max:       4 899 837

== Median report:
Items/sec:                       4866761
Signals/sec:                      160834
Mutex contentions/sec:            116166
Thread sender  0:   sigs:       248582, items:      7523877
Thread sender  1:   sigs:       250192, items:      7571289
Thread sender  2:   sigs:       250304, items:      7586338
Thread sender  3:   sigs:       250922, items:      7577951
Thread receiver  0: sigs:       253721, items:      7746055
Thread receiver  1: sigs:       247818, items:      7664650
Thread receiver  2: sigs:       256049, items:      7594876
Thread receiver  3: sigs:       242412, items:      7253874
```
</details>

---
**Nano load, 4 senders, 1 receiver, 1 000 000 signals, non-optimized send**

```
Command line args: -load nano -senders 4 -receivers 1 -sigs 1000000 -each 1
Run count: 5
Summary:
* 6 352 246 items per second;
* x4.109 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       6 095 611
Items/sec median:    6 352 246
Items/sec max:       6 407 961

== Median report:
Items/sec:                       6352246
Signals/sec:                     1092173
Mutex contentions/sec:              2519
Thread sender  0:   sigs:       240071, items:      1428776
Thread sender  1:   sigs:       251949, items:      1474655
Thread sender  2:   sigs:       255750, items:      1459000
Thread sender  3:   sigs:       252230, items:      1458194
Thread receiver  0: sigs:      1000000, items:      5816151


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 534 632
Items/sec median:    1 546 060
Items/sec max:       1 579 919

== Median report:
Items/sec:                       1546060
Signals/sec:                      281373
Mutex contentions/sec:            388459
Thread sender  0:   sigs:       246561, items:      1362737
Thread sender  1:   sigs:       251560, items:      1380247
Thread sender  2:   sigs:       250011, items:      1372660
Thread sender  3:   sigs:       251868, items:      1381033
Thread receiver  0: sigs:      1000000, items:      5494701
```
</details>

---
**Nano load, 1 sender, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load nano -senders 1 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 396 844 items per second;
* x1.040 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         380 748
Items/sec median:      396 844
Items/sec max:         410 163

== Median report:
Items/sec:                        396844
Signals/sec:                      254396
Mutex contentions/sec:            111885
Thread sender  0:   sigs:      1000000, items:      1559945
Thread receiver  0: sigs:       249710, items:       387305
Thread receiver  1: sigs:       250902, items:       392606
Thread receiver  2: sigs:       249839, items:       390852
Thread receiver  3: sigs:       249549, items:       389182


#### Trivial Signal
== Aggregated report:
Items/sec min:         376 650
Items/sec median:      381 643
Items/sec max:         386 514

== Median report:
Items/sec:                        381643
Signals/sec:                      244626
Mutex contentions/sec:            151209
Thread sender  0:   sigs:      1000000, items:      1560107
Thread receiver  0: sigs:       246832, items:       385216
Thread receiver  1: sigs:       245134, items:       380890
Thread receiver  2: sigs:       255590, items:       401491
Thread receiver  3: sigs:       252444, items:       392510
```
</details>

---
**Nano load, 4 senders, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load nano -senders 4 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 4 370 053 items per second;
* x9.123 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       4 286 690
Items/sec median:    4 370 053
Items/sec max:       4 462 497

== Median report:
Items/sec:                       4370053
Signals/sec:                      135688
Mutex contentions/sec:            111804
Thread sender  0:   sigs:       247899, items:      8038619
Thread sender  1:   sigs:       249701, items:      8049067
Thread sender  2:   sigs:       250639, items:      8065182
Thread sender  3:   sigs:       251761, items:      8053750
Thread receiver  0: sigs:       258009, items:      7949606
Thread receiver  1: sigs:       252802, items:      8180085
Thread receiver  2: sigs:       242464, items:      8048549
Thread receiver  3: sigs:       246725, items:      8028378


#### Trivial Signal
== Aggregated report:
Items/sec min:         476 263
Items/sec median:      479 016
Items/sec max:         493 959

== Median report:
Items/sec:                        479016
Signals/sec:                      199105
Mutex contentions/sec:            221330
Thread sender  0:   sigs:       251195, items:       601840
Thread sender  1:   sigs:       249678, items:       601725
Thread sender  2:   sigs:       249611, items:       600590
Thread sender  3:   sigs:       249516, items:       601693
Thread receiver  0: sigs:       250810, items:       605853
Thread receiver  1: sigs:       250545, items:       599820
Thread receiver  2: sigs:       247058, items:       593211
Thread receiver  3: sigs:       251587, items:       606964
```
</details>

---
**Micro load, 4 senders, 1 receiver, 1 000 000 signals, optimized send**

```
Command line args: -load micro -senders 4 -receivers 1 -sigs 1000000 -each 0
Run count: 5
Summary:
* 1 422 657 items per second;
* x1.059 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       1 393 860
Items/sec median:    1 422 657
Items/sec max:       1 434 827

== Median report:
Items/sec:                       1422657
Signals/sec:                      316138
Mutex contentions/sec:               540
Thread sender  0:   sigs:       246228, items:      1143362
Thread sender  1:   sigs:       247874, items:      1117660
Thread sender  2:   sigs:       248588, items:      1119212
Thread sender  3:   sigs:       257310, items:      1120378
Thread receiver  0: sigs:      1000000, items:      4500101


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 237 506
Items/sec median:    1 343 294
Items/sec max:       1 372 729

== Median report:
Items/sec:                       1343294
Signals/sec:                      305247
Mutex contentions/sec:               298
Thread sender  0:   sigs:       242201, items:      1100817
Thread sender  1:   sigs:       254135, items:      1100099
Thread sender  2:   sigs:       250493, items:      1104682
Thread sender  3:   sigs:       253171, items:      1095836
Thread receiver  0: sigs:      1000000, items:      4400673
```
</details>

---
**Micro load, 1 sender, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load micro -senders 1 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 266 704 items per second;
* x1.230 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         263 783
Items/sec median:      266 704
Items/sec max:         269 331

== Median report:
Items/sec:                        266704
Signals/sec:                      250445
Mutex contentions/sec:             30520
Thread sender  0:   sigs:      1000000, items:      1064919
Thread receiver  0: sigs:       251227, items:       268184
Thread receiver  1: sigs:       248826, items:       264786
Thread receiver  2: sigs:       247229, items:       262863
Thread receiver  3: sigs:       252718, items:       269086


#### Trivial Signal
== Aggregated report:
Items/sec min:         216 579
Items/sec median:      216 777
Items/sec max:         218 048

== Median report:
Items/sec:                        216777
Signals/sec:                      202924
Mutex contentions/sec:            112475
Thread sender  0:   sigs:      1000000, items:      1068267
Thread receiver  0: sigs:       249621, items:       266567
Thread receiver  1: sigs:       251457, items:       268225
Thread receiver  2: sigs:       249227, items:       266222
Thread receiver  3: sigs:       249695, items:       267253
```
</details>

---
**Micro load, 4 senders, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load micro -senders 4 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 1 011 632 items per second;
* x1.020 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         924 512
Items/sec median:    1 011 632
Items/sec max:       1 060 425

== Median report:
Items/sec:                       1011632
Signals/sec:                      200025
Mutex contentions/sec:             37229
Thread sender  0:   sigs:       250998, items:      1264640
Thread sender  1:   sigs:       251556, items:      1266109
Thread sender  2:   sigs:       248312, items:      1262418
Thread sender  3:   sigs:       249134, items:      1264356
Thread receiver  0: sigs:       245033, items:      1223580
Thread receiver  1: sigs:       257254, items:      1311364
Thread receiver  2: sigs:       253436, items:      1261599
Thread receiver  3: sigs:       244277, items:      1260980


#### Trivial Signal
== Aggregated report:
Items/sec min:         950 277
Items/sec median:      991 644
Items/sec max:       1 041 131

== Median report:
Items/sec:                        991644
Signals/sec:                      182999
Mutex contentions/sec:             52524
Thread sender  0:   sigs:       251505, items:      1350911
Thread sender  1:   sigs:       246114, items:      1352137
Thread sender  2:   sigs:       251065, items:      1355251
Thread sender  3:   sigs:       251316, items:      1360544
Thread receiver  0: sigs:       251833, items:      1369014
Thread receiver  1: sigs:       248011, items:      1366572
Thread receiver  2: sigs:       250053, items:      1326888
Thread receiver  3: sigs:       250103, items:      1356369
```
</details>

---
**Micro load, 4 senders, 1 receiver, 1 000 000 signals, non-optimized send**

```
Command line args: -load micro -senders 4 -receivers 1 -sigs 1000000 -each 1
Run count: 5
Summary:
* 1 417 856 items per second;
* x1.215 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       1 379 409
Items/sec median:    1 417 856
Items/sec max:       1 422 112

== Median report:
Items/sec:                       1417856
Signals/sec:                      301106
Mutex contentions/sec:               810
Thread sender  0:   sigs:       252376, items:      1177084
Thread sender  1:   sigs:       250249, items:      1171526
Thread sender  2:   sigs:       254671, items:      1185873
Thread sender  3:   sigs:       242704, items:      1175325
Thread receiver  0: sigs:      1000000, items:      4708817


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 143 901
Items/sec median:    1 166 572
Items/sec max:       1 189 201

== Median report:
Items/sec:                       1166572
Signals/sec:                      298646
Mutex contentions/sec:            124891
Thread sender  0:   sigs:       249275, items:       979236
Thread sender  1:   sigs:       250414, items:       976253
Thread sender  2:   sigs:       249847, items:       976546
Thread sender  3:   sigs:       250464, items:       975052
Thread receiver  0: sigs:      1000000, items:      3906201
```
</details>

---
**Micro load, 1 sender, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load micro -senders 1 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 265 187 items per second;
* x1.236 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         259 385
Items/sec median:      265 187
Items/sec max:         266 329

== Median report:
Items/sec:                        265187
Signals/sec:                      248532
Mutex contentions/sec:             30517
Thread sender  0:   sigs:      1000000, items:      1067014
Thread receiver  0: sigs:       250410, items:       267735
Thread receiver  1: sigs:       253700, items:       269218
Thread receiver  2: sigs:       247067, items:       265323
Thread receiver  3: sigs:       248823, items:       264738


#### Trivial Signal
== Aggregated report:
Items/sec min:         212 336
Items/sec median:      214 620
Items/sec max:         216 638

== Median report:
Items/sec:                        214620
Signals/sec:                      207464
Mutex contentions/sec:            114077
Thread sender  0:   sigs:      1000000, items:      1034492
Thread receiver  0: sigs:       250861, items:       258896
Thread receiver  1: sigs:       248785, items:       258213
Thread receiver  2: sigs:       250792, items:       259757
Thread receiver  3: sigs:       249562, items:       257626
```
</details>

---
**Micro load, 4 senders, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load micro -senders 4 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 1 006 217 items per second;
* x3.343 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         979 805
Items/sec median:    1 006 217
Items/sec max:       1 037 335

== Median report:
Items/sec:                       1006217
Signals/sec:                      203016
Mutex contentions/sec:             43897
Thread sender  0:   sigs:       248949, items:      1229884
Thread sender  1:   sigs:       251740, items:      1247248
Thread sender  2:   sigs:       251278, items:      1244771
Thread sender  3:   sigs:       248033, items:      1234429
Thread receiver  0: sigs:       247186, items:      1257328
Thread receiver  1: sigs:       252739, items:      1228663
Thread receiver  2: sigs:       249389, items:      1229913
Thread receiver  3: sigs:       250686, items:      1240428


#### Trivial Signal
== Aggregated report:
Items/sec min:         298 700
Items/sec median:      300 967
Items/sec max:         306 809

== Median report:
Items/sec:                        300967
Signals/sec:                      168777
Mutex contentions/sec:            172195
Thread sender  0:   sigs:       249862, items:       445212
Thread sender  1:   sigs:       249988, items:       446097
Thread sender  2:   sigs:       250522, items:       446234
Thread sender  3:   sigs:       249628, items:       445676
Thread receiver  0: sigs:       249440, items:       444065
Thread receiver  1: sigs:       249478, items:       442803
Thread receiver  2: sigs:       251256, items:       448278
Thread receiver  3: sigs:       249826, items:       448073
```
</details>
