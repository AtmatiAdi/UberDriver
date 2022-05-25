%close all;
figure(3);
clear all;

size = 12*32/8 %% IT SHOULD BE MILTIPLICATION OF 6
%size = 360
power = 255;

step = 360 / size;

ang = 0:step:360-step;

sin_a = power * sin(deg2rad(ang));
sin_b = power * sin(deg2rad(ang-120));
sin_c = power * sin(deg2rad(ang-240));


for i = 1:1:size
    svm_max(i) = max([sin_a(i) sin_b(i) sin_c(i)]); 
end
svm_max = circshift(svm_max,+size/6);

svmA_GND = (sin_a + svm_max)/sqrt(3);
svmA_GND = circshift(svmA_GND,-size/12);
svmB_GND = circshift(svmA_GND,size/3);
svmC_GND = circshift(svmB_GND,size/3);


subplot(5,1,1);
hold on;
plot(svmA_GND);
plot(svmB_GND);
plot(svmC_GND);
legend(["A-GND","B-GND","C-GND"]);


% PHASE VOLTAGES
svmAB = (svmA_GND-svmB_GND);
svmBC = (svmB_GND-svmC_GND);
svmCA = (svmC_GND-svmA_GND);

subplot(5,1,2);
hold on;
plot(svmAB);
plot(svmBC);
plot(svmCA);
legend(["A","B","C"]);

% ALFA BETA
svm_alfa = svmAB;
svm_beta = (svmBC - svmCA)/sqrt(3);

subplot(5,1,3);
hold on;
plot(svm_alfa);
plot(svm_beta);
legend(["Alfa","Beta"]);

% D-Q
% ZROBIC TO DLA WIEKSZEJ ROZDZIELCZOSCI
for i = 1:1:size
    tet = deg2rad(ang(i)+180+30);
    svm_D(i) = cos(tet)*svm_alfa(i) + sin(tet)*svm_beta(i);
    svm_Q(i) = -sin(tet)*svm_alfa(i) + cos(tet)*svm_beta(i);
end
subplot(5,1,4);
hold on;
plot(svm_D);
plot(svm_Q);
legend(["D","Q"]);

svmA_GND = round(svmA_GND,0);
out = svmA_GND;

out = circshift(out,-size/48);
subplot(5,1,5);
plot(out);