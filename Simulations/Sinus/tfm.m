%close all;
figure(1)
clear all;

ang = 1:1:360;
                                                % SINOSUIDAL CONTROL
% PHASE TO GROUND VOLTAGES
sinA_GND = (sin(deg2rad(ang))+1)/2;
sinB_GND = (sin(deg2rad(ang+120))+1)/2;
sinC_GND = (sin(deg2rad(ang+240))+1)/2;

% PHASE VOLTAGES
sinA = (sinB_GND-sinC_GND)/sqrt(3);
sinB = (sinC_GND-sinA_GND)/sqrt(3);
sinC = (sinA_GND-sinB_GND)/sqrt(3);

% ALFA BETA
sin_alfa = sinA;
sin_beta = (sinB - sinC)/sqrt(3);

% D-Q
for i = 1:1:360
    tet = deg2rad(-ang(i));
    sin_D(i) = cos(tet)*sin_alfa(i) + sin(tet)*sin_beta(i);
    sin_Q(i) = -sin(tet)*sin_alfa(i) + cos(tet)*sin_beta(i);
end
% subplot(4,5,4);
% hold on;
% plot(sin_D);
% plot(sin_Q);
% legend(["D","Q"]);
% ylim([-1,1]);

                                                    % DUPKOSINUS CONTROL
sin_a = sin(deg2rad(ang));
sin_b = sin(deg2rad(ang+120));
sin_c = sin(deg2rad(ang+240));
for i = 1:1:360
    a = (abs(sin_a(i)));
    b = (abs(sin_b(i)));
    c = (abs(sin_c(i)));
    if i <= 120
        svmA_GND(i) = a;
        svmB_GND(i) = c;
        svmC_GND(i) = 0;
    elseif i <= 240
        svmA_GND(i) = b;
        svmB_GND(i) = 0;
        svmC_GND(i) = c;
    elseif i <= 360
        svmA_GND(i) = 0; 
        svmB_GND(i) = b;
        svmC_GND(i) = a;
    end
end

% PHASE VOLTAGES
svmA = (svmB_GND-svmC_GND)/sqrt(3);
svmB = (svmC_GND-svmA_GND)/sqrt(3);
svmC = (svmA_GND-svmB_GND)/sqrt(3);

% ALFA BETA
svm_alfa = svmA;
svm_beta = (svmB - svmC)/sqrt(3);

% D-Q

for i = 1:1:360
    tet = deg2rad(-ang(i)+30);
    svm_D(i) = cos(tet)*svm_alfa(i) + sin(tet)*svm_beta(i);
    svm_Q(i) = -sin(tet)*svm_alfa(i) + cos(tet)*svm_beta(i);
end
                                                    % SIX STEP CONTROL
% PHASE TO GROUND VOLTAGES
sixU_GND = zeros(1,360);
sixU_GND(1:120) = 1;
sixV_GND = zeros(1,360);
sixV_GND(121:240) = 1;
sixW_GND = zeros(1,360);
sixW_GND(241:360) = 1;

subplot(4,5,1);
hold on;
plot(sixU_GND);
plot(sixV_GND+1.5);
plot(sixW_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','southeast');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

% PHASE VOLTAGES
sixU = zeros(1,360);
sixU(1:60) = 1;
sixU(61:120) = 1/2;
sixU(121:180) = -1/2;
sixU(181:240) = -1;
sixU(241:300) = -1/2;
sixU(301:360) = 1/2;
%sixU = circshift(sixU,30);
sixV = circshift(sixU,120);
sixW = circshift(sixV,120);

subplot(4,5,2);
hold on;
plot(sixU);
plot(sixV+2.5);
plot(sixW+5);
grid on;
legend(["U","V","W"]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");

% ALFA BETA
six_alfa = sixU;
six_beta = (sixV - sixW)/sqrt(3);

subplot(4,5,3);
hold on;
plot(six_alfa);
plot(six_beta);
grid on;
legend(["Alfa","Beta"]);
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

subplot(4,5,4);
tet = atan2(six_beta,six_alfa);
rho = sqrt(six_alfa.^2+six_beta.^2);

polarplot(tet,rho,'--')
hold on;
polarplot(tet,rho,'.')
hold off;
title("Alfa-Beta current vectors");

% D-Q
for i = 1:1:360
    tet = deg2rad(ang(i)+180+30+30);
    six_D(i) = cos(tet)*six_alfa(i) + sin(tet)*six_beta(i);
    six_Q(i) = -sin(tet)*six_alfa(i) + cos(tet)*six_beta(i);
end
subplot(4,5,5);
hold on;
plot(six_D);
plot(six_Q);
grid on;
legend(["D","Q"]);
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("D-Q currents");

                                                        % BEMF_SINUS CONTROL
% PHASE TO GROUND VOLTAGES
newA_GND = zeros(1,360);
newA_GND(1:120) = sinA_GND(31:150);
newB_GND = circshift(newA_GND,120);
newC_GND = circshift(newB_GND,120);

subplot(4,5,6);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','southeast');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

% PHASE VOLTAGES
newU = zeros(1,360);
newU(1:60) = newA_GND(1:60)-newB_GND(1:60);
newU(61:120) = (newA_GND(61:120)-newC_GND(61:120))/2;
newU(121:180) = -(newB_GND(121:180)-newC_GND(121:180))/2;
newU(181:240) = -(newB_GND(181:240)-newA_GND(181:240));
newU(241:300) = -(newC_GND(241:300)-newA_GND(241:300))/2;
newU(301:360) = (newC_GND(301:360)-newB_GND(301:360))/2;
newV = circshift(newU,120);
newW = circshift(newV,120);

subplot(4,5,7);
hold on;
plot(newU);
plot(newV+2);
plot(newW+4);
grid on;
legend(["U","V","W"]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");


% ALFA BETA
new_alfa = newU;
new_beta = (newV - newW)/sqrt(3);

subplot(4,5,8);
hold on;
plot(new_alfa);
plot(new_beta);
grid on;
legend(["Alfa","Beta"]);
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30+30);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end
subplot(4,5,10);
hold on;
plot(new_D);
plot(new_Q);
grid on;
legend(["D","Q"]);
ylim([-1,1]);

                                                        % BEMF_DUPKOSINUS CONTROL

newA_GND = zeros(1,360);
newA_GND(1:120) = svmA_GND(61:180);
newB_GND = zeros(1,360);
newB_GND(121:240) = svmA_GND(61:180);
newC_GND = zeros(1,360);
newC_GND(241:360) = svmA_GND(61:180);

% PHASE TO GROUND VOLTAGES

subplot(4,5,11);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
newU = zeros(1,360);
newU(1:60) = newA_GND(1:60)-newB_GND(1:60);
newU(61:120) = (newA_GND(61:120)-newC_GND(61:120))/2;
newU(121:180) = -(newB_GND(121:180)-newC_GND(121:180))/2;
newU(181:240) = -(newB_GND(181:240)-newA_GND(181:240));
newU(241:300) = -(newC_GND(241:300)-newA_GND(241:300))/2;
newU(301:360) = (newC_GND(301:360)-newB_GND(301:360))/2;
newV = circshift(newU,120);
newW = circshift(newV,120);

subplot(4,5,12);
hold on;
plot(newU);
plot(newV+2);
plot(newW+4);
grid on;
legend(["U","V","W"]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");


% ALFA BETA
new_alfa = newU;
new_beta = (newV - newW)/sqrt(3);

subplot(4,5,13);
hold on;
plot(new_alfa);
plot(new_beta);
grid on;
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30+30);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end
subplot(4,5,15);
hold on;
plot(new_D);
plot(new_Q);
grid on;
legend(["D","Q"]);
ylim([-1,1]);

                                                        % BEMF_SINUS CONTROL

newA_GND = zeros(1,360);
newA_GND(1:120) = sinA_GND(31:150);
newB_GND = zeros(1,360);
newB_GND(121:240) = sinA_GND(31:150);
newC_GND = zeros(1,360);
newC_GND(241:360) = sinA_GND(31:150);



SIN = (sin(deg2rad(1:1:1000))+1)/2;

a = 61 + 60+30;
b = a + 120;
c = b + 120;
newA_GND(31:60) = SIN(a:a+29);
newB_GND(31:60) = SIN(c:c+29);
newC_GND(31:60) = SIN(b:b+29);

a = a + 60;
b = a + 120;
c = b + 120;

newA_GND(91:120) = SIN(a:a+29);
newB_GND(91:120) = SIN(c:c+29);
newC_GND(91:120) = SIN(b:b+29);

a = a + 60;
b = a + 120;
c = b + 120;

newA_GND(151:180) = SIN(a:a+29);
newB_GND(151:180) = SIN(c:c+29);
newC_GND(151:180) = SIN(b:b+29);

a = a + 60;
b = a + 120;
c = b + 120;

newA_GND(211:240) = SIN(a:a+29);
newB_GND(211:240) = SIN(c:c+29);
newC_GND(211:240) = SIN(b:b+29);

a = a + 60;
b = a + 120;
c = b + 120;

newA_GND(271:300) = SIN(a:a+29);
newB_GND(271:300) = SIN(c:c+29);
newC_GND(271:300) = SIN(b:b+29);

a = a + 60;
b = a + 120;
c = b + 120;

newA_GND(331:360) = SIN(a:a+29);
newB_GND(331:360) = SIN(c:c+29);
newC_GND(331:360) = SIN(b:b+29);
% PHASE TO GROUND VOLTAGES

subplot(4,5,16);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
newA = (newB_GND-newC_GND)/sqrt(3);
newB = (newC_GND-newA_GND)/sqrt(3);
newC = (newA_GND-newB_GND)/sqrt(3);

newA(1:30) = newA_GND(1:30)/2;
newA(61:90) = newA_GND(61:90)/2;
newA(121:150) = 0;
newA(181:210) = -newB_GND(181:210)/2;
newA(241:270) = -newC_GND(241:270)/2;
newA(301:330) = 0;

newB(121:150) = newB_GND(121:150)/2;
newB(181:210) = newB_GND(181:210)/2;
newB(61:90) = 0;
newB(1:30) = -newA_GND(1:30)/2;
newB(301:330) = -newC_GND(301:330)/2;
newB(241:270) = 0;

newC(241:270) = newC_GND(241:270)/2;
newC(301:330) = newC_GND(301:330)/2;
newC(1:30) = 0;
newC(61:90) = -newA_GND(61:90)/2;
newC(121:150) = -newB_GND(121:150)/2;
newC(181:210) = 0;


subplot(4,5,17);
hold on;
plot(newA);
plot(newB+1.5);
plot(newC+3);
%legend(["A","B","C"]);


% ALFA BETA
new_alfa = newA;
new_beta = (newB - newC)/sqrt(3);

subplot(4,5,18);
hold on;
plot(new_alfa);
plot(new_beta);
grid on;
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)-60-90);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end
subplot(4,5,20);
hold on;
plot(new_D);
plot(new_Q);
grid on;
legend(["D","Q"]);
ylim([-1,1]);


