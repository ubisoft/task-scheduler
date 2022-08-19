**OS**: Operating system name and version<br>
**CPU**: Processor details

---
**Empty load, 4 senders, 1 receiver, 3 000 000 signals, optimized send**

```
Command line args: -load empty -senders 4 -receivers 1 -sigs 3000000 -each 0
Run count: 5
Summary:
* 22 501 663 items per second;
* x0.918 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      21 555 196
Items/sec median:   22 501 663
Items/sec max:      25 326 738

== Median report:
Items/sec:                      22501663
Signals/sec:                      563718
Mutex contentions/sec:            322277
Thread sender  0:   sigs:       764498, items:     29918815
Thread sender  1:   sigs:       750166, items:     32505503
Thread sender  2:   sigs:       762093, items:     28637356
Thread sender  3:   sigs:       723243, items:     28698541
Thread receiver  0: sigs:      3000000, items:    119749521


#### Trivial Signal
== Aggregated report:
Items/sec min:      22 545 391
Items/sec median:   24 523 317
Items/sec max:      25 596 611

== Median report:
Items/sec:                      24523317
Signals/sec:                      428991
Mutex contentions/sec:            204537
Thread sender  0:   sigs:       745765, items:     45302840
Thread sender  1:   sigs:       726284, items:     39205978
Thread sender  2:   sigs:       807999, items:     40675362
Thread sender  3:   sigs:       719952, items:     46334025
Thread receiver  0: sigs:      3000000, items:    171495246
```
</details>

---
**Empty load, 1 sender, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load empty -senders 1 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 12 317 126 items per second;
* x0.759 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      11 594 132
Items/sec median:   12 317 126
Items/sec max:      12 974 580

== Median report:
Items/sec:                      12317126
Signals/sec:                       77480
Mutex contentions/sec:             48846
Thread sender  0:   sigs:      1000000, items:    158971054
Thread receiver  0: sigs:       247087, items:     38407922
Thread receiver  1: sigs:       252083, items:     41107214
Thread receiver  2: sigs:       248368, items:     39010391
Thread receiver  3: sigs:       252462, items:     40445527


#### Trivial Signal
== Aggregated report:
Items/sec min:      16 143 427
Items/sec median:   16 235 500
Items/sec max:      17 500 192

== Median report:
Items/sec:                      16235500
Signals/sec:                       77511
Mutex contentions/sec:             50810
Thread sender  0:   sigs:      1000000, items:    209459683
Thread receiver  0: sigs:       251539, items:     52873058
Thread receiver  1: sigs:       249528, items:     52671123
Thread receiver  2: sigs:       252412, items:     54058186
Thread receiver  3: sigs:       246521, items:     49857316
```
</details>

---
**Empty load, 4 senders, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load empty -senders 4 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 22 708 927 items per second;
* x0.890 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      22 164 680
Items/sec median:   22 708 927
Items/sec max:      24 071 768

== Median report:
Items/sec:                      22708927
Signals/sec:                       65994
Mutex contentions/sec:             48183
Thread sender  0:   sigs:       272264, items:     77090877
Thread sender  1:   sigs:       239606, items:     96205178
Thread sender  2:   sigs:       248154, items:     82893702
Thread sender  3:   sigs:       239976, items:     87913891
Thread receiver  0: sigs:       250665, items:     84521185
Thread receiver  1: sigs:       253633, items:     86791927
Thread receiver  2: sigs:       244588, items:     85676306
Thread receiver  3: sigs:       251114, items:     87114230


#### Trivial Signal
== Aggregated report:
Items/sec min:      25 451 916
Items/sec median:   25 523 363
Items/sec max:      26 452 530

== Median report:
Items/sec:                      25523363
Signals/sec:                       89892
Mutex contentions/sec:             74712
Thread sender  0:   sigs:       234511, items:     69652171
Thread sender  1:   sigs:       233035, items:     60497834
Thread sender  2:   sigs:       290195, items:     78906604
Thread sender  3:   sigs:       242259, items:     74874525
Thread receiver  0: sigs:       243584, items:     69911375
Thread receiver  1: sigs:       255732, items:     71195363
Thread receiver  2: sigs:       251548, items:     71414353
Thread receiver  3: sigs:       249136, items:     71410043
```
</details>

---
**Empty load, 4 senders, 1 receiver, 1 000 000 signals, non-optimized send**

```
Command line args: -load empty -senders 4 -receivers 1 -sigs 1000000 -each 1
Run count: 5
Summary:
* 16 395 468 items per second;
* x4.457 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      14 062 525
Items/sec median:   16 395 468
Items/sec max:      17 289 682

== Median report:
Items/sec:                      16395468
Signals/sec:                      305160
Mutex contentions/sec:            278872
Thread sender  0:   sigs:       246942, items:     13352530
Thread sender  1:   sigs:       251275, items:     13701012
Thread sender  2:   sigs:       248439, items:     13450827
Thread sender  3:   sigs:       253344, items:     13238581
Thread receiver  0: sigs:      1000000, items:     53727422


#### Trivial Signal
== Aggregated report:
Items/sec min:       3 304 452
Items/sec median:    3 678 573
Items/sec max:       4 050 715

== Median report:
Items/sec:                       3678573
Signals/sec:                      263852
Mutex contentions/sec:            744849
Thread sender  0:   sigs:       249769, items:      3317184
Thread sender  1:   sigs:       246503, items:      3509542
Thread sender  2:   sigs:       244357, items:      3485816
Thread sender  3:   sigs:       259371, items:      3634437
Thread receiver  0: sigs:      1000000, items:     13941761
```
</details>

---
**Empty load, 1 sender, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load empty -senders 1 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 4 002 276 items per second;
* x2.037 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       3 389 416
Items/sec median:    4 002 276
Items/sec max:       4 156 722

== Median report:
Items/sec:                       4002276
Signals/sec:                      147487
Mutex contentions/sec:             89474
Thread sender  0:   sigs:      1000000, items:     27136325
Thread receiver  0: sigs:       250893, items:      7256477
Thread receiver  1: sigs:       248657, items:      6389349
Thread receiver  2: sigs:       253064, items:      6736805
Thread receiver  3: sigs:       247386, items:      6753694


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 898 370
Items/sec median:    1 964 909
Items/sec max:       2 071 144

== Median report:
Items/sec:                       1964909
Signals/sec:                      117062
Mutex contentions/sec:             99285
Thread sender  0:   sigs:      1000000, items:     16785117
Thread receiver  0: sigs:       250192, items:      4135484
Thread receiver  1: sigs:       248130, items:      4119206
Thread receiver  2: sigs:       240834, items:      4020897
Thread receiver  3: sigs:       260844, items:      4509530
```
</details>

---
**Empty load, 4 senders, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load empty -senders 4 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 15 935 598 items per second;
* x8.660 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:      14 975 096
Items/sec median:   15 935 598
Items/sec max:      18 736 491

== Median report:
Items/sec:                      15935598
Signals/sec:                       88432
Mutex contentions/sec:            104780
Thread sender  0:   sigs:       251164, items:     44480961
Thread sender  1:   sigs:       248759, items:     45452458
Thread sender  2:   sigs:       249647, items:     45672791
Thread sender  3:   sigs:       250430, items:     44594524
Thread receiver  0: sigs:       252037, items:     46080863
Thread receiver  1: sigs:       248544, items:     44832846
Thread receiver  2: sigs:       250255, items:     44936906
Thread receiver  3: sigs:       249164, items:     44350119


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 641 865
Items/sec median:    1 840 130
Items/sec max:       1 921 336

== Median report:
Items/sec:                       1840130
Signals/sec:                      157660
Mutex contentions/sec:            276406
Thread sender  0:   sigs:       249025, items:      2886872
Thread sender  1:   sigs:       247744, items:      2882206
Thread sender  2:   sigs:       252166, items:      2946310
Thread sender  3:   sigs:       251065, items:      2956101
Thread receiver  0: sigs:       250748, items:      2924949
Thread receiver  1: sigs:       250005, items:      2908379
Thread receiver  2: sigs:       250513, items:      2930049
Thread receiver  3: sigs:       248734, items:      2908112
```
</details>

---
**Nano load, 4 senders, 1 receiver, 1 000 000 signals, optimized send**

```
Command line args: -load nano -senders 4 -receivers 1 -sigs 1000000 -each 0
Run count: 5
Summary:
* 5 049 373 items per second;
* x0.951 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       4 792 378
Items/sec median:    5 049 373
Items/sec max:       5 259 468

== Median report:
Items/sec:                       5049373
Signals/sec:                     1226750
Mutex contentions/sec:             39305
Thread sender  0:   sigs:       236193, items:      1053335
Thread sender  1:   sigs:       224188, items:      1048631
Thread sender  2:   sigs:       298127, items:      1081585
Thread sender  3:   sigs:       241492, items:       942276
Thread receiver  0: sigs:      1000000, items:      4116055


#### Trivial Signal
== Aggregated report:
Items/sec min:       5 085 959
Items/sec median:    5 310 150
Items/sec max:       5 436 716

== Median report:
Items/sec:                       5310150
Signals/sec:                     1234678
Mutex contentions/sec:              6373
Thread sender  0:   sigs:       263217, items:      1096102
Thread sender  1:   sigs:       274139, items:      1101368
Thread sender  2:   sigs:       246023, items:      1063810
Thread sender  3:   sigs:       216621, items:      1041033
Thread receiver  0: sigs:      1000000, items:      4300838
```
</details>

---
**Nano load, 1 sender, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load nano -senders 1 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 384 286 items per second;
* x0.835 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         378 450
Items/sec median:      384 286
Items/sec max:         396 181

== Median report:
Items/sec:                        384286
Signals/sec:                      100533
Mutex contentions/sec:             43771
Thread sender  0:   sigs:      1000000, items:      3822495
Thread receiver  0: sigs:       246722, items:       932593
Thread receiver  1: sigs:       249093, items:       953238
Thread receiver  2: sigs:       255487, items:       964300
Thread receiver  3: sigs:       248698, items:       972364


#### Trivial Signal
== Aggregated report:
Items/sec min:         450 829
Items/sec median:      460 171
Items/sec max:         472 629

== Median report:
Items/sec:                        460171
Signals/sec:                       92785
Mutex contentions/sec:             60051
Thread sender  0:   sigs:      1000000, items:      4959528
Thread receiver  0: sigs:       241227, items:      1214031
Thread receiver  1: sigs:       257935, items:      1315659
Thread receiver  2: sigs:       252531, items:      1217216
Thread receiver  3: sigs:       248307, items:      1212622
```
</details>

---
**Nano load, 4 senders, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load nano -senders 4 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 3 739 254 items per second;
* x0.834 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       3 734 609
Items/sec median:    3 739 254
Items/sec max:       3 794 649

== Median report:
Items/sec:                       3739254
Signals/sec:                       80996
Mutex contentions/sec:             67393
Thread sender  0:   sigs:       253247, items:     11646035
Thread sender  1:   sigs:       249896, items:     11479634
Thread sender  2:   sigs:       242281, items:     11343211
Thread sender  3:   sigs:       254576, items:     11696976
Thread receiver  0: sigs:       248273, items:     11326727
Thread receiver  1: sigs:       252951, items:     11749203
Thread receiver  2: sigs:       250578, items:     11489488
Thread receiver  3: sigs:       248198, items:     11600438


#### Trivial Signal
== Aggregated report:
Items/sec min:       4 359 631
Items/sec median:    4 485 019
Items/sec max:       4 525 252

== Median report:
Items/sec:                       4485019
Signals/sec:                      143898
Mutex contentions/sec:             98344
Thread sender  0:   sigs:       261790, items:      8032156
Thread sender  1:   sigs:       267909, items:      7910088
Thread sender  2:   sigs:       225251, items:      7504863
Thread sender  3:   sigs:       245050, items:      7720746
Thread receiver  0: sigs:       251699, items:      7721167
Thread receiver  1: sigs:       252374, items:      7777630
Thread receiver  2: sigs:       243840, items:      7801524
Thread receiver  3: sigs:       252087, items:      7867532
```
</details>

---
**Nano load, 4 senders, 1 receiver, 1 000 000 signals, non-optimized send**

```
Command line args: -load nano -senders 4 -receivers 1 -sigs 1000000 -each 1
Run count: 5
Summary:
* 4 491 252 items per second;
* x2.151 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       4 388 286
Items/sec median:    4 491 252
Items/sec max:       5 005 540

== Median report:
Items/sec:                       4491252
Signals/sec:                      960952
Mutex contentions/sec:             91506
Thread sender  0:   sigs:       241494, items:      1227381
Thread sender  1:   sigs:       254762, items:      1170980
Thread sender  2:   sigs:       246873, items:      1149813
Thread sender  3:   sigs:       256871, items:      1128766
Thread receiver  0: sigs:      1000000, items:      4673751


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 484 804
Items/sec median:    2 087 577
Items/sec max:       2 381 499

== Median report:
Items/sec:                       2087577
Signals/sec:                      492856
Mutex contentions/sec:            737484
Thread sender  0:   sigs:       219642, items:       947772
Thread sender  1:   sigs:       266842, items:      1138435
Thread sender  2:   sigs:       254371, items:      1063475
Thread sender  3:   sigs:       259145, items:      1088718
Thread receiver  0: sigs:      1000000, items:      4235673
```
</details>

---
**Nano load, 1 sender, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load nano -senders 1 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 327 000 items per second;
* x0.988 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         325 899
Items/sec median:      327 000
Items/sec max:         347 476

== Median report:
Items/sec:                        327000
Signals/sec:                      163535
Mutex contentions/sec:             52213
Thread sender  0:   sigs:      1000000, items:      1999568
Thread receiver  0: sigs:       238967, items:       470171
Thread receiver  1: sigs:       257823, items:       529713
Thread receiver  2: sigs:       255055, items:       517646
Thread receiver  3: sigs:       248155, items:       482038


#### Trivial Signal
== Aggregated report:
Items/sec min:         317 994
Items/sec median:      331 109
Items/sec max:         345 596

== Median report:
Items/sec:                        331109
Signals/sec:                      162855
Mutex contentions/sec:             76535
Thread sender  0:   sigs:      1000000, items:      2033151
Thread receiver  0: sigs:       241003, items:       492563
Thread receiver  1: sigs:       248354, items:       514347
Thread receiver  2: sigs:       251879, items:       498474
Thread receiver  3: sigs:       258764, items:       527767
```
</details>

---
**Nano load, 4 senders, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load nano -senders 4 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 3 489 034 items per second;
* x6.128 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       3 456 694
Items/sec median:    3 489 034
Items/sec max:       3 549 033

== Median report:
Items/sec:                       3489034
Signals/sec:                      107853
Mutex contentions/sec:             87305
Thread sender  0:   sigs:       247468, items:      8021949
Thread sender  1:   sigs:       252665, items:      8168559
Thread sender  2:   sigs:       251878, items:      8141513
Thread sender  3:   sigs:       247989, items:      8017716
Thread receiver  0: sigs:       249999, items:      8119800
Thread receiver  1: sigs:       249197, items:      8151891
Thread receiver  2: sigs:       251455, items:      7976910
Thread receiver  3: sigs:       249349, items:      8101136


#### Trivial Signal
== Aggregated report:
Items/sec min:         550 620
Items/sec median:      569 369
Items/sec max:         612 324

== Median report:
Items/sec:                        569369
Signals/sec:                      206274
Mutex contentions/sec:            233398
Thread sender  0:   sigs:       249578, items:       693421
Thread sender  1:   sigs:       248516, items:       684379
Thread sender  2:   sigs:       247788, items:       682525
Thread sender  3:   sigs:       254118, items:       699923
Thread receiver  0: sigs:       248382, items:       686644
Thread receiver  1: sigs:       251228, items:       694213
Thread receiver  2: sigs:       253594, items:       691759
Thread receiver  3: sigs:       246796, items:       687632
```
</details>

---
**Micro load, 4 senders, 1 receiver, 1 000 000 signals, optimized send**

```
Command line args: -load micro -senders 4 -receivers 1 -sigs 1000000 -each 0
Run count: 5
Summary:
* 1 265 931 items per second;
* almost same as trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       1 252 163
Items/sec median:    1 265 931
Items/sec max:       1 273 095

== Median report:
Items/sec:                       1265931
Signals/sec:                      317071
Mutex contentions/sec:               101
Thread sender  0:   sigs:       252799, items:       993443
Thread sender  1:   sigs:       247453, items:       991224
Thread sender  2:   sigs:       248022, items:      1004649
Thread sender  3:   sigs:       251726, items:      1003448
Thread receiver  0: sigs:      1000000, items:      3992570


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 235 307
Items/sec median:    1 275 819
Items/sec max:       1 290 604

== Median report:
Items/sec:                       1275819
Signals/sec:                      309701
Mutex contentions/sec:               372
Thread sender  0:   sigs:       242735, items:      1007410
Thread sender  1:   sigs:       252967, items:      1031012
Thread sender  2:   sigs:       254023, items:      1033007
Thread sender  3:   sigs:       250275, items:      1048114
Thread receiver  0: sigs:      1000000, items:      4119509
```
</details>

---
**Micro load, 1 sender, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load micro -senders 1 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 110 159 items per second;
* x0.810 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         109 539
Items/sec median:      110 159
Items/sec max:         111 348

== Median report:
Items/sec:                        110159
Signals/sec:                       82755
Mutex contentions/sec:             18942
Thread sender  0:   sigs:      1000000, items:      1331152
Thread receiver  0: sigs:       251727, items:       332530
Thread receiver  1: sigs:       246367, items:       332184
Thread receiver  2: sigs:       256016, items:       334957
Thread receiver  3: sigs:       245890, items:       331481


#### Trivial Signal
== Aggregated report:
Items/sec min:         133 286
Items/sec median:      135 918
Items/sec max:         137 440

== Median report:
Items/sec:                        135918
Signals/sec:                       74687
Mutex contentions/sec:             54902
Thread sender  0:   sigs:      1000000, items:      1819815
Thread receiver  0: sigs:       250713, items:       457774
Thread receiver  1: sigs:       246836, items:       447055
Thread receiver  2: sigs:       251683, items:       458795
Thread receiver  3: sigs:       250768, items:       456191
```
</details>

---
**Micro load, 4 senders, 4 receivers, 1 000 000 signals, optimized send**

```
Command line args: -load micro -senders 4 -receivers 4 -sigs 1000000 -each 0
Run count: 5
Summary:
* 827 968 items per second;
* x0.842 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         807 992
Items/sec median:      827 968
Items/sec max:         848 689

== Median report:
Items/sec:                        827968
Signals/sec:                       73282
Mutex contentions/sec:             46313
Thread sender  0:   sigs:       254686, items:      2871713
Thread sender  1:   sigs:       248181, items:      2820440
Thread sender  2:   sigs:       249275, items:      2826075
Thread sender  3:   sigs:       247858, items:      2780152
Thread receiver  0: sigs:       251110, items:      2821875
Thread receiver  1: sigs:       250263, items:      2833161
Thread receiver  2: sigs:       249067, items:      2824818
Thread receiver  3: sigs:       249560, items:      2818526


#### Trivial Signal
== Aggregated report:
Items/sec min:         953 180
Items/sec median:      983 037
Items/sec max:       1 024 063

== Median report:
Items/sec:                        983037
Signals/sec:                      107068
Mutex contentions/sec:             72896
Thread sender  0:   sigs:       256108, items:      2330914
Thread sender  1:   sigs:       242369, items:      2244281
Thread sender  2:   sigs:       251840, items:      2311771
Thread sender  3:   sigs:       249683, items:      2294421
Thread receiver  0: sigs:       256405, items:      2333912
Thread receiver  1: sigs:       243787, items:      2239585
Thread receiver  2: sigs:       247235, items:      2286802
Thread receiver  3: sigs:       252573, items:      2321088
```
</details>

---
**Micro load, 4 senders, 1 receiver, 1 000 000 signals, non-optimized send**

```
Command line args: -load micro -senders 4 -receivers 1 -sigs 1000000 -each 1
Run count: 5
Summary:
* 1 264 866 items per second;
* x1.040 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:       1 229 385
Items/sec median:    1 264 866
Items/sec max:       1 272 547

== Median report:
Items/sec:                       1264866
Signals/sec:                      312664
Mutex contentions/sec:               410
Thread sender  0:   sigs:       240213, items:      1019109
Thread sender  1:   sigs:       240253, items:       993566
Thread sender  2:   sigs:       249909, items:      1009247
Thread sender  3:   sigs:       269625, items:      1023556
Thread receiver  0: sigs:      1000000, items:      4045448


#### Trivial Signal
== Aggregated report:
Items/sec min:       1 103 233
Items/sec median:    1 216 011
Items/sec max:       1 225 137

== Median report:
Items/sec:                       1216011
Signals/sec:                      302365
Mutex contentions/sec:             86138
Thread sender  0:   sigs:       247803, items:      1009690
Thread sender  1:   sigs:       248331, items:       976720
Thread sender  2:   sigs:       252657, items:      1011418
Thread sender  3:   sigs:       251209, items:      1024267
Thread receiver  0: sigs:      1000000, items:      4021657
```
</details>

---
**Micro load, 1 sender, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load micro -senders 1 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 104 446 items per second;
* x0.911 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         103 442
Items/sec median:      104 446
Items/sec max:         106 630

== Median report:
Items/sec:                        104446
Signals/sec:                       90292
Mutex contentions/sec:             17184
Thread sender  0:   sigs:      1000000, items:      1156757
Thread receiver  0: sigs:       256481, items:       291855
Thread receiver  1: sigs:       244435, items:       288393
Thread receiver  2: sigs:       248861, items:       287004
Thread receiver  3: sigs:       250223, items:       289505


#### Trivial Signal
== Aggregated report:
Items/sec min:         108 725
Items/sec median:      114 618
Items/sec max:         119 677

== Median report:
Items/sec:                        114618
Signals/sec:                       95326
Mutex contentions/sec:             54374
Thread sender  0:   sigs:      1000000, items:      1202370
Thread receiver  0: sigs:       248140, items:       303333
Thread receiver  1: sigs:       248280, items:       296124
Thread receiver  2: sigs:       256008, items:       303397
Thread receiver  3: sigs:       247572, items:       299516
```
</details>

---
**Micro load, 4 senders, 4 receivers, 1 000 000 signals, non-optimized send**

```
Command line args: -load micro -senders 4 -receivers 4 -sigs 1000000 -each 1
Run count: 5
Summary:
* 802 283 items per second;
* x3.039 of trivial signal;
```

<details><summary>Details</summary>

```
#### Canon Signal
== Aggregated report:
Items/sec min:         781 714
Items/sec median:      802 283
Items/sec max:         828 836

== Median report:
Items/sec:                        802283
Signals/sec:                       89208
Mutex contentions/sec:             51314
Thread sender  0:   sigs:       247339, items:      2240870
Thread sender  1:   sigs:       255382, items:      2284718
Thread sender  2:   sigs:       245694, items:      2211781
Thread sender  3:   sigs:       251585, items:      2256026
Thread receiver  0: sigs:       250553, items:      2259791
Thread receiver  1: sigs:       250579, items:      2252575
Thread receiver  2: sigs:       248990, items:      2254305
Thread receiver  3: sigs:       249878, items:      2226724


#### Trivial Signal
== Aggregated report:
Items/sec min:         257 947
Items/sec median:      264 009
Items/sec max:         270 388

== Median report:
Items/sec:                        264009
Signals/sec:                      140534
Mutex contentions/sec:            155503
Thread sender  0:   sigs:       248859, items:       468867
Thread sender  1:   sigs:       253055, items:       472842
Thread sender  2:   sigs:       247661, items:       466578
Thread sender  3:   sigs:       250425, items:       470319
Thread receiver  0: sigs:       251141, items:       464836
Thread receiver  1: sigs:       250496, items:       474661
Thread receiver  2: sigs:       250164, items:       474583
Thread receiver  3: sigs:       248199, items:       464526
```
</details>
