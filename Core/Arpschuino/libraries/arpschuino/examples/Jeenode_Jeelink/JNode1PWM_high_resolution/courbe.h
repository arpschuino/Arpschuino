const uint16_t level_12B[256]{0,1,
1,
1,
1,
2,
2,
3,
4,
5,
6,
8,
9,
11,
12,
14,
16,
18,
20,
23,
25,
28,
30,
33,
36,
39,
42,
46,
49,
53,
56,
60,
64,
68,
72,
77,
81,
86,
90,
95,
100,
105,
110,
116,
121,
127,
132,
138,
144,
150,
156,
163,
169,
176,
182,
189,
196,
203,
210,
218,
225,
233,
240,
248,
256,
264,
272,
281,
289,
298,
306,
315,
324,
333,
342,
352,
361,
371,
380,
390,
400,
410,
420,
431,
441,
452,
462,
473,
484,
495,
506,
518,
529,
541,
552,
564,
576,
588,
600,
613,
625,
638,
650,
663,
676,
689,
702,
716,
729,
743,
756,
770,
784,
798,
812,
827,
841,
856,
870,
885,
900,
915,
930,
946,
961,
977,
992,
1008,
1024,
1040,
1056,
1073,
1089,
1106,
1122,
1139,
1156,
1173,
1190,
1208,
1225,
1243,
1260,
1278,
1296,
1314,
1332,
1351,
1369,
1388,
1406,
1425,
1444,
1463,
1482,
1502,
1521,
1541,
1560,
1580,
1600,
1620,
1640,
1661,
1681,
1702,
1722,
1743,
1764,
1785,
1806,
1828,
1849,
1871,
1892,
1914,
1936,
1958,
1980,
2003,
2025,
2048,
2070,
2093,
2116,
2139,
2162,
2186,
2209,
2233,
2256,
2280,
2304,
2328,
2352,
2377,
2401,
2426,
2450,
2475,
2500,
2525,
2550,
2576,
2601,
2627,
2652,
2678,
2704,
2730,
2756,
2783,
2809,
2836,
2862,
2889,
2916,
2943,
2970,
2998,
3025,
3053,
3080,
3108,
3136,
3165,
3194,
3224,
3253,
3283,
3312,
3342,
3372,
3402,
3432,
3463,
3493,
3524,
3554,
3585,
3616,
3647,
3678,
3710,
3741,
3773,
3804,
3836,
3868,
3900,
3932,
3965,
3997,
4030,
4062,
4095};


const uint16_t level_13B[256]{ 
0  ,
6 ,
6 ,
6 ,
6 ,
6 ,
7 ,
7 ,
7 ,
7 ,
7 ,
8 ,
8 ,
8 ,
8 ,
9 ,
9 ,
9 ,
9 ,
10  ,
10  ,
10  ,
11  ,
11  ,
12  ,
13  ,
14  ,
15  ,
16  ,
17  ,
19  ,
20  ,
22  ,
23  ,
25  ,
27  ,
29  ,
31  ,
33  ,
35  ,
38  ,
40  ,
43  ,
46  ,
48  ,
52  ,
55  ,
58  ,
61  ,
65  ,
69  ,
73  ,
77  ,
81  ,
85  ,
90  ,
94  ,
99  ,
104 ,
109 ,
115 ,
120 ,
126 ,
132 ,
138 ,
144 ,
151 ,
158 ,
164 ,
171 ,
179 ,
186 ,
194 ,
202 ,
210 ,
218 ,
227 ,
236 ,
245 ,
254 ,
263 ,
273 ,
283 ,
293 ,
304 ,
315 ,
326 ,
337 ,
348 ,
360 ,
372 ,
384 ,
397 ,
410 ,
423 ,
436 ,
450 ,
464 ,
478 ,
492 ,
507 ,
522 ,
538 ,
553 ,
569 ,
586 ,
602 ,
619 ,
636 ,
654 ,
672 ,
690 ,
709 ,
727 ,
747 ,
766 ,
786 ,
806 ,
827 ,
848 ,
869 ,
891 ,
913 ,
935 ,
958 ,
981 ,
1004  ,
1028  ,
1052  ,
1077  ,
1102  ,
1127  ,
1153  ,
1179  ,
1205  ,
1232  ,
1260  ,
1287  ,
1315  ,
1344  ,
1373  ,
1402  ,
1432  ,
1462  ,
1493  ,
1524  ,
1555  ,
1587  ,
1619  ,
1652  ,
1685  ,
1719  ,
1753  ,
1787  ,
1822  ,
1858  ,
1894  ,
1930  ,
1967  ,
2004  ,
2042  ,
2080  ,
2119  ,
2158  ,
2197  ,
2238  ,
2278  ,
2319  ,
2361  ,
2403  ,
2446  ,
2489  ,
2532  ,
2576  ,
2621  ,
2666  ,
2712  ,
2758  ,
2804  ,
2852  ,
2899  ,
2948  ,
2996  ,
3046  ,
3096  ,
3146  ,
3197  ,
3248  ,
3301  ,
3353  ,
3406  ,
3460  ,
3514  ,
3569  ,
3625  ,
3681  ,
3737  ,
3794  ,
3852  ,
3910  ,
3969  ,
4029  ,
4089  ,
4149  ,
4211  ,
4272  ,
4335  ,
4398  ,
4462  ,
4526  ,
4591  ,
4656  ,
4723  ,
4789  ,
4857  ,
4925  ,
4993  ,
5063  ,
5133  ,
5203  ,
5274  ,
5346  ,
5419  ,
5492  ,
5566  ,
5640  ,
5715  ,
5791  ,
5868  ,
5945  ,
6023  ,
6101  ,
6180  ,
6260  ,
6341  ,
6422  ,
6504  ,
6587  ,
6670  ,
6754  ,
6839  ,
6924  ,
7010  ,
7097  ,
7185  ,
7273  ,
7362  ,
7452  ,
7542  ,
7633  ,
7725  ,
7818  ,
7911  ,
8005  ,
8100  ,
8191  };


const uint16_t level_14B[256]{
0  ,
0 ,
0 ,
0 ,
1 ,
1 ,
1 ,
1 ,
1 ,
2 ,
2 ,//10
2 ,
2 ,
3 ,
3 ,
3 ,
4 ,
5 ,
6 ,
7 ,
8 ,//20
9 ,
11  ,
13  ,
14  ,
16  ,
18  ,
20  ,
23  ,
25  ,
28  ,
31  ,
34  ,
37  ,
41  ,
45  ,
48  ,
53  ,
57  ,
62  ,
66  ,
71  ,
77  ,
82  ,
88  ,
94  ,
100 ,
107 ,
114 ,
121 ,
129 ,
136 ,
144 ,
153 ,
161 ,
171 ,
180 ,
190 ,
200 ,
210 ,
221 ,
232 ,
243 ,
255 ,
267 ,
280 ,
293 ,
306 ,
320 ,
334 ,
349 ,
364 ,
379 ,
395 ,
411 ,
428 ,
445 ,
462 ,
480 ,
499 ,
518 ,
537 ,
557 ,
578 ,
599 ,
620 ,
642 ,
665 ,
687 ,
711 ,
735 ,
759 ,
785 ,
810 ,
836 ,
863 ,
890 ,
918 ,
947 ,
976 ,
1005  ,
1035  ,
1066  ,
1098  ,
1129  ,
1162  ,
1195  ,
1229  ,
1264  ,
1299  ,
1335  ,
1371  ,
1408  ,
1446  ,
1484  ,
1523  ,
1563  ,
1604  ,
1645  ,
1687  ,
1729  ,
1772  ,
1816  ,
1861  ,
1906  ,
1952  ,
1999  ,
2047  ,
2095  ,
2145  ,
2194  ,
2245  ,
2296  ,
2349  ,
2402  ,
2456  ,
2510  ,
2565  ,
2622  ,
2679  ,
2737  ,
2795  ,
2855  ,
2915  ,
2976  ,
3038  ,
3101  ,
3165  ,
3229  ,
3295  ,
3361  ,
3429  ,
3497  ,
3566  ,
3636  ,
3706  ,
3778  ,
3851  ,
3924  ,
3999  ,
4074  ,
4151  ,
4228  ,
4307  ,
4386  ,
4466  ,
4547  ,
4630  ,
4713  ,
4797  ,
4882  ,
4968  ,
5055  ,
5144  ,
5233  ,
5323  ,
5414  ,
5507  ,
5600  ,
5694  ,
5790  ,
5886  ,
5984  ,
6083  ,
6182  ,
6283  ,
6385  ,
6488  ,
6592  ,
6697  ,
6804  ,
6911  ,
7020  ,
7129  ,
7240  ,
7352  ,
7465  ,
7579  ,
7695  ,
7812  ,
7929  ,
8048  ,
8168  ,
8290  ,
8412  ,
8536  ,
8661  ,
8787  ,
8914  ,
9043  ,
9173  ,
9304  ,
9436  ,
9570  ,
9704  ,
9841  ,
9978  ,
10116 ,
10256 ,
10397 ,
10540 ,
10684 ,
10829 ,
10975 ,
11123 ,
11272 ,
11422 ,
11574 ,
11727 ,
11881 ,
12036 ,
12194 ,
12352 ,
12512 ,
12673 ,
12835 ,
12999 ,
13164 ,
13331 ,
13499 ,
13668 ,
13839 ,
14012 ,
14185 ,
14360 ,
14537 ,
14715 ,
14895 ,
15075 ,
15258 ,
15442 ,
15627 ,
15814 ,
16002 ,
16192 ,
16383 };

//*/


const uint16_t level_16B[256]{0 ,1 ,
2  ,
3  ,
4  ,
5 ,
6 ,
7 ,
8 ,
9 ,
10 ,
11 ,
12 ,
13  ,
14  ,
15  ,
18  ,
22  ,
26  ,
30  ,
35  ,
41  ,
47  ,
53  ,
60  ,
68  ,
76  ,
85  ,
94  ,
104 ,
115 ,
127 ,
139 ,
153 ,
166 ,
181 ,
197 ,
213 ,
231 ,
249 ,
268 ,
288 ,
310 ,
332 ,
355 ,
379 ,
405 ,
431 ,
459 ,
487 ,
517 ,
548 ,
581 ,
614 ,
649 ,
685 ,
722 ,
761 ,
801 ,
843 ,
886 ,
930 ,
976 ,
1023  ,
1072  ,
1122  ,
1174  ,
1227  ,
1282  ,
1339  ,
1397  ,
1457  ,
1519  ,
1582  ,
1647  ,
1714  ,
1782  ,
1853  ,
1925  ,
1999  ,
2075  ,
2153  ,
2233  ,
2314  ,
2398  ,
2484  ,
2571  ,
2661  ,
2753  ,
2847  ,
2943  ,
3041  ,
3141  ,
3243  ,
3348  ,
3455  ,
3564  ,
3676  ,
3789  ,
3905  ,
4024  ,
4144  ,
4267  ,
4393  ,
4521  ,
4651  ,
4784  ,
4920  ,
5058  ,
5198  ,
5341  ,
5487  ,
5635  ,
5786  ,
5940  ,
6096  ,
6255  ,
6417  ,
6582  ,
6749  ,
6919  ,
7092  ,
7268  ,
7447  ,
7628  ,
7813  ,
8000  ,
8191  ,
8385  ,
8581  ,
8781  ,
8983  ,
9189  ,
9398  ,
9610  ,
9825  ,
10043 ,
10265 ,
10490 ,
10718 ,
10949 ,
11184 ,
11422 ,
11663 ,
11908 ,
12156 ,
12407 ,
12662 ,
12921 ,
13183 ,
13448 ,
13717 ,
13990 ,
14266 ,
14545 ,
14829 ,
15116 ,
15406 ,
15701 ,
15999 ,
16301 ,
16607 ,
16916 ,
17229 ,
17546 ,
17867 ,
18192 ,
18521 ,
18854 ,
19190 ,
19531 ,
19876 ,
20224 ,
20577 ,
20934 ,
21295 ,
21660 ,
22029 ,
22403 ,
22780 ,
23162 ,
23548 ,
23938 ,
24333 ,
24732 ,
25135 ,
25543 ,
25955 ,
26371 ,
26792 ,
27217 ,
27647 ,
28081 ,
28520 ,
28963 ,
29411 ,
29864 ,
30321 ,
30783 ,
31249 ,
31720 ,
32196 ,
32676 ,
33162 ,
33652 ,
34147 ,
34646 ,
35151 ,
35660 ,
36175 ,
36694 ,
37218 ,
37747 ,
38282 ,
38821 ,
39365 ,
39914 ,
40469 ,
41028 ,
41593 ,
42163 ,
42737 ,
43318 ,
43903 ,
44494 ,
45090 ,
45691 ,
46297 ,
46909 ,
47526 ,
48149 ,
48777 ,
49410 ,
50049 ,
50694 ,
51344 ,
51999 ,
52660 ,
53327 ,
53999 ,
54677 ,
55360 ,
56049 ,
56744 ,
57445 ,
58151 ,
58863 ,
59581 ,
60305 ,
61034 ,
61770 ,
62511 ,
63258 ,
64011 ,
64770 ,
65535 
 };
//*/
 
