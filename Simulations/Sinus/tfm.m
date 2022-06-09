close all;
clear all;

ang = 1:1:360;

%    _____ _______   __     _____ _______ ______ _____  
%   / ____|_   _\ \ / /    / ____|__   __|  ____|  __ \ 
%  | (___   | |  \ V /____| (___    | |  | |__  | |__) |
%   \___ \  | |   > <______\___ \   | |  |  __| |  ___/ 
%   ____) |_| |_ / . \     ____) |  | |  | |____| |     
%  |_____/|_____/_/ \_\   |_____/   |_|  |______|_|     
                                                     
% PHASE TO GROUND VOLTAGES
sixU_GND = zeros(1,360);
sixU_GND(1:120) = 1;
sixV_GND = zeros(1,360);
sixV_GND(121:240) = 1;
sixW_GND = zeros(1,360);
sixW_GND(241:360) = 1;

% PHASE CURRENTS
sixU = zeros(1,360);
sixU(1:120) = 1;
sixU(121:180) = 0;
sixU(181:300) = -1;
sixU(301:360) = 0;
%sixU = circshift(sixU,30);
sixV = circshift(sixU,120);
sixW = circshift(sixV,120);

% ALFA BETA
six_alfa = sixU;
six_beta = (sixV - sixW)/sqrt(3);

alfa = atan2(six_beta,six_alfa);
rho = sqrt(six_alfa.^2+six_beta.^2);

% D-Q
for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    six_D(i) = cos(tet)*six_alfa(i) + sin(tet)*six_beta(i);
    six_Q(i) = -sin(tet)*six_alfa(i) + cos(tet)*six_beta(i);
end
six_Q_per = mean(six_Q.^2*100/1);
six_D_per = mean(six_D.^2*100/1);

figure(1);

subplot(2,2,1);
hold on;
plot(sixU_GND);
plot(sixV_GND+1.5);
plot(sixW_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(sixU);
plot(sixV+2.5);
plot(sixW+5);
grid on;
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(six_alfa);
plot(six_beta);
grid on;
legend(["Alfa","Beta"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

subplot(2,2,4);
hold on;
plot(six_D);
plot(six_Q);
grid on;
legend(["D","Q"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("D-Q currents");

%left-upper corner
set(gcf, 'Position', [1.8000 885.8000 1444 760.8000]);
saveas(gcf,'1','epsc')

figure(2);

polarplot(alfa,rho,'--')
hold on;
polarplot(alfa,rho,'o')
hold off;
title("Alfa-Beta current vectors");
rlim([0 1.2]);

saveas(gcf,'2','epsc')

%    _____ _______   __     _____ _______ ______ _____        _____ _____ _   _ 
%   / ____|_   _\ \ / /    / ____|__   __|  ____|  __ \      / ____|_   _| \ | |
%  | (___   | |  \ V /____| (___    | |  | |__  | |__) |____| (___   | | |  \| |
%   \___ \  | |   > <______\___ \   | |  |  __| |  ___/______\___ \  | | | . ` |
%   ____) |_| |_ / . \     ____) |  | |  | |____| |          ____) |_| |_| |\  |
%  |_____/|_____/_/ \_\   |_____/   |_|  |______|_|         |_____/|_____|_| \_|
                                                                                                                                                 
% PHASE TO GROUND VOLTAGES
newA_GND = zeros(1,360);
sinA_GND = (sin(deg2rad(ang))+1)/2;
newA_GND(1:120) = sinA_GND(31:150);
newB_GND = circshift(newA_GND,120);
newC_GND = circshift(newB_GND,120);

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

% ALFA BETA
new_alfa = newU;
new_beta = (newV - newW)/sqrt(3);

alfa = atan2(new_beta,new_alfa);
rho = sqrt(new_alfa.^2+new_beta.^2);
% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30+30);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end

new_Q_per = mean(new_Q.^2*100/1);
new_D_per = mean(new_D.^2*100/1);

figure(3);

subplot(2,2,1);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(newU);
plot(newV+2);
plot(newW+4);
grid on;
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(new_alfa);
plot(new_beta);
grid on;
legend(["Alfa","Beta"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

subplot(2,2,4);
hold on;
plot(new_D);
plot(new_Q);
grid on;
legend(["D","Q"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("D-Q currents");

%left-bottom corner
set(gcf, 'Position', [1.8000 41.8000 1444 760.8000]);
saveas(gcf,'3','epsc')

figure(4);

polarplot(alfa,rho,'--')
hold on;
polarplot(alfa,rho,'o')
hold off;
title("Alfa-Beta current vectors");
rlim([0 1]);

saveas(gcf,'4','epsc')

%    _____ _______   __     _____ _______ ______ _____        _____   _____ _____ _   _ 
%   / ____|_   _\ \ / /    / ____|__   __|  ____|  __ \      |  __ \ / ____|_   _| \ | |
%  | (___   | |  \ V /____| (___    | |  | |__  | |__) |_____| |  | | (___   | | |  \| |
%   \___ \  | |   > <______\___ \   | |  |  __| |  ___/______| |  | |\___ \  | | | . ` |
%   ____) |_| |_ / . \     ____) |  | |  | |____| |          | |__| |____) |_| |_| |\  |
%  |_____/|_____/_/ \_\   |_____/   |_|  |______|_|          |_____/|_____/|_____|_| \_|


% PHASE TO GROUND VOLTAGES
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

newA_GND = zeros(1,360);
newA_GND(1:120) = svmA_GND(61:180);
newB_GND = zeros(1,360);
newB_GND(121:240) = svmA_GND(61:180);
newC_GND = zeros(1,360);
newC_GND(241:360) = svmA_GND(61:180);

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

% ALFA BETA
new_alfa = newU;
new_beta = (newV - newW)/sqrt(3);

alfa = atan2(new_beta,new_alfa);
rho = sqrt(new_alfa.^2+new_beta.^2);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30+30);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end
sum = mean(new_Q.^2 + new_D.^2);
new_Q_per = mean(new_Q.^2*100/sum);
new_D_per = mean(new_D.^2*100/sum);

figure(5);

subplot(2,2,1);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(newU);
plot(newV+2);
plot(newW+4);
grid on;
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(new_alfa);
plot(new_beta);
grid on;
legend(["Alfa","Beta"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

subplot(2,2,4);
hold on;
plot(new_D);
plot(new_Q);
grid on;
legend(["D","Q"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("D-Q currents");

%right-upper corner
set(gcf, 'Position', [1.4474e+03 885.8000 1.4448e+03 760.8000]);
saveas(gcf,'5','epsc')

figure(6);

polarplot(alfa,rho,'--')
hold on;
polarplot(alfa,rho,'.')
hold off;
title("Alfa-Beta current vectors");

saveas(gcf,'6','epsc')

%    _____ _______   __     _____ _______ ______ _____        _______      ____  __ 
%   / ____|_   _\ \ / /    / ____|__   __|  ____|  __ \      / ____\ \    / /  \/  |
%  | (___   | |  \ V /____| (___    | |  | |__  | |__) |____| (___  \ \  / /| \  / |
%   \___ \  | |   > <______\___ \   | |  |  __| |  ___/______\___ \  \ \/ / | |\/| |
%   ____) |_| |_ / . \     ____) |  | |  | |____| |          ____) |  \  /  | |  | |
%  |_____/|_____/_/ \_\   |_____/   |_|  |______|_|         |_____/    \/   |_|  |_|

% PHASE TO GROUND VOLTAGES
sinA_GND = circshift(sinA_GND,-30);

newA_GND(31:60) = sinA_GND(31:60);
newA_GND(91:120) = sinA_GND(91:120);
newA_GND(151:180) = sinA_GND(151:180);
newA_GND(211:240) = sinA_GND(211:240);
newA_GND(271:300) = sinA_GND(271:300);
newA_GND(331:360) = sinA_GND(331:360);

newB_GND = circshift(newA_GND,120);
newC_GND = circshift(newB_GND,120);

% PHASE VOLTAGES
newU = (newA_GND-newB_GND);

newU(1:30) = newA_GND(1:30)-newB_GND(1:30);
newU(61:90) = (newA_GND(61:90)-newC_GND(61:90))/2;
newU(121:150) = -(newB_GND(121:150)-newC_GND(121:150))/2;
newU(181:210) = -(newB_GND(181:210)-newA_GND(181:210));
newU(241:270) = -(newC_GND(241:270)-newA_GND(241:270))/2;
newU(301:330) = (newC_GND(301:330)-newB_GND(301:330))/2;
newV = circshift(newU,120);
newW = circshift(newV,120);

% ALFA BETA
new_alfa = newU;
new_beta = (newV - newW)/sqrt(3);

alfa = atan2(new_beta,new_alfa);
rho = sqrt(new_alfa.^2+new_beta.^2);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30+30);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end

sum = mean(new_Q.^2 + new_D.^2);
new_Q_per = mean(new_Q.^2*100/sum);
new_D_per = mean(new_D.^2*100/sum);

figure(7);

subplot(2,2,1);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(newU);
plot(newV+1.5);
plot(newW+3);
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(new_alfa);
plot(new_beta);
grid on;
legend(["Alfa","Beta"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

subplot(2,2,4);
hold on;
plot(new_D);
plot(new_Q);
grid on;
legend(["D","Q"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("D-Q currents");

%right-upper corner
set(gcf, 'Position', [1.4474e+03 41.8000 1.4448e+03 760.8000]);
saveas(gcf,'7','epsc')

figure(8);

polarplot(alfa,rho,'--')
hold on;
polarplot(alfa,rho,'.')
hold off;
title("Alfa-Beta current vectors");

saveas(gcf,'8','epsc')