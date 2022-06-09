close all;

figure(14)

subplot(3,3,1)
hold on;
plot(m4_Thrust,m4_eff,'Color','#0072BD');
plot(m5_Thrust,m5_eff,'--','Color','#0072BD');
title([m4_prop]);
ylim([2.4 4.3]);
xlim([190 520]);
legend(["bemf","bemf+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,2)
hold on;
plot(m6_Thrust,m6_eff,'Color','#D95319');
plot(m7_Thrust,m7_eff,'--','Color','#D95319');
title([m6_prop]);
ylim([2.4 4.3]);
xlim([190 520]);
legend(["bemf","bemf+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,3)
hold on;
plot(m8_Thrust,m8_eff,'Color','#EDB120');
plot(m9_Thrust,m9_eff,'--','Color','#EDB120');
title([m8_prop]);
ylim([2.4 4.3]);
xlim([190 520]);
legend(["bemf","bemf+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,4)
hold on;
plot(m10_Thrust,m10_eff,'Color','#0072BD');
plot(m11_Thrust,m11_eff,'--','Color','#0072BD');
title([m4_prop]);
ylim([0 6.4]);
xlim([0 520]);
legend(["hall","hall+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,5)
hold on;
plot(m12_Thrust,m12_eff,'Color','#D95319');
plot(m13_Thrust,m13_eff,'--','Color','#D95319');
title([m6_prop]);
ylim([0 6.4]);
xlim([0 520]);
legend(["hall","hall+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,6)
hold on;
plot(m14_Thrust,m14_eff,'Color','#EDB120');
plot(m15_Thrust,m15_eff,'--','Color','#EDB120');
title([m8_prop]);
ylim([0 6.4]);
xlim([0 520]);
legend(["hall","hall+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,7)
hold on;
plot(m16_Thrust,m16_eff,'Color','#0072BD');
plot(m17_Thrust,m17_eff,'--','Color','#0072BD');
title([m4_prop]);
ylim([0 8]);
xlim([0 520]);
legend(["svm","svm+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,8)
hold on;
plot(m18_Thrust,m18_eff,'Color','#D95319');
plot(m19_Thrust,m19_eff,'--','Color','#D95319');
title([m6_prop]);
ylim([0 8]);
xlim([0 520]);
legend(["svm","svm+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,3,9)
hold on;
plot(m20_Thrust,m20_eff,'Color','#EDB120');
plot(m21_Thrust,m21_eff,'--','Color','#EDB120');
title([m8_prop]);
ylim([0 8]);
xlim([0 520]);
legend(["svm","svm+30"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])


%left-upper corner
set(gcf, 'Position', [1.8000 885.8000 1444 760.8000]);
saveas(gcf,'meas_1','epsc');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(15)

subplot(1,3,1)
hold on;
plot(m4_In,m4_rpm,'Color','#0072BD');
plot(m5_In,m5_rpm,'--','Color','#0072BD');
title([m4_prop]);
ylim([6400 13000]);
xlim([55 100]);
legend(["bemf","bemf+30"])
ylabel(["Rotations [RPM]"]);
xlabel(["PWM Duty [%]"]);

subplot(1,3,2)
hold on;
plot(m6_In,m6_rpm,'Color','#D95319');
plot(m7_In,m7_rpm,'--','Color','#D95319');
title([m6_prop]);
ylim([6400 13000]);
xlim([55 100])
legend(["hall","hall+30"])
ylabel(["Rotations [RPM]"]);
xlabel(["PWM Duty [%]"]);

subplot(1,3,3)
hold on;
plot(m8_In,m8_rpm,'Color','#EDB120');
plot(m9_In,m9_rpm,'--','Color','#EDB120');
title([m8_prop]);
ylim([6400 13000]);
xlim([55 100])
legend(["svm","svm+30"])
ylabel(["Rotations [RPM]"]);
xlabel(["PWM Duty [%]"]);

%left-bottom corner
set(gcf, 'Position', [1.8000 41.8000 1444 760.8000]);
saveas(gcf,'meas_2','epsc')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(16)

subplot(1,3,1)
hold on;
plot(m4_In,m4_p,'Color','#0072BD');
plot(m5_In,m5_p,'--','Color','#0072BD');
title([m4_prop]);
ylim([40 160]);
xlim([55 100]);
legend(["bemf","bemf+30"])
ylabel(["Input power [W]"]);
xlabel(["PWM Duty [%]"]);

subplot(1,3,2)
hold on;
plot(m6_In,m6_p,'Color','#D95319');
plot(m7_In,m7_p,'--','Color','#D95319');
title([m6_prop]);
ylim([40 160]);
xlim([55 100]);
legend(["hall","hall+30"])
ylabel(["Input power [W]"]);
xlabel(["PWM Duty [%]"]);

subplot(1,3,3)
hold on;
plot(m8_In,m8_p,'Color','#EDB120');
plot(m9_In,m9_p,'--','Color','#EDB120');
title([m8_prop]);
ylim([40 160]);
xlim([55 100]);
legend(["svm","svm+30"])
ylabel(["Input power [W]"]);
xlabel(["PWM Duty [%]"]);

%right-upper corner
set(gcf, 'Position', [1.4474e+03 885.8000 1.4448e+03 760.8000]);
saveas(gcf,'meas_3','epsc')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure(17)

subplot(1,3,1)
hold on;
plot(m4_rpm,m4_rpm_dev,'Color','#0072BD');
plot(m5_rpm,m5_rpm_dev,'--','Color','#0072BD');
title([m4_prop]);
ylim([40 190]);
xlim([6400 13000]);
legend(["bemf","bemf+30"])
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

subplot(1,3,2)
hold on;
plot(m6_rpm,m6_rpm_dev,'Color','#D95319');
plot(m7_rpm,m7_rpm_dev,'--','Color','#D95319');
title([m6_prop]);
ylim([40 190]);
xlim([6400 13000]);
legend(["hall","hall+30"])
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

subplot(1,3,3)
hold on;
plot(m8_rpm,m8_rpm_dev,'Color','#EDB120');
plot(m9_rpm,m9_rpm_dev,'--','Color','#EDB120');
title([m8_prop]);
ylim([40 190]);
xlim([6400 13000]);
legend(["svm","svm+30"])
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

%right-bottom corner
set(gcf, 'Position', [1.4474e+03 41.8000 1.4448e+03 760.8000]);
saveas(gcf,'meas_4','epsc')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure(18)

subplot(1,3,1)
hold on;
plot(m4_Thrust,m4_mot_temp,'Color','#0072BD');
plot(m5_Thrust,m5_mot_temp,'--','Color','#0072BD');
title([m4_prop]);
%ylim([40 190]);
%xlim([6400 13000]);
legend(["bemf","bemf+30"])
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

subplot(1,3,2)
hold on;
plot(m6_Thrust,m6_mot_temp,'Color','#D95319');
plot(m7_Thrust,m7_mot_temp,'--','Color','#D95319');
title([m6_prop]);

legend(["hall","hall+30"])
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

subplot(1,3,3)
hold on;
plot(m8_Thrust,m8_mot_temp,'Color','#EDB120');
plot(m9_Thrust,m9_mot_temp,'--','Color','#EDB120');
title([m8_prop]);

legend(["svm","svm+30"])
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

%left-upper corner
set(gcf, 'Position', [1.8000 885.8000 1444 760.8000]);
saveas(gcf,'meas_5','epsc');
