close all;
clear all;

ang = 1:1:360;
                                                % SINOSUIDAL CONTROL
% PHASE TO GROUND VOLTAGES
sinA_GND = (sin(deg2rad(ang))+1)/2;
sinB_GND = (sin(deg2rad(ang+120))+1)/2;
sinC_GND = (sin(deg2rad(ang+240))+1)/2;

subplot(5,5,1);
hold on;
plot(sinA_GND);
plot(sinB_GND);
plot(sinC_GND);
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
sinA = (sinB_GND-sinC_GND)/sqrt(3);
sinB = (sinC_GND-sinA_GND)/sqrt(3);
sinC = (sinA_GND-sinB_GND)/sqrt(3);

subplot(5,5,2);
hold on;
plot(sinA);
plot(sinB);
plot(sinC);
legend(["A","B","C"]);
ylim([-1,1]);

% ALFA BETA
sin_alfa = sinA;
sin_beta = (sinB - sinC)/sqrt(3);

subplot(5,5,3);
hold on;
plot(sin_alfa);
plot(sin_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(-ang(i));
    sin_D(i) = cos(tet)*sin_alfa(i) + sin(tet)*sin_beta(i);
    sin_Q(i) = -sin(tet)*sin_alfa(i) + cos(tet)*sin_beta(i);
end
subplot(5,5,4);
hold on;
plot(sin_D);
plot(sin_Q);
legend(["D","Q"]);
ylim([-1,1]);

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

% PHASE TO GROUND VOLTAGES
subplot(5,5,6);
hold on;
plot(svmA_GND);
plot(svmB_GND);
plot(svmC_GND);
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
svmA = (svmB_GND-svmC_GND)/sqrt(3);
svmB = (svmC_GND-svmA_GND)/sqrt(3);
svmC = (svmA_GND-svmB_GND)/sqrt(3);

subplot(5,5,7);
hold on;
plot(svmA);
plot(svmB);
plot(svmC);
legend(["A","B","C"]);
ylim([-1,1]);

% ALFA BETA
svm_alfa = svmA;
svm_beta = (svmB - svmC)/sqrt(3);

subplot(5,5,8);
hold on;
plot(svm_alfa);
plot(svm_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(-ang(i)+30);
    svm_D(i) = cos(tet)*svm_alfa(i) + sin(tet)*svm_beta(i);
    svm_Q(i) = -sin(tet)*svm_alfa(i) + cos(tet)*svm_beta(i);
end
subplot(5,5,9);
hold on;
plot(svm_D);
plot(svm_Q);
legend(["D","Q"]);
ylim([-1,1]);

                                                    % SINUS + TRIANGLE

stA_GND = (sin(deg2rad(ang)) - sawtooth(deg2rad(ang+90)*3,1/2)/4)/sqrt(3) + 0.5;
stB_GND = (sin(deg2rad(ang+120)) - sawtooth(deg2rad(ang+210)*3,1/2)/4)/sqrt(3) + 0.5;
stC_GND = (sin(deg2rad(ang+240)) - sawtooth(deg2rad(ang+330)*3,1/2)/4)/sqrt(3) + 0.5;
x = sawtooth(deg2rad(ang+90)*3,1/2)/4;

subplot(5,5,11);
hold on;
plot(stA_GND);
plot(stB_GND);
plot(stC_GND);
plot(x+0.5);
legend(["A-GND","B-GND","C-GND","Mod"]);

% PHASE VOLTAGES
stA = (stB_GND-stC_GND)/sqrt(3);
stB = (stC_GND-stA_GND)/sqrt(3);
stC = (stA_GND-stB_GND)/sqrt(3);

subplot(5,5,12);
hold on;
plot(stA);
plot(stB);
plot(stC);
legend(["A","B","C"]);
ylim([-1,1]);

% ALFA BETA
st_alfa = stA;
st_beta = (stB - stC)/sqrt(3);

subplot(5,5,13);
hold on;
plot(st_alfa);
plot(st_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(-ang(i)+0);
    st_D(i) = cos(tet)*st_alfa(i) + sin(tet)*st_beta(i);
    st_Q(i) = -sin(tet)*st_alfa(i) + cos(tet)*st_beta(i);
end
subplot(5,5,14);
hold on;
plot(st_D);
plot(st_Q);
legend(["D","Q"]);
ylim([-1,1]);