%close all;
figure(2);
clear all;

ang = 1:1:360;
                                                % SINOSUIDAL CONTROL
% PHASE TO GROUND VOLTAGES
sinA_GND = (sin(deg2rad(ang))+1)/2;
sinB_GND = (sin(deg2rad(ang-120))+1)/2;
sinC_GND = (sin(deg2rad(ang-240))+1)/2;

subplot(3,4,1);
hold on;
plot(sinA_GND);
plot(sinB_GND);
plot(sinC_GND);
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
sinAB = (sinA_GND-sinB_GND);
sinBC = (sinB_GND-sinC_GND);
sinCA = (sinC_GND-sinA_GND);

subplot(3,4,2);
hold on;
plot(sinAB);
plot(sinBC);
plot(sinCA);
legend(["A-B","B-C","C-A"]);
%ylim([-1,1]);

% ALFA BETA
sin_alfa = sinAB;
sin_beta = (sinBC - sinCA)/sqrt(3);

subplot(3,4,3);
hold on;
plot(sin_alfa);
plot(sin_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30); 
    % Phase voltages are shifted to inputs by 30 deg,
    % 180 is cause by signs in A-B calculations
    sin_D(i) = cos(tet)*sin_alfa(i) + sin(tet)*sin_beta(i);
    sin_Q(i) = -sin(tet)*sin_alfa(i) + cos(tet)*sin_beta(i);
end
subplot(3,4,4);
hold on;
plot(sin_D);
plot(sin_Q);
legend(["D","Q"]);
ylim([-1.1,1.1]);

                                                    % DUPKOSINUS CONTROL

% PHASE TO GROUND VOLTAGES
sin_a = sin(deg2rad(ang));
sin_b = sin(deg2rad(ang-120));
sin_c = sin(deg2rad(ang-240));

for i = 1:1:360
    svm_max(i) = max([sin_a(i) sin_b(i) sin_c(i)]); 
end
svm_max = circshift(svm_max,+60);
for i = 1:1:360
    svmA_GND(i) = (sin_a(i) + svm_max(i))/sqrt(3);
    svmB_GND(i) = (sin_b(i) + svm_max(i))/sqrt(3);
    svmC_GND(i) = (sin_c(i) + svm_max(i))/sqrt(3);
end

subplot(3,4,5);
hold on;
plot(svmA_GND);
plot(svmB_GND);
plot(svmC_GND);
plot(svm_max);

legend(["A-GND","B-GND","C-GND","Mod"]);
%ylim([0,1]);

% PHASE VOLTAGES
svmAB = (svmA_GND-svmB_GND);
svmBC = (svmB_GND-svmC_GND);
svmCA = (svmC_GND-svmA_GND);

subplot(3,4,6);
hold on;
plot(svmAB);
plot(svmBC);
plot(svmCA);
legend(["A-B","B-C","C-A"]);
ylim([-1,1]);

% ALFA BETA
svm_alfa = svmAB;
svm_beta = (svmBC - svmCA)/sqrt(3);

subplot(3,4,7);
hold on;
plot(svm_alfa);
plot(svm_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    svm_D(i) = cos(tet)*svm_alfa(i) + sin(tet)*svm_beta(i);
    svm_Q(i) = -sin(tet)*svm_alfa(i) + cos(tet)*svm_beta(i);
end
subplot(3,4,8);
hold on;
plot(svm_D);
plot(svm_Q);
legend(["D","Q"]);
ylim([-1.1,1.1]);

                                                    % SINUS + TRIANGLE

stA_GND = (sin(deg2rad(ang)) - sawtooth(deg2rad(ang+90)*3,1/2)/4)/sqrt(3) + 0.5;
stB_GND = (sin(deg2rad(ang-120)) - sawtooth(deg2rad(ang+210)*3,1/2)/4)/sqrt(3) + 0.5;
stC_GND = (sin(deg2rad(ang-240)) - sawtooth(deg2rad(ang+330)*3,1/2)/4)/sqrt(3) + 0.5;
x = sawtooth(deg2rad(ang+90)*3,1/2)/4;

subplot(3,4,9);
hold on;
plot(stA_GND);
plot(stB_GND);
plot(stC_GND);
plot(x+0.5);
legend(["A-GND","B-GND","C-GND","Mod"]);

% PHASE VOLTAGES
stAB = (stA_GND-stB_GND);
stBC = (stB_GND-stC_GND);
stCA = (stC_GND-stA_GND);

subplot(3,4,10);
hold on;
plot(stAB);
plot(stBC);
plot(stCA);
legend(["A-B","B-C","C-A"]);
ylim([-1,1]);

% ALFA BETA
st_alfa = stAB;
st_beta = (stBC - stCA)/sqrt(3);

subplot(3,4,11);
hold on;
plot(st_alfa);
plot(st_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)+180+30);
    st_D(i) = cos(tet)*st_alfa(i) + sin(tet)*st_beta(i);
    st_Q(i) = -sin(tet)*st_alfa(i) + cos(tet)*st_beta(i);
end
subplot(3,4,12);
hold on;
plot(st_D);
plot(st_Q);
legend(["D","Q"]);
ylim([-1.1,1.1]);