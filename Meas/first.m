%   _____ ____  __  __ ______ _____   _____ _____          _      
%  / ____/ __ \|  \/  |  ____|  __ \ / ____|_   _|   /\   | |     
% | |   | |  | | \  / | |__  | |__) | |      | |    /  \  | |     
% | |   | |  | | |\/| |  __| |  _  /| |      | |   / /\ \ | |     
% | |___| |__| | |  | | |____| | \ \| |____ _| |_ / ____ \| |____ 
%  \_____\____/|_|  |_|______|_|  \_\\_____|_____/_/    \_\______|

m1_prop = "6x4.5 triple";
m1_motor = "Tarot 1900kV";
m1_inv = "comercial";
m1_comm = "6-Step";
m1_U = 10;
m1_I = [0.035 0.148 0.514 1.19 2.15 3.34 4.76 6.57 9.01 10.5 12 13.1 13.8 14.5 15.4];
m1_in = [8 16 32 48 64 80 96 112 128 144 160 176 192 208 224];
m1_In = (m1_in*100)/256;
m1_Thrust = [     0    10    24    50    73   103   137   181   215   255   301   349   410   461   523];
m1_rpm = [ 334        1269        2606        3780        4826        5769        6651        7500        8203        8923        9694       10534       11357       12043       12703];
m1_rpm_dev = [  8    18    31    52    69    81    87    97   105   123   141   147   174   216   209];
m1_inv_temp = [ 31    31    31    31    33    34    36    40    44    50    55    61    68    71    81
];
m1_mot_temp = [27    28    30    32    35    38    43    48    54    62    72    85   103   125   150];
m1_p = m1_I * m1_U;
m1_eff = m1_Thrust./m1_p;

m2_prop = "7x6 duo";
m2_motor = "Tarot 1900kV";
m2_inv = "comercial";
m2_comm = "6-Step";
m2_U = 10;
m2_I = [ 0.0370    0.1820    0.7400    1.7600    3.4200    5.2500    7.1900    9.5000   11.5000   13.8000   15.7000   16.5000   17.1000];
m2_in = [8    16    32    48    64    80    96   112   128   144   160   176   192];
m2_In = (m2_in*100)/256;
m2_Thrust = [ 0     6    26    47    87   119   159   201   239   273   318   370   420];
m2_rpm = [ 326        1217        2391        3369        4406        5186        5889        6591        7191        7714        8246        8889        9463];
m2_rpm_dev = [ 5    14    29    47    64    75    80    81    99   123   149   188   172];
m2_inv_temp = [32    31    31    32    34    37    40    43    49    58    65    74    77];
m2_mot_temp = [ 29    29    30    32    37    42    48    56    67    79    93   118   150];
m2_p = m2_I * m2_U;
m2_eff = m2_Thrust./m2_p;

m3_prop = "8x4.5 duo";
m3_motor = "Tarot 1900kV";
m3_inv = "comercial";
m3_comm = "6-Step";
m3_U = 10;
m3_I = [ 0.0370    0.1730    0.7270    1.8200    3.6900    5.7900    7.9000   10.4000   12.9000   15.0000   16.9000   18.7000   18.7000];
m3_in = [ 8    16    32    48    64    80    96   112   128   144   160   176   180];
m3_In = (m3_in*100)/256;
m3_Thrust = [ 0     7    33    71   121   163   205   253   300   346   389   446   454];
m3_rpm = [326        1209        2297        3326        4286        4989        5614        6223        6711        7183        7594        8100        8169];
m3_rpm_dev = [  6    13    27    46    61    64    67    75    90   105   106   111   121];
m3_inv_temp = [  32    31    31    31    34    37    40    46    51    60    67    75    79];
m3_mot_temp = [    29    29    30    33    39    45    53    63    77    92   112   141   151];
m3_p = m3_I * m3_U;
m3_eff = m3_Thrust./m3_p;

%  ____  ______ __  __ ______             __        _____ _______ ______ _____  
% |  _ \|  ____|  \/  |  ____|    _      / /       / ____|__   __|  ____|  __ \ 
% | |_) | |__  | \  / | |__     _| |_   / /_ _____| (___    | |  | |__  | |__) |
% |  _ <|  __| | |\/| |  __|   |_   _| | '_ \______\___ \   | |  |  __| |  ___/ 
% | |_) | |____| |  | | |        |_|   | (_) |     ____) |  | |  | |____| |     
% |____/|______|_|  |_|_|               \___/     |_____/   |_|  |______|_|     
                                                                           
m4_prop = "6x4.5 triple";
m4_motor = "Tarot 1900kV";
m4_inv = "uber";
m4_comm = "6-Step";
m4_U = 10;
m4_I = [5.6700    6.6400    9.1200   12.1500   13.0200   14.7400   14.7000];
m4_in = [164   176   192   208   224   240   256];
m4_In = (m4_in*100)/256;
m4_Thrust = [  227   279   358   425   481   516   516];
m4_rpm = [8700        9454       10706       11657       12386       12926       12926];
m4_rpm_dev = [183   124    88   108   102   107   106];
m4_inv_temp = [ 79    76    85    68    70    72    70];
m4_mot_temp = [76    76    98   116   126   142   141];
m4_p = m4_I * m4_U;
m4_eff = m4_Thrust./m4_p;

m5_prop = "6x4.5 triple";
m5_motor = "Tarot 1900kV";
m5_inv = "developed";
m5_comm = "6-Step";
m5_shift = 30;
m5_U = 10;
m5_I = [6.7100    7.1100    9.8700   11.9600   13.4200   15.1900   15.1300];
m5_in = [164   176   192   208   224   240   256];
m5_In = (m5_in*100)/256;
m5_Thrust = [234   284   361   424   479   513   515];
m5_rpm = [8760        9643       10740       11734       12403       12926       12926];
m5_rpm_dev = [123    88    83    89   103   109   104];
m5_inv_temp = [80    82    89    68    71    74    73];
m5_mot_temp = [89    87   106   124   134   149   150];
m5_p = m5_I * m5_U;
m5_eff = m5_Thrust./m5_p;

m6_prop = "7x6 duo";
m6_motor = "Tarot 1900kV";
m6_inv = "developed";
m6_comm = "6-Step";
m6_U = 10;
m6_I = [ 5.8700    9.2200   12.1000];
m6_in = [ 160   176   192];
m6_In = (m6_in*100)/256;
m6_Thrust = [  208   290   368];
m6_rpm = [6849        7929        8846];
m6_rpm_dev = [ 79    91    78];
m6_inv_temp = [81    99   107];
m6_mot_temp = [92   110   149];
m6_p = m6_I * m6_U;
m6_eff = m6_Thrust./m6_p;

m7_prop = "7x6 duo";
m7_motor = "Tarot 1900kV";
m7_inv = "developed";
m7_comm = "6-Step";
m7_shift = 30;
m7_U = 10;
m7_I = [6.3000    8.3500    9.7000   10.0000];
m7_in = [148   160   176   184];
m7_In = (m7_in*100)/256;
m7_Thrust = [ 192   215   288   315];
m7_rpm = [6566        6994        7877        8229];
m7_rpm_dev = [76   163   103    57];
m7_inv_temp = [86    95   107   101];
m7_mot_temp = [ 106   135   142   149];
m7_p = m7_I * m7_U;
m7_eff = m7_Thrust./m7_p;

m8_prop = "8x4.5 duo";
m8_motor = "Tarot 1900kV";
m8_inv = "developed";
m8_comm = "6-Step";
m8_U = 10;
m8_I = [6.4000    9.5000   10.8000];
m8_in = [160   176   184];
m8_In = (m8_in*100)/256;
m8_Thrust = [261   346   393];
m8_rpm = [6497        7406        7834];
m8_rpm_dev = [56    81    58];
m8_inv_temp = [76    94    98];
m8_mot_temp = [97   128   150];
m8_p = m8_I * m8_U;
m8_eff = m8_Thrust./m8_p;

m9_prop = "8x4.5 duo";
m9_motor = "Tarot 1900kV";
m9_inv = "developed";
m9_comm = "6-Step";
m9_U = 10;
m9_I = [ 8.7000   10.3000];
m9_in = [164   176];
m9_In = (m9_in*100)/256;
m9_Thrust = [ 293   317];
m9_rpm = [6746        7054];
m9_rpm_dev = [59   104];
m9_inv_temp = [94   102];
m9_mot_temp = [125   155];
m9_p = m9_I * m9_U;
m9_eff = m9_Thrust./m9_p;

%   _    _          _      _                  __        _____ _______ ______ _____  
%  | |  | |   /\   | |    | |         _      / /       / ____|__   __|  ____|  __ \ 
%  | |__| |  /  \  | |    | |       _| |_   / /_ _____| (___    | |  | |__  | |__) |
%  |  __  | / /\ \ | |    | |      |_   _| | '_ \______\___ \   | |  |  __| |  ___/ 
%  | |  | |/ ____ \| |____| |____    |_|   | (_) |     ____) |  | |  | |____| |     
%  |_|  |_/_/    \_\______|______|          \___/     |_____/   |_|  |______|_|     
                                                                                

m10_prop = "6x4.5 triple";
m10_motor = "Tarot 1900kV";
m10_inv = "uber";
m10_comm = "6-Step";
m10_U = 10;
m10_I = [ 0.2070    0.5220    0.9980    1.6000    2.4200    3.2200    4.0900    4.9700    6.4200    9.1000   12.2000   13.4000];
m10_in = [ 48    64    80    96   112   128   144   160   176   192   208   224];
m10_In = (m10_in*100)/256;
m10_Thrust = [1    13    43    69    96   121   161   206   254   327   392   441];
m10_rpm = [283        1886        3703        4680        5511        6369        7329        8220        9086       10286       11194       11949];
m10_rpm_dev = [4    25    51    63    75    89   106   117   135   153   174   193];
m10_inv_temp = [37    41    45    49    57    60    64    69    75    88    80    85];
m10_mot_temp = [32    33    34    36    43    47    53    60    69    92   124   146];
m10_p = m10_I * m10_U;
m10_eff = m10_Thrust./m10_p;

m11_prop = "6x4.5 triple";
m11_motor = "Tarot 1900kV";
m11_inv = "developed";
m11_comm = "6-Step";
m11_shift = 30;
m11_U = 10;
m11_I = [0.2240    0.2380    1.2100    1.7900    2.5500    3.3300    4.2600    5.2300    6.5100   10.0000   12.1000   13.6000];
m11_in = [48    64    80    96   112   128   144   160   176   192   208   224];
m11_In = (m11_in*100)/256;
m11_Thrust = [ 1    15    45    71   100   136   180   223   271   365   418   465];
m11_rpm = [257        2040        4037        4971        5846        6754        7517        8571        9146       10483       11597       12274];
m11_rpm_dev = [4    26    55    68    79    88    99   115   125   146   159   183];
m11_inv_temp = [34    41    46    51    56    62    66    72    77    90    75    80];
m11_mot_temp = [29    32    26    40    44    49    55    61    70    97   115   137];
m11_p = m11_I * m11_U;
m11_eff = m11_Thrust./m11_p;

m12_prop = "7x6 duo";
m12_motor = "Tarot 1900kV";
m12_inv = "developed";
m12_comm = "6-Step";
m12_U = 10;
m12_I = [0.2690    0.6450    1.2500    2.1600    3.1300    3.9500    5.1600    6.5800    8.4100   11.6000];
m12_in = [48    64    80    96   112   128   144   160   176   192];
m12_In = (m12_in*100)/256;
m12_Thrust = [1    18    43    71   105   139   175   214   260   317];
m12_rpm = [343        1954        3111        3951        4774        5323        6197        6874        7534        8280];
m12_rpm_dev = [5    26    43    56    65    74    81    93   120   120];
m12_inv_temp = [36    42    45    51    57    62    69    75    84   101];
m12_mot_temp = [ 31    32    35    41    48    55    66    80   103   145];
m12_p = m12_I * m12_U;
m12_eff = m12_Thrust./m12_p;

m13_prop = "7x6 duo";
m13_motor = "Tarot 1900kV";
m13_inv = "developed";
m13_comm = "6-Step";
m13_shift = 30;
m13_U = 10;
m13_I = [ 0.2630    0.6100    1.3400    2.3000    3.1600    4.0900    5.3100    6.6200    8.5100   11.6000];
m13_in = [  48    64    80    96   112   128   144   160   176   192];
m13_In = (m13_in*100)/256;
m13_Thrust = [1    15    44    81   113   149   189   231   279   344];
m13_rpm = [386        1860        3240        4251        5040        5760        6480        7149        7791        8554];
m13_rpm_dev = [ 5    25    45    59    67    74    78    93   112   137];
m13_inv_temp = [39    43    48    56    62    69    74    85    93   104];
m13_mot_temp = [34    34    37    44    51    59    69    87   104   141];
m13_p = m13_I * m13_U;
m13_eff = m13_Thrust./m13_p;

m14_prop = "8x4.5 duo";
m14_motor = "Tarot 1900kV";
m14_inv = "developed";
m14_comm = "6-Step";
m14_U = 10;
m14_I = [0.2490    0.5940    1.3500    2.1800    3.1500    4.1500    5.4400    6.7500    8.9300   10.6000];
m14_in = [48    64    80    96   112   128   144   160   176   184];
m14_In = (m14_in*100)/256;
m14_Thrust = [1    17    58    92   136   184   233   276   337   368];
m14_rpm = [283        1723        3077        3797        4620        5331        6034        6574        7234        7491];
m14_rpm_dev = [4    22    39    59    62    69    80    76    98   111];
m14_inv_temp = [39    41    46    50    55    63    69    75    86    98];
m14_mot_temp = [ 36    36    37    42    48    58    70    87   118   150];
m14_p = m14_I * m14_U;
m14_eff = m14_Thrust./m14_p;

m15_prop = "8x4.5 duo";
m15_motor = "Tarot 1900kV";
m15_inv = "developed";
m15_comm = "6-Step";
m15_U = 10;
m15_I = [290.0000  672.0000    1.3700    2.2400    3.1200    4.3000    5.5100    6.9100    8.7200   11.0000];
m15_in = [48    64    80    96   112   128   144   160   176   184];
m15_In = (m15_in*100)/256;
m15_Thrust = [1    22    63   103   145   202   252   303   349   433];
m15_rpm = [437        2040        3146        4011        4774        5623        6231        6814        7371        7817];
m15_rpm_dev = [ 6    27    43    56    63    72    77    69    97   111];
m15_inv_temp = [33    38    45    50    56    63    70    79    86    98];
m15_mot_temp = [31    32    36    42    49    59    72    90   110   145];
m15_p = m15_I * m15_U;
m15_eff = m15_Thrust./m15_p;

%   _    _          _      _                 _______      ____  __ 
%  | |  | |   /\   | |    | |         _     / ____\ \    / /  \/  |
%  | |__| |  /  \  | |    | |       _| |_  | (___  \ \  / /| \  / |
%  |  __  | / /\ \ | |    | |      |_   _|  \___ \  \ \/ / | |\/| |
%  | |  | |/ ____ \| |____| |____    |_|    ____) |  \  /  | |  | |
%  |_|  |_/_/    \_\______|______|         |_____/    \/   |_|  |_|

m16_prop = "6x4.5 triple";
m16_motor = "Tarot 1900kV";
m16_inv = "uber";
m16_comm = "6-Step";
m16_U = 10;
m16_I = [0.184	0.46	0.662	0.993	1.47	1.99	2.83	3.55	4.7	5.7	6.87	8.45	10.1	10.6];
m16_in = [16	32	48	64	80	96	112	128	144	160	176	192	208	216];
m16_In = (m16_in*100)/256;
m16_Thrust = [0	9	23	48	77	109	144	170	203	238	262	299	326	329];
m16_rpm = [163	2160	3214	4243	5211	6077	6900	7500	8254	8811	9394	9926	10277	10423];
m16_rpm_dev = [5	31	48	66	79	96	119	143	151	146	163	132	180	189];
m16_inv_temp = [37	36	35	36	37	38	41	45	50	55	60	66	75	81];
m16_mot_temp = [36	35	34	35	37	41	49	56	69	78	91	112	138	150];
m16_p = m16_I * m16_U;
m16_eff = m16_Thrust./m16_p;

m17_prop = "6x4.5 triple";
m17_motor = "Tarot 1900kV";
m17_inv = "developed";
m17_comm = "svm";
m17_shift = 30;
m17_U = 10;
m17_I = [0.208	0.445	0.689	1	1.51	2.1	2.75	3.71	4.44	5.82	6.9	8.16	9.84	11.53	12.4];
m17_in = [16	32	48	64	80	96	112	128	144	160	176	192	208	224	232];
m17_In = (m17_in*100)/256;
m17_Thrust = [0	7	27	48	80	113	145	184	213	261	283	315	345	374	387];
m17_rpm = [274	2006	3231	4209	5254	6120	6857	7706	8289	9034	9523	10011	10534	10937	11160];
m17_rpm_dev = [6	27	43	60	74	94	103	120	129	154	163	154	171	170	171];
m17_inv_temp = [35	35	35	35	36	38	40	43	47	52	58	64	72	80	86];
m17_mot_temp = [34	35	35	35	38	41	45	52	58	67	80	94	115	137	150];
m17_p = m17_I * m17_U;
m17_eff = m17_Thrust./m17_p;

m18_prop = "7x6 duo";
m18_motor = "Tarot 1900kV";
m18_inv = "developed";
m18_comm = "svm";
m18_U = 10;
m18_I = [0.187	0.425	0.735	1.18	1.77	2.48	3.44	4.48	5.78	7.31	9.02];
m18_in = [16	32	48	64	80	96	112	128	144	160	176];
m18_In = (m18_in*100)/256;
m18_Thrust = [1	16	37	63	91	124	155	189	217	248	274];
m18_rpm = [291	1817	2863	3711	4491	5229	5871	6446	6909	7329	7680];
m18_rpm_dev = [4	28	45	64	86	94	111	137	137	146	163];
m18_inv_temp = [33	33	34	45	37	43	46	52	59	70	77];
m18_mot_temp = [29	30	32	35	40	48	58	76	96	123	150];
m18_p = m18_I * m18_U;
m18_eff = m18_Thrust./m18_p;

m19_prop = "7x6 duo";
m19_motor = "Tarot 1900kV";
m19_inv = "developed";
m19_comm = "svm";
m19_shift = 30;
m19_U = 10;
m19_I = [0.191	0.442	0.753	1.2	1.78	2.5	3.41	4.51	5.74	6.95	8.68	10.3];
m19_in = [16	32	48	64	80	96	112	128	144	160	176	192];
m19_In = (m19_in*100)/256;
m19_Thrust = [1	13	39	65	95	128	163	200	235	261	296	325];
m19_rpm = [274	1800	2863	3737	4526	5271	5914	6514	7020	7449	7894	8263];
m19_rpm_dev = [4	29	46	63	82	94	111	129	137	154	163	189];
m19_inv_temp = [30	33	34	36	38	42	46	54	58	65	73	81];
m19_mot_temp = [28	30	32	35	40	47	55	66	80	98	120	145];
m19_p = m19_I * m19_U;
m19_eff = m19_Thrust./m19_p;

m20_prop = "8x4.5 duo";
m20_motor = "Tarot 1900kV";
m20_inv = "developed";
m20_comm = "svm";
m20_U = 10;
m20_I = [0.19	0.428	0.759	1.22	1.83	2.26	3.62	4.73	6.13	7.75	8.47];
m20_in = [16	32	48	64	80	96	112	128	144	160	168];
m20_In = (m20_in*100)/256;
m20_Thrust = [1	15	45	80	121	162	207	242	275	302	317];
m20_rpm = [291	1800	2811	3651	4397	5083	5691	6214	6617	6951	7097];
m20_rpm_dev = [4	27	46	57	83	103	120	137	154	197	180];
m20_inv_temp = [28	28	31	33	36	39	43	49	57	65	72];
m20_mot_temp = [29	30	32	35	42	50	62	80	102	132	150];
m20_p = m20_I * m20_U;
m20_eff = m20_Thrust./m20_p;

m21_prop = "8x4.5 duo";
m21_motor = "Tarot 1900kV";
m21_inv = "developed";
m21_comm = "svm";
m21_U = 10;
m21_I = [0.2	0.446	0.778	1.37	1.86	2.64	3.66	4.86	6.1	7.62	9.18	10];
m21_in = [16	32	48	64	80	96	112	128	144	160	176	184];
m21_In = (m21_in*100)/256;
m21_Thrust = [1	17	47	108	139	182	219	251	289	331	369	386];
m21_rpm = [274	1783	2820	3823	4414	5117	5726	6283	6703	7106	7483	7637];
m21_rpm_dev = [4	27	45	69	81	100	123	135	189	189	180	171];
m21_inv_temp = [32	33	32	34	36	40	45	49	57	63	72	77];
m21_mot_temp = [32	32	33	37	40	48	58	70	88	110	138	150];
m21_p = m21_I * m21_U;
m21_eff = m21_Thrust./m21_p;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

close all;

figure(1);

subplot(2,2,1);
plot(m1_In,m1_Thrust);
hold on;
plot(m2_In,m2_Thrust);
hold on;
plot(m3_In,m3_Thrust);
grid on;
legend({m1_prop,m2_prop,m3_prop},'Location','southeast');
title("Input pover vs Thrust");
xlabel("PWM duty");
ylabel("Thrust [g]");

subplot(2,2,2);
plot(m1_In,m1_rpm);
hold on;
plot(m2_In,m2_rpm);
hold on;
plot(m3_In,m3_rpm);
grid on;
legend({m1_prop,m2_prop,m3_prop},'Location','southeast');
title("Input pover vs RPM");
xlabel("PWM duty");
ylabel("Rotations [RPM]");

subplot(2,2,3);
plot(m1_In,m1_mot_temp);
hold on;
plot(m2_In,m2_mot_temp);
hold on;
plot(m3_In,m3_mot_temp);
grid on;
legend({m1_prop,m2_prop,m3_prop},'Location','southeast');
title("Input pover vs motor temperature");
xlabel("PWM duty");
ylabel("Motor temperature [°C]");

subplot(2,2,4);
plot(m1_In,m1_eff);
hold on;
plot(m2_In,m2_eff);
hold on;
plot(m3_In,m3_eff);
grid on;
legend({m1_prop,m2_prop,m3_prop},'Location','southeast');
title("Input pover vs Efficency");
xlabel("PWM duty");
ylabel("Efficency [g/W]");

figure(2);

subplot(2,2,1);
plot(m4_In,m4_Thrust);
hold on;
plot(m6_In,m6_Thrust);
hold on;
plot(m8_In,m8_Thrust);
grid on;
legend({m4_prop,m6_prop,m8_prop},'Location','southeast');
title("Input pover vs Thrust");
xlabel("PWM duty");
ylabel("Thrust [g]");

subplot(2,2,2);
plot(m4_In,m4_rpm);
hold on;
plot(m6_In,m6_rpm);
hold on;
plot(m8_In,m8_rpm);
grid on;
legend({m4_prop,m6_prop,m8_prop},'Location','southeast');
title("Input pover vs RPM");
xlabel("PWM duty");
ylabel("Rotations [RPM]");

subplot(2,2,3);
plot(m4_In,m4_mot_temp);
hold on;
plot(m6_In,m6_mot_temp);
hold on;
plot(m8_In,m8_mot_temp);
grid on;
legend({m4_prop,m6_prop,m8_prop},'Location','southeast');
title("Input pover vs motor temperature");
xlabel("PWM duty"); 
ylabel("Motor temperature [°C]");

subplot(2,2,4);
plot(m4_In,m4_eff);
hold on;
plot(m6_In,m6_eff);
hold on;
plot(m8_In,m8_eff);
grid on;
legend({m4_prop,m6_prop,m8_prop},'Location','southeast');
title("Input pover vs Efficency");
xlabel("PWM duty");
ylabel("Efficency [g/W]");

figure(3);

subplot(2,2,1);
plot(m5_In,m5_Thrust);
hold on;
plot(m7_In,m7_Thrust);
hold on;
plot(m9_In,m9_Thrust);
grid on;
legend({m5_prop,m7_prop,m9_prop},'Location','southeast');
title("Input pover vs Thrust");
xlabel("PWM duty");
ylabel("Thrust [g]");

subplot(2,2,2);
plot(m5_In,m5_rpm);
hold on;
plot(m7_In,m7_rpm);
hold on;
plot(m9_In,m9_rpm);
grid on;
legend({m5_prop,m7_prop,m9_prop},'Location','southeast');
title("Input pover vs RPM");
xlabel("PWM duty");
ylabel("Rotations [RPM]");

subplot(2,2,3);
plot(m5_In,m5_mot_temp);
hold on;
plot(m7_In,m7_mot_temp);
hold on;
plot(m9_In,m9_mot_temp);
grid on;
legend({m5_prop,m7_prop,m9_prop},'Location','southeast');
title("Input pover vs motor temperature");
xlabel("PWM duty");
ylabel("Motor temperature [°C]");

subplot(2,2,4);
plot(m5_In,m5_eff);
hold on;
plot(m7_In,m7_eff);
hold on;
plot(m9_In,m9_eff);
grid on;
legend({m5_prop,m7_prop,m9_prop},'Location','southeast');
title("Input pover vs Efficency");
xlabel("PWM duty");
ylabel("Efficency [g/W]");

figure(4);

subplot(2,2,1);
plot(m10_In,m10_Thrust);
hold on;
plot(m12_In,m12_Thrust);
hold on;
plot(m14_In,m14_Thrust);
grid on;
legend({m10_prop,m12_prop,m14_prop},'Location','southeast');
title("Input pover vs Thrust");
xlabel("PWM duty");
ylabel("Thrust [g]");

subplot(2,2,2);
plot(m10_In,m10_rpm);
hold on;
plot(m12_In,m12_rpm);
hold on;
plot(m14_In,m14_rpm);
grid on;
legend({m10_prop,m12_prop,m14_prop},'Location','southeast');
title("Input pover vs RPM");
xlabel("PWM duty");
ylabel("Rotations [RPM]");

subplot(2,2,3);
plot(m10_In,m10_mot_temp);
hold on;
plot(m12_In,m12_mot_temp);
hold on;
plot(m14_In,m14_mot_temp);
grid on;
legend({m10_prop,m12_prop,m14_prop},'Location','southeast');
title("Input pover vs motor temperature");
xlabel("PWM duty");
ylabel("Motor temperature [°C]");

subplot(2,2,4);
plot(m10_In,m10_eff);
hold on;
plot(m12_In,m12_eff);
hold on;
plot(m14_In,m14_eff);
grid on;
legend({m10_prop,m12_prop,m14_prop},'Location','southeast');
title("Input pover vs Efficency");
xlabel("PWM duty");
ylabel("Efficency [g/W]");

figure(5);

subplot(2,2,1);
plot(m11_In,m11_Thrust);
hold on;
plot(m13_In,m13_Thrust);
hold on;
plot(m15_In,m15_Thrust);
grid on;
legend({m11_prop,m13_prop,m15_prop},'Location','southeast');
title("Input pover vs Thrust");
xlabel("PWM duty");
ylabel("Thrust [g]");

subplot(2,2,2);
plot(m11_In,m11_rpm);
hold on;
plot(m13_In,m13_rpm);
hold on;
plot(m15_In,m15_rpm);
grid on;
legend({m11_prop,m13_prop,m15_prop},'Location','southeast');
title("Input pover vs RPM");
xlabel("PWM duty");
ylabel("Rotations [RPM]");

subplot(2,2,3);
plot(m11_In,m11_mot_temp);
hold on;
plot(m13_In,m13_mot_temp);
hold on;
plot(m15_In,m15_mot_temp);
grid on;
legend({m11_prop,m13_prop,m15_prop},'Location','southeast');
title("Input pover vs motor temperature");
xlabel("PWM duty");
ylabel("Motor temperature [°C]");

subplot(2,2,4);
plot(m11_In,m11_eff);
hold on;
plot(m13_In,m13_eff);
hold on;
plot(m15_In,m15_eff);
grid on;
legend({m11_prop,m13_prop,m15_prop},'Location','southeast');
title("Input pover vs Efficency");
xlabel("PWM duty");
ylabel("Efficency [g/W]");

figure(6);

subplot(2,2,1);
plot(m16_In,m16_Thrust);
hold on;
plot(m18_In,m18_Thrust);
hold on;
plot(m20_In,m20_Thrust);
grid on;
legend({m16_prop,m18_prop,m20_prop},'Location','southeast');
title("Input pover vs Thrust");
xlabel("PWM duty");
ylabel("Thrust [g]");

subplot(2,2,2);
plot(m16_In,m16_rpm);
hold on;
plot(m18_In,m18_rpm);
hold on;
plot(m20_In,m20_rpm);
grid on;
legend({m16_prop,m18_prop,m20_prop},'Location','southeast');
title("Input pover vs RPM");
xlabel("PWM duty");
ylabel("Rotations [RPM]");

subplot(2,2,3);
plot(m16_In,m16_mot_temp);
hold on;
plot(m18_In,m18_mot_temp);
hold on;
plot(m20_In,m20_mot_temp);
grid on;
legend({m16_prop,m18_prop,m20_prop},'Location','southeast');
title("Input pover vs motor temperature");
xlabel("PWM duty");
ylabel("Motor temperature [°C]");

subplot(2,2,4);
plot(m16_In,m16_eff);
hold on;
plot(m18_In,m18_eff);
hold on;
plot(m20_In,m20_eff);
grid on;
legend({m16_prop,m18_prop,m20_prop},'Location','southeast');
title("Input pover vs Efficency");
xlabel("PWM duty");
ylabel("Efficency [g/W]");

figure(7);

subplot(2,2,1);
plot(m17_In,m17_Thrust);
hold on;
plot(m19_In,m19_Thrust);
hold on;
plot(m21_In,m21_Thrust);
grid on;
legend({m17_prop,m19_prop,m21_prop},'Location','southeast');
title("Input pover vs Thrust");
xlabel("PWM duty");
ylabel("Thrust [g]");

subplot(2,2,2);
plot(m17_In,m17_rpm);
hold on;
plot(m19_In,m19_rpm);
hold on;
plot(m21_In,m21_rpm);
grid on;
legend({m17_prop,m19_prop,m21_prop},'Location','southeast');
title("Input pover vs RPM");
xlabel("PWM duty");
ylabel("Rotations [RPM]");

subplot(2,2,3);
plot(m17_In,m17_mot_temp);
hold on;
plot(m19_In,m19_mot_temp);
hold on;
plot(m21_In,m21_mot_temp);
grid on;
legend({m17_prop,m19_prop,m21_prop},'Location','southeast');
title("Input pover vs motor temperature");
xlabel("PWM duty");
ylabel("Motor temperature [°C]");

subplot(2,2,4);
plot(m17_In,m17_eff);
hold on;
plot(m19_In,m19_eff);
hold on;
plot(m21_In,m21_eff);
grid on;
legend({m17_prop,m19_prop,m21_prop},'Location','southeast');
title("Input pover vs Efficency");
xlabel("PWM duty");
ylabel("Efficency [g/W]");

figure(8);
hold on;
plot(m1_Thrust,m1_eff);
plot(m4_Thrust,m4_eff);
plot(m5_Thrust,m5_eff);
plot(m10_Thrust,m10_eff);
plot(m11_Thrust,m11_eff);
plot(m16_Thrust,m16_eff);
plot(m17_Thrust,m17_eff);

legend(["bemf","bemf+30","hall","hall+30","svm","svm+30","comercial"])
ylabel(["XD"])
ylabel(["Efficency g/W"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])
