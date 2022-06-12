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

% Alpha BETA
six_Alpha = sixU;
six_beta = (sixV - sixW)/sqrt(3);

Alpha = atan2(six_beta,six_Alpha);
rho = sqrt(six_Alpha.^2+six_beta.^2);

% D-Q
for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    six_D(i) = cos(tet)*six_Alpha(i) + sin(tet)*six_beta(i);
    six_Q(i) = -sin(tet)*six_Alpha(i) + cos(tet)*six_beta(i);
end
fprintf("six-step D_Q percentage currents:");
six_Q_per = mean(six_Q.^2*100/(six_Q.^2 + six_D.^2))
six_D_per = mean(six_D.^2*100/(six_Q.^2 + six_D.^2))

figure(1);

subplot(2,2,1);
hold on;
plot(sixU_GND);
plot(sixV_GND+1.5);
plot(sixW_GND+3);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([0,0.5,1,1.5,2,2.5,3,3.5,4]);
yticklabels({'0','0.5','1','0','0.5','1','0','0.5','1'});
legend(["U-GND","V-GND","W-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(sixU);
plot(sixV+2.5);
plot(sixW+5);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([-1,0,1, 1.5,2.5,3.5, 4,5,6]);
yticklabels({'-1','0','1','-1','0','1','-1','0','1'});
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(six_Alpha);
plot(six_beta);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
legend(["Alpha","Beta"],'Location','east');
ylim([-1.2,1.2]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Alpha-Beta currents");

subplot(2,2,4);
hold on;
plot(six_D);
plot(six_Q);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
ylim([-1.5,1.5]);
legend(["D","Q"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("D-Q currents");

%left-upper corner
set(gcf, 'Position', [1.8000 885.8000 1444 760.8000]);
saveas(gcf,'1','epsc')

figure(2);

polarplot(Alpha,rho,'--')
hold on;
polarplot(Alpha,rho,'o')
hold off;
title("Alpha-Beta current vectors");
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

% PHASE CURRENTS
newU = zeros(1,360);
newU(1:120) = newA_GND(1:120);
newU(121:180) = 0;
newU(181:240) = -newA_GND(61:120);
newU(241:300) = -newA_GND(1:60);
newU(301:360) = 0;
newV = circshift(newU,120);
newW = circshift(newV,120);

% Alpha BETA
new_Alpha = newU;
new_beta = (newV - newW)/sqrt(3);

Alpha = atan2(new_beta,new_Alpha);
rho = sqrt(new_Alpha.^2+new_beta.^2);
% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    new_D(i) = cos(tet)*new_Alpha(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_Alpha(i) + cos(tet)*new_beta(i);
end
fprintf("six-step-sin D_Q percentage currents:");
six_Q_per = mean(new_Q.^2*100/(new_Q.^2 + new_D.^2))
six_D_per = mean(new_D.^2*100/(new_Q.^2 + new_D.^2))

figure(3);

subplot(2,2,1);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([0,0.5,1,1.5,2,2.5,3,3.5,4]);
yticklabels({'0','0.5','1','0','0.5','1','0','0.5','1'});
legend(["U-GND","V-GND","W-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(newU);
plot(newV+2.5);
plot(newW+5);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([-1,0,1, 1.5,2.5,3.5, 4,5,6]);
yticklabels({'-1','0','1','-1','0','1','-1','0','1'});
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(new_Alpha);
plot(new_beta);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
legend(["Alpha","Beta"],'Location','east');
ylim([-1.2,1.2]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Alpha-Beta currents");

subplot(2,2,4);
hold on;
plot(new_D);
plot(new_Q);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
legend(["D","Q"],'Location','east');
ylim([-1.5,1.5]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("D-Q currents");

%left-bottom corner
set(gcf, 'Position', [1.8000 41.8000 1444 760.8000]);
saveas(gcf,'3','epsc')

figure(4);

polarplot(Alpha,rho,'--')
hold on;
polarplot(Alpha,rho,'o')
hold off;
title("Alpha-Beta current vectors");
%rlim([0 1]);

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

% PHASE CURRENTS
newU = zeros(1,360);
newU(1:120) = newA_GND(1:120);
newU(121:180) = 0;
newU(181:300) = -newA_GND(1:120);
newU(301:360) = 0;
newV = circshift(newU,120);
newW = circshift(newV,120);

% Alpha BETA
new_Alpha = newU;
new_beta = (newV - newW)/sqrt(3);

Alpha = atan2(new_beta,new_Alpha);
rho = sqrt(new_Alpha.^2+new_beta.^2);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    new_D(i) = cos(tet)*new_Alpha(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_Alpha(i) + cos(tet)*new_beta(i);
end
fprintf("six-step-Dsin D_Q percentage currents:");
six_Q_per = mean(new_Q.^2*100/(new_Q.^2 + new_D.^2))
six_D_per = mean(new_D.^2*100/(new_Q.^2 + new_D.^2))

figure(5);

subplot(2,2,1);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([0,0.5,1,1.5,2,2.5,3,3.5,4]);
yticklabels({'0','0.5','1','0','0.5','1','0','0.5','1'});
legend(["U-GND","V-GND","W-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(newU);
plot(newV+2.5);
plot(newW+5);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([-1,0,1, 1.5,2.5,3.5, 4,5,6]);
yticklabels({'-1','0','1','-1','0','1','-1','0','1'});
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(new_Alpha);
plot(new_beta);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
legend(["Alpha","Beta"],'Location','east');
ylim([-1.2,1.2]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Alpha-Beta currents");

subplot(2,2,4);
hold on;
plot(new_D);
plot(new_Q);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
legend(["D","Q"],'Location','east');
ylim([-1.5,1.5]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("D-Q currents");

%right-upper corner
set(gcf, 'Position', [1.4474e+03 885.8000 1.4448e+03 760.8000]);
saveas(gcf,'5','epsc')

figure(6);

polarplot(Alpha,rho,'--')
hold on;
polarplot(Alpha,rho,'.')
hold off;
title("Alpha-Beta current vectors");

saveas(gcf,'6','epsc')

%    _____ _______   __     _____ _______ ______ _____        _______      ____  __ 
%   / ____|_   _\ \ / /    / ____|__   __|  ____|  __ \      / ____\ \    / /  \/  |
%  | (___   | |  \ V /____| (___    | |  | |__  | |__) |____| (___  \ \  / /| \  / |
%   \___ \  | |   > <______\___ \   | |  |  __| |  ___/______\___ \  \ \/ / | |\/| |
%   ____) |_| |_ / . \     ____) |  | |  | |____| |          ____) |  \  /  | |  | |
%  |_____/|_____/_/ \_\   |_____/   |_|  |______|_|         |_____/    \/   |_|  |_|

% PHASE TO GROUND VOLTAGES
sinA_GND = circshift(sinA_GND,0);
%newA_GND = 3*newA_GND/(2*sqrt(3));
newA_GND = newA_GND * 0.75;
newA_GND(31:60) = sinA_GND(31:60);
newA_GND(91:120) = sinA_GND(91:120);
newA_GND(151:180) = sinA_GND(151:180);
newA_GND(211:240) = sinA_GND(211:240);
newA_GND(271:300) = sinA_GND(271:300);
newA_GND(331:360) = sinA_GND(331:360);

newB_GND = circshift(newA_GND,120);
newC_GND = circshift(newB_GND,120);

% PHASE CURRENTS
newU = (newA_GND-newB_GND);

newU(1:30) = newA_GND(1:30);
newU(61:90) = newA_GND(61:90);
newU(121:150) = 0;
newU(181:210) = -newA_GND(1:30);
newU(241:270) = -newA_GND(61:90);
newU(301:330) = 0;
newV = circshift(newU,120);
newW = circshift(newV,120);

% Alpha BETA
new_Alpha = newU;
new_beta = (newV - newW)/sqrt(3);

Alpha = atan2(new_beta,new_Alpha);
rho = sqrt(new_Alpha.^2+new_beta.^2);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    new_D(i) = cos(tet)*new_Alpha(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_Alpha(i) + cos(tet)*new_beta(i);
end
fprintf("hybrid D_Q percentage currents:");
six_Q_per = mean(new_Q.^2*100/(new_Q.^2 + new_D.^2))
six_D_per = mean(new_D.^2*100/(new_Q.^2 + new_D.^2))

figure(7);

subplot(2,2,1);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([0,0.5,1,1.5,2,2.5,3,3.5,4]);
yticklabels({'0','0.5','1','0','0.5','1','0','0.5','1'});
legend(["U-GND","V-GND","W-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(newU);
plot(newV+2.5);
plot(newW+5);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
yticks([-1,0,1, 1.5,2.5,3.5, 4,5,6]);
yticklabels({'-1','0','1','-1','0','1','-1','0','1'});
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(new_Alpha);
plot(new_beta);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
legend(["Alpha","Beta"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("Alpha-Beta currents");

subplot(2,2,4);
hold on;
plot(new_D);
plot(new_Q);
grid on;
xlim([0,360]);
xticks([0,60,120,180,240,300,360]);
legend(["D","Q"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude [V]");
title("D-Q currents");

%right-upper corner
set(gcf, 'Position', [1.4474e+03 41.8000 1.4448e+03 760.8000]);
saveas(gcf,'7','epsc')

figure(8);

polarplot(Alpha,rho,'--')
hold on;
polarplot(Alpha,rho,'.')
hold off;
title("Alpha-Beta current vectors");

saveas(gcf,'8','epsc')