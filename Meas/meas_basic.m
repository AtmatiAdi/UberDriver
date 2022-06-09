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

