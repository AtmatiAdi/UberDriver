close all;

figure(8);
hold on;
plot(m1_Thrust,m1_eff,'Color','#0072BD');
plot(m4_Thrust,m4_eff,'Color','#D95319');
plot(m10_Thrust,m10_eff,'Color','#EDB120');
plot(m16_Thrust,m16_eff,'Color','#7E2F8E');
legend(["comercial","bemf","hall","svm"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])

legend(["comercial","bemf","hall","svm"])
ylabel(["Efficiency [g/W]"])
xlabel(["Thrust [g]"])
%left-upper corner
set(gcf, 'Position', [1.8000 885.8000 1444 760.8000]);
saveas(gcf,'meas_1','epsc');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(9)
hold on
plot(m1_In,m1_rpm);
plot(m4_In,m4_rpm);
plot(m10_In,m10_rpm);
plot(m16_In,m16_rpm);

legend(["comercial","bemf","hall","svm"])
ylabel(["Rotations [RPM]"]);
xlabel(["PWM Duty [%]"]);
%left-bottom corner
set(gcf, 'Position', [1.8000 41.8000 1444 760.8000]);
saveas(gcf,'meas_2','epsc')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(10);
hold on
plot(m1_In,m1_p);
plot(m4_In,m4_p);
plot(m10_In,m10_p);
plot(m16_In,m16_p);

legend(["comercial","bemf","hall","svm"])
ylabel(["Input power [W]"]);
xlabel(["PWM Duty [%]"]);
%right-upper corner
set(gcf, 'Position', [1.4474e+03 885.8000 1.4448e+03 760.8000]);
saveas(gcf,'meas_3','epsc')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(11)
hold on
plot(m1_rpm,m1_rpm_dev)
plot(m4_rpm,m4_rpm_dev)
plot(m10_rpm,m10_rpm_dev)
plot(m16_rpm,m16_rpm_dev)

legend(["comercial","bemf","hall","svm"])
ylabel(["Rotations deviation [RPM]"]);
xlabel(["Rotations [RPM]"]);
%right-bottom corner
set(gcf, 'Position', [1.4474e+03 41.8000 1.4448e+03 760.8000]);
saveas(gcf,'meas_4','epsc')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(12);
hold on;
plot(m1_Thrust,m1_mot_temp);
plot(m4_Thrust,m4_mot_temp);
plot(m10_Thrust,m10_mot_temp);
plot(m16_Thrust,m16_mot_temp);

legend(["comercial","bemf","hall","svm"])
ylabel(["Motor temperature [°C]"])
xlabel(["Thrust [g]"])
%left-upper corner
set(gcf, 'Position', [1.8000 885.8000 1444 760.8000]);
saveas(gcf,'meas_5','epsc');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(13);
hold on;
plot(m1_Thrust,m1_inv_temp);
plot(m4_Thrust,m4_inv_temp);
plot(m10_Thrust,m10_inv_temp);
plot(m16_Thrust,m16_inv_temp);

legend(["comercial","bemf","hall","svm"])
ylabel(["Inverter temperature [°C]"])
xlabel(["Thrust [g]"])
%left-bottom corner
set(gcf, 'Position', [1.8000 41.8000 1444 760.8000]);
saveas(gcf,'meas_6','epsc')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
