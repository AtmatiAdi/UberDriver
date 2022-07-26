close all;
clear all;

ang = 1:1:360;

%    _____ _____ _   _ _    _  _____ 
%   / ____|_   _| \ | | |  | |/ ____|
%  | (___   | | |  \| | |  | | (___  
%   \___ \  | | | . ` | |  | |\___ \ 
%   ____) |_| |_| |\  | |__| |____) |
%  |_____/|_____|_| \_|\____/|_____/ 
                                                                  
% PHASE TO GROUND VOLTAGES
sinA_GND = (sin(deg2rad(ang))+1)/2;
sinB_GND = (sin(deg2rad(ang-120))+1)/2;
sinC_GND = (sin(deg2rad(ang-240))+1)/2;

% PHASE VOLTAGES
sinAB = (sinA_GND-sinB_GND);
sinBC = (sinB_GND-sinC_GND);
sinCA = (sinC_GND-sinA_GND);

% ALFA BETA
sin_alfa = sinAB;
sin_beta = (sinBC - sinCA)/sqrt(3);

alfa = atan2(sin_beta,sin_alfa);
rho = sqrt(sin_alfa.^2+sin_beta.^2);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30); 
    % Phase voltages are shifted to inputs by 30 deg,
    % 180 is cause by signs in A-B calculations
    sin_D(i) = cos(tet)*sin_alfa(i) + sin(tet)*sin_beta(i);
    sin_Q(i) = -sin(tet)*sin_alfa(i) + cos(tet)*sin_beta(i);
end

figure(1);

subplot(2,2,1);
hold on;
plot(sinA_GND);
plot(sinB_GND);
plot(sinC_GND);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(sinAB);
plot(sinBC);
plot(sinCA);
grid on;
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(sin_alfa);
plot(sin_beta);
grid on;
legend(["Alfa","Beta"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

subplot(2,2,4);
hold on;
plot(sin_D);
plot(sin_Q);
grid on;
legend(["D","Q"],'Location','east');
ylim([-1.5,1.5]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("D-Q currents");

%left-upper corner
set(gcf, 'Position', [1.8000 885.8000 1444 760.8000]);
saveas(gcf,'svm_1','epsc');

figure(2);

polarplot(alfa,rho,'--');
hold on;
polarplot(alfa,rho,'o');
hold off;
title("Alfa-Beta current vectors");
rlim([0 1]);

saveas(gcf,'svm_2','epsc');

%   __  __  ____  _____         _____ _____ _   _ _    _  _____ 
%  |  \/  |/ __ \|  __ \       / ____|_   _| \ | | |  | |/ ____|
%  | \  / | |  | | |  | |_____| (___   | | |  \| | |  | | (___  
%  | |\/| | |  | | |  | |______\___ \  | | | . ` | |  | |\___ \ 
%  | |  | | |__| | |__| |      ____) |_| |_| |\  | |__| |____) |
%  |_|  |_|\____/|_____/      |_____/|_____|_| \_|\____/|_____/ 
                                                                                                                    
% PHASE TO GROUND VOLTAGES
sin_a = sin(deg2rad(ang));
sin_b = sin(deg2rad(ang-120));
sin_c = sin(deg2rad(ang-240));

for i = 1:1:360
    mod(i) = min([sin_a(i) sin_b(i) sin_c(i)]); 
end
%mod = circshift(mod,+60);

svmA_GND = (sin_a - mod)/sqrt(3);
svmB_GND = (sin_b - mod)/sqrt(3);
svmC_GND = (sin_c - mod)/sqrt(3);


% PHASE VOLTAGES
svmAB = (svmA_GND-svmB_GND);
svmBC = (svmB_GND-svmC_GND);
svmCA = (svmC_GND-svmA_GND);

% ALFA BETA
svm_alfa = svmAB;
svm_beta = (svmBC - svmCA)/sqrt(3);

alfa = atan2(svm_beta,svm_alfa);
rho = sqrt(svm_alfa.^2+svm_beta.^2);

% D-Q
for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    svm_D(i) = cos(tet)*svm_alfa(i) + sin(tet)*svm_beta(i);
    svm_Q(i) = -sin(tet)*svm_alfa(i) + cos(tet)*svm_beta(i);
end

figure(3);

subplot(2,2,1);
hold on;
plot(svmA_GND);
plot(svmB_GND);
plot(svmC_GND);
plot(mod);
grid on;
legend(["A-GND","B-GND","C-GND"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Input motor terminals to ground voltages");

subplot(2,2,2);
hold on;
plot(svmAB);
plot(svmBC);
plot(svmCA);
grid on;
legend(["U","V","W"],'Location','east');
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Phases currents");

subplot(2,2,3);
hold on;
plot(svm_alfa);
plot(svm_beta);
grid on;
legend(["Alfa","Beta"],'Location','east');
ylim([-1,1]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("Alfa-Beta currents");

subplot(2,2,4);
hold on;
plot(svm_D);
plot(svm_Q);
grid on;
legend(["D","Q"],'Location','east');
ylim([-1.5,1.5]);
xlabel("Commutation angle [deg°]");
ylabel("Amplitude");
title("D-Q currents");

%left-bottom corner
set(gcf, 'Position', [1.8000 41.8000 1444 760.8000]);
saveas(gcf,'svm_3','epsc')

figure(4);

polarplot(alfa,rho,'--')
hold on;
polarplot(alfa,rho,'o')
hold off;
title("Alfa-Beta current vectors");
rlim([0 1]);

saveas(gcf,'svm_4','epsc')

%   _______ _____  _____      _____ _____ _   _ _    _  _____ 
%  |__   __|  __ \|_   _|    / ____|_   _| \ | | |  | |/ ____|
%     | |  | |__) | | |_____| (___   | | |  \| | |  | | (___  
%     | |  |  _  /  | |______\___ \  | | | . ` | |  | |\___ \ 
%     | |  | | \ \ _| |_     ____) |_| |_| |\  | |__| |____) |
%     |_|  |_|  \_\_____|   |_____/|_____|_| \_|\____/|_____/                                                     

stA_GND = (sin(deg2rad(ang)) - sawtooth(deg2rad(ang+90)*3,1/2)/4)/sqrt(3) + 0.5;
stB_GND = (sin(deg2rad(ang-120)) - sawtooth(deg2rad(ang+210)*3,1/2)/4)/sqrt(3) + 0.5;
stC_GND = (sin(deg2rad(ang-240)) - sawtooth(deg2rad(ang+330)*3,1/2)/4)/sqrt(3) + 0.5;
x = sawtooth(deg2rad(ang+90)*3,1/2)/4;

% PHASE VOLTAGES
stAB = (stA_GND-stB_GND);
stBC = (stB_GND-stC_GND);
stCA = (stC_GND-stA_GND);

% ALFA BETA
st_alfa = stAB;
st_beta = (stBC - stCA)/sqrt(3);

alfa = atan2(st_beta,st_alfa);
rho = sqrt(st_alfa.^2+st_beta.^2);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    st_D(i) = cos(tet)*st_alfa(i) + sin(tet)*st_beta(i);
    st_Q(i) = -sin(tet)*st_alfa(i) + cos(tet)*st_beta(i);
end

figure(5);

subplot(2,2,1);
hold on;
plot(stA_GND);
plot(stB_GND);
plot(stC_GND);
plot(x+0.5);
legend(["A-GND","B-GND","C-GND","Mod"]);

subplot(2,2,2);
hold on;
plot(stAB);
plot(stBC);
plot(stCA);
legend(["A-B","B-C","C-A"]);
ylim([-1,1]);

subplot(2,2,3);
hold on;
plot(st_alfa);
plot(st_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

subplot(2,2,4);
hold on;
plot(st_D);
plot(st_Q);
legend(["D","Q"]);
ylim([-1.5,1.5]);

%right-upper corner
set(gcf, 'Position', [1.4474e+03 41.8000 1.4448e+03 760.8000]);
saveas(gcf,'svm_5','epsc')

figure(6);

polarplot(alfa,rho,'--')
hold on;
polarplot(alfa,rho,'.')
hold off;
title("Alfa-Beta current vectors");

saveas(gcf,'svm_6','epsc')
