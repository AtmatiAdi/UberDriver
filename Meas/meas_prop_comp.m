close all;
add = 500;
figure(14)

subplot(3,2,1)
hold on;
plot(m1_Thrust,m1_eff,'Color','#7E2F8E');
plot(m4_Thrust,m4_eff,'Color','#0072BD');
plot(m5_Thrust,m5_eff,'--','Color','#0072BD');
%plot(m10_Thrust,m10_eff,'Color','#D95319');
%plot(m11_Thrust,m11_eff,'--','Color','#D95319');
%plot(m16_Thrust,m16_eff,'Color','#EDB120');
%plot(m17_Thrust,m17_eff,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 8]);
xlim([0 520]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,2,3)
hold on;
plot(m2_Thrust,m2_eff,'Color','#7E2F8E');
plot(m6_Thrust,m6_eff,'Color','#0072BD');
plot(m7_Thrust,m7_eff,'--','Color','#0072BD');
%plot(m12_Thrust,m12_eff,'Color','#D95319');
%plot(m13_Thrust,m13_eff,'--','Color','#D95319');
%plot(m18_Thrust,m18_eff,'Color','#EDB120');
%plot(m19_Thrust,m19_eff,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 8]);
xlim([0 520]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,2,5)
hold on;
plot(m3_Thrust,m3_eff,'Color','#7E2F8E');
plot(m8_Thrust,m8_eff,'Color','#0072BD');
plot(m9_Thrust,m9_eff,'--','Color','#0072BD');
%plot(m14_Thrust,m14_eff,'Color','#D95319');
%plot(m15_Thrust,m15_eff,'--','Color','#D95319');
%plot(m20_Thrust,m20_eff,'Color','#EDB120');
%plot(m21_Thrust,m21_eff,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 8]);
xlim([0 520]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,2,2)
hold on;
% plot(m1_Thrust,m1_eff,'Color','#7E2F8E');
% plot(m4_Thrust,m4_eff,'Color','#0072BD');
% plot(m5_Thrust,m5_eff,'--','Color','#0072BD');
plot(m10_Thrust,m10_eff,'Color','#D95319');
plot(m11_Thrust,m11_eff,'--','Color','#D95319');
plot(m16_Thrust,m16_eff,'Color','#EDB120');
plot(m17_Thrust,m17_eff,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 8]);
xlim([0 520]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,2,4)
hold on;
% plot(m2_Thrust,m2_eff,'Color','#7E2F8E');
% plot(m6_Thrust,m6_eff,'Color','#0072BD');
% plot(m7_Thrust,m7_eff,'--','Color','#0072BD');
plot(m12_Thrust,m12_eff,'Color','#D95319');
plot(m13_Thrust,m13_eff,'--','Color','#D95319');
plot(m18_Thrust,m18_eff,'Color','#EDB120');
plot(m19_Thrust,m19_eff,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 8]);
xlim([0 520]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

subplot(3,2,6)
hold on;
% plot(m3_Thrust,m3_eff,'Color','#7E2F8E');
% plot(m8_Thrust,m8_eff,'Color','#0072BD');
% plot(m9_Thrust,m9_eff,'--','Color','#0072BD');
plot(m14_Thrust,m14_eff,'Color','#D95319');
plot(m15_Thrust,m15_eff,'--','Color','#D95319');
plot(m20_Thrust,m20_eff,'Color','#EDB120');
plot(m21_Thrust,m21_eff,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 8]);
xlim([0 520]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

%left-upper corner
set(gcf, 'Position', [1.8000 885.8000-add 1444 760.8000+add]);
saveas(gcf,'meas_1','epsc');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(15)

subplot(3,2,1)
hold on;
plot(m1_In,m1_rpm,'Color','#7E2F8E');
plot(m4_In,m4_rpm,'Color','#0072BD');
plot(m5_In,m5_rpm,'--','Color','#0072BD');
% plot(m10_In,m10_rpm,'Color','#D95319');
% plot(m11_In,m11_rpm,'--','Color','#D95319');
% plot(m16_In,m16_rpm,'Color','#EDB120');
% plot(m17_In,m17_rpm,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 13000]);
xlim([0 100]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','southeast');
ylabel(["Rotations [RPM]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,3)
hold on;

plot(m2_In,m2_rpm,'Color','#7E2F8E');
plot(m6_In,m6_rpm,'Color','#0072BD');
plot(m7_In,m7_rpm,'--','Color','#0072BD');
% plot(m12_In,m12_rpm,'Color','#D95319');
% plot(m13_In,m13_rpm,'--','Color','#D95319');
% plot(m18_In,m18_rpm,'Color','#EDB120');
% plot(m19_In,m19_rpm,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 13000]);
xlim([0 100])
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','southeast');
ylabel(["Rotations [RPM]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,5)
hold on;
plot(m3_In,m3_rpm,'Color','#7E2F8E');
plot(m8_In,m8_rpm,'Color','#0072BD');
plot(m9_In,m9_rpm,'--','Color','#0072BD');
% plot(m14_In,m14_rpm,'Color','#D95319');
% plot(m15_In,m15_rpm,'--','Color','#D95319');
% plot(m20_In,m20_rpm,'Color','#EDB120');
% plot(m21_In,m21_rpm,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 13000]);
xlim([0 100])
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','southeast');
ylabel(["Rotations [RPM]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,2)
hold on;
% plot(m1_In,m1_rpm,'Color','#7E2F8E');
% plot(m4_In,m4_rpm,'Color','#0072BD');
% plot(m5_In,m5_rpm,'--','Color','#0072BD');
plot(m10_In,m10_rpm,'Color','#D95319');
plot(m11_In,m11_rpm,'--','Color','#D95319');
plot(m16_In,m16_rpm,'Color','#EDB120');
plot(m17_In,m17_rpm,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 13000]);
xlim([0 100]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Rotations [RPM]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,4)
hold on;

% plot(m2_In,m2_rpm,'Color','#7E2F8E');
% plot(m6_In,m6_rpm,'Color','#0072BD');
% plot(m7_In,m7_rpm,'--','Color','#0072BD');
plot(m12_In,m12_rpm,'Color','#D95319');
plot(m13_In,m13_rpm,'--','Color','#D95319');
plot(m18_In,m18_rpm,'Color','#EDB120');
plot(m19_In,m19_rpm,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 13000]);
xlim([0 100])
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Rotations [RPM]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,6)
hold on;
% plot(m3_In,m3_rpm,'Color','#7E2F8E');
% plot(m8_In,m8_rpm,'Color','#0072BD');
% plot(m9_In,m9_rpm,'--','Color','#0072BD');
plot(m14_In,m14_rpm,'Color','#D95319');
plot(m15_In,m15_rpm,'--','Color','#D95319');
plot(m20_In,m20_rpm,'Color','#EDB120');
plot(m21_In,m21_rpm,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 13000]);
xlim([0 100])
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Rotations [RPM]"]);
xlabel(["Throttle [%]"]);

%left-bottom corner
set(gcf, 'Position', [1.8000 41.8000 1444 760.8000+add]);
saveas(gcf,'meas_2','epsc')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(16)

subplot(3,2,1)
hold on;
plot(m1_In,m1_p,'Color','#7E2F8E');
plot(m4_In,m4_p,'Color','#0072BD');
plot(m5_In,m5_p,'--','Color','#0072BD');
% plot(m10_In,m10_p,'Color','#D95319');
% plot(m11_In,m11_p,'--','Color','#D95319');
% plot(m16_In,m16_p,'Color','#EDB120');
% plot(m17_In,m17_p,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 190]);
xlim([0 100]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','northwest');
ylabel(["Input power [W]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,3)
hold on;
plot(m2_In,m2_p,'Color','#7E2F8E');
plot(m6_In,m6_p,'Color','#0072BD');
plot(m7_In,m7_p,'--','Color','#0072BD');
% plot(m12_In,m12_p,'Color','#D95319');
% plot(m13_In,m13_p,'--','Color','#D95319');
% plot(m18_In,m18_p,'Color','#EDB120');
% plot(m19_In,m19_p,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 190]);
xlim([0 100]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','northwest');
ylabel(["Input power [W]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,5)
hold on;
plot(m3_In,m3_p,'Color','#7E2F8E');
plot(m8_In,m8_p,'Color','#0072BD');
plot(m9_In,m9_p,'--','Color','#0072BD');
% plot(m14_In,m14_p,'Color','#D95319');
% plot(m15_In,m15_p,'--','Color','#D95319');
% plot(m20_In,m20_p,'Color','#EDB120');
% plot(m21_In,m21_p,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 190]);
xlim([0 100]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','northwest');
ylabel(["Input power [W]"]);
xlabel(["Throttle [%]"]);


subplot(3,2,2)
hold on;
% plot(m1_In,m1_p,'Color','#7E2F8E');
% plot(m4_In,m4_p,'Color','#0072BD');
% plot(m5_In,m5_p,'--','Color','#0072BD');
plot(m10_In,m10_p,'Color','#D95319');
plot(m11_In,m11_p,'--','Color','#D95319');
plot(m16_In,m16_p,'Color','#EDB120');
plot(m17_In,m17_p,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 190]);
xlim([0 100]);
legend(["6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','northwest');
ylabel(["Input power [W]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,4)
hold on;
% plot(m2_In,m2_p,'Color','#7E2F8E');
% plot(m6_In,m6_p,'Color','#0072BD');
% plot(m7_In,m7_p,'--','Color','#0072BD');
plot(m12_In,m12_p,'Color','#D95319');
plot(m13_In,m13_p,'--','Color','#D95319');
plot(m18_In,m18_p,'Color','#EDB120');
plot(m19_In,m19_p,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 190]);
xlim([0 100]);
legend(["6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','northwest');
ylabel(["Input power [W]"]);
xlabel(["Throttle [%]"]);

subplot(3,2,6)
hold on;
% plot(m3_In,m3_p,'Color','#7E2F8E');
% plot(m8_In,m8_p,'Color','#0072BD');
% plot(m9_In,m9_p,'--','Color','#0072BD');
plot(m14_In,m14_p,'Color','#D95319');
plot(m15_In,m15_p,'--','Color','#D95319');
plot(m20_In,m20_p,'Color','#EDB120');
plot(m21_In,m21_p,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 190]);
xlim([0 100]);
legend(["6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','northwest');
ylabel(["Input power [W]"]);
xlabel(["Throttle [%]"]);

%right-upper corner
set(gcf, 'Position', [1.4474e+03 885.8000-add 1.4448e+03 760.8000+add]);
saveas(gcf,'meas_3','epsc')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure(17)

subplot(3,2,1)
hold on;
plot(m1_rpm,m1_rpm_dev,'Color','#7E2F8E');
plot(m4_rpm,m4_rpm_dev,'Color','#0072BD');
plot(m5_rpm,m5_rpm_dev,'--','Color','#0072BD');
% plot(m10_rpm,m10_rpm_dev,'Color','#D95319');
% plot(m11_rpm,m11_rpm_dev,'--','Color','#D95319');
% plot(m16_rpm,m16_rpm_dev,'Color','#EDB120');
% plot(m17_rpm,m17_rpm_dev,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 220]);
xlim([0 13000]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°",],'Location','southeast');
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

subplot(3,2,3)
hold on;
plot(m2_rpm,m2_rpm_dev,'Color','#7E2F8E');
plot(m6_rpm,m6_rpm_dev,'Color','#0072BD');
plot(m7_rpm,m7_rpm_dev,'--','Color','#0072BD');
% plot(m12_rpm,m12_rpm_dev,'Color','#D95319');
% plot(m13_rpm,m13_rpm_dev,'--','Color','#D95319');
% plot(m18_rpm,m18_rpm_dev,'Color','#EDB120');
% plot(m19_rpm,m19_rpm_dev,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 220]);
xlim([0 13000]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°",],'Location','southeast');
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

subplot(3,2,5)
hold on;
plot(m3_rpm,m3_rpm_dev,'Color','#7E2F8E');
plot(m8_rpm,m8_rpm_dev,'Color','#0072BD');
plot(m9_rpm,m9_rpm_dev,'--','Color','#0072BD');
% plot(m14_rpm,m14_rpm_dev,'Color','#D95319');
% plot(m15_rpm,m15_rpm_dev,'--','Color','#D95319');
% plot(m20_rpm,m20_rpm_dev,'Color','#EDB120');
% plot(m21_rpm,m21_rpm_dev,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 220]);
xlim([0 13000]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°",],'Location','southeast');
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

subplot(3,2,2)
hold on;
% plot(m1_rpm,m1_rpm_dev,'Color','#7E2F8E');
% plot(m4_rpm,m4_rpm_dev,'Color','#0072BD');
% plot(m5_rpm,m5_rpm_dev,'--','Color','#0072BD');
plot(m10_rpm,m10_rpm_dev,'Color','#D95319');
plot(m11_rpm,m11_rpm_dev,'--','Color','#D95319');
plot(m16_rpm,m16_rpm_dev,'Color','#EDB120');
plot(m17_rpm,m17_rpm_dev,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([0 220]);
xlim([0 13000]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

subplot(3,2,4)
hold on;
% plot(m2_rpm,m2_rpm_dev,'Color','#7E2F8E');
% plot(m6_rpm,m6_rpm_dev,'Color','#0072BD');
% plot(m7_rpm,m7_rpm_dev,'--','Color','#0072BD');
plot(m12_rpm,m12_rpm_dev,'Color','#D95319');
plot(m13_rpm,m13_rpm_dev,'--','Color','#D95319');
plot(m18_rpm,m18_rpm_dev,'Color','#EDB120');
plot(m19_rpm,m19_rpm_dev,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([0 220]);
xlim([0 13000]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

subplot(3,2,6)
hold on;
% plot(m3_rpm,m3_rpm_dev,'Color','#7E2F8E');
% plot(m8_rpm,m8_rpm_dev,'Color','#0072BD');
% plot(m9_rpm,m9_rpm_dev,'--','Color','#0072BD');
plot(m14_rpm,m14_rpm_dev,'Color','#D95319');
plot(m15_rpm,m15_rpm_dev,'--','Color','#D95319');
plot(m20_rpm,m20_rpm_dev,'Color','#EDB120');
plot(m21_rpm,m21_rpm_dev,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([0 220]);
xlim([0 13000]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);

%right-bottom corner
set(gcf, 'Position', [1.4474e+03 41.8000 1.4448e+03 760.8000+add]);
saveas(gcf,'meas_4','epsc')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure(18)

subplot(3,2,1)
hold on;
plot(m1_Thrust,m1_mot_temp,'Color','#7E2F8E');
plot(m4_Thrust,m4_mot_temp,'Color','#0072BD');
plot(m5_Thrust,m5_mot_temp,'--','Color','#0072BD');
% plot(m10_Thrust,m10_mot_temp,'Color','#D95319');
% plot(m11_Thrust,m11_mot_temp,'--','Color','#D95319');
% plot(m16_Thrust,m16_mot_temp,'Color','#EDB120');
% plot(m17_Thrust,m17_mot_temp,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([20 160]);
xlim([0 530]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','southeast');
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

subplot(3,2,3)
hold on;
plot(m2_Thrust,m2_mot_temp,'Color','#7E2F8E');
plot(m6_Thrust,m6_mot_temp,'Color','#0072BD');
plot(m7_Thrust,m7_mot_temp,'--','Color','#0072BD');
% plot(m12_Thrust,m12_mot_temp,'Color','#D95319');
% plot(m13_Thrust,m13_mot_temp,'--','Color','#D95319');
% plot(m18_Thrust,m18_mot_temp,'Color','#EDB120');
% plot(m19_Thrust,m19_mot_temp,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([20 160]);
xlim([0 530]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','southeast');
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

subplot(3,2,5)
hold on;
plot(m3_Thrust,m3_mot_temp,'Color','#7E2F8E');
plot(m8_Thrust,m8_mot_temp,'Color','#0072BD');
plot(m9_Thrust,m9_mot_temp,'--','Color','#0072BD');
% plot(m14_Thrust,m14_mot_temp,'Color','#D95319');
% plot(m15_Thrust,m15_mot_temp,'--','Color','#D95319');
% plot(m20_Thrust,m20_mot_temp,'Color','#EDB120');
% plot(m21_Thrust,m21_mot_temp,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([20 160]);
xlim([0 530]);
legend(["ThunderX 30A","6-Step + BEMF","6-Step + BEMF + 30°"],'Location','southeast');
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

subplot(3,2,2)
hold on;
% plot(m1_Thrust,m1_mot_temp,'Color','#7E2F8E');
% plot(m4_Thrust,m4_mot_temp,'Color','#0072BD');
% plot(m5_Thrust,m5_mot_temp,'--','Color','#0072BD');
plot(m10_Thrust,m10_mot_temp,'Color','#D95319');
plot(m11_Thrust,m11_mot_temp,'--','Color','#D95319');
plot(m16_Thrust,m16_mot_temp,'Color','#EDB120');
plot(m17_Thrust,m17_mot_temp,'--','Color','#EDB120');
title([m4_prop]);
grid on;
ylim([20 160]);
xlim([0 530]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

subplot(3,2,4)
hold on;
% plot(m2_Thrust,m2_mot_temp,'Color','#7E2F8E');
% plot(m6_Thrust,m6_mot_temp,'Color','#0072BD');
% plot(m7_Thrust,m7_mot_temp,'--','Color','#0072BD');
plot(m12_Thrust,m12_mot_temp,'Color','#D95319');
plot(m13_Thrust,m13_mot_temp,'--','Color','#D95319');
plot(m18_Thrust,m18_mot_temp,'Color','#EDB120');
plot(m19_Thrust,m19_mot_temp,'--','Color','#EDB120');
title([m6_prop]);
grid on;
ylim([20 160]);
xlim([0 530]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

subplot(3,2,6)
hold on;
% plot(m3_Thrust,m3_mot_temp,'Color','#7E2F8E');
% plot(m8_Thrust,m8_mot_temp,'Color','#0072BD');
% plot(m9_Thrust,m9_mot_temp,'--','Color','#0072BD');
plot(m14_Thrust,m14_mot_temp,'Color','#D95319');
plot(m15_Thrust,m15_mot_temp,'--','Color','#D95319');
plot(m20_Thrust,m20_mot_temp,'Color','#EDB120');
plot(m21_Thrust,m21_mot_temp,'--','Color','#EDB120');
title([m8_prop]);
grid on;
ylim([20 160]);
xlim([0 530]);
legend(["6-Step + Hall","6-Step + Hall + 30°","SVM + Hall","SVM + Hall + 30°"],'Location','southeast');
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])

%left-upper corner
set(gcf, 'Position', [1.8000 885.8000-add 1444 760.8000+add]);
saveas(gcf,'meas_5','epsc');
