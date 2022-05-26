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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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