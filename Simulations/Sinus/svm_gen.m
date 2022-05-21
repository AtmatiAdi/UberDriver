close all;
clear all;

size = 6*64/32 %% IT SHOULD BE MILTIPLICATION OF 6
power = 255;

step = 360 / size;

ang = 0:step:360-step;

sin_a = power * sin(deg2rad(ang));
sin_b = power * sin(deg2rad(ang+120));
sin_c = power * sin(deg2rad(ang+240));

for i = 1:1:size
    a = (abs(sin_a(i)));
    b = (abs(sin_b(i)));
    c = (abs(sin_c(i)));
    if i <= size/3
        svmA_GND(i) = a;
        svmB_GND(i) = c;
        svmC_GND(i) = 0;
    elseif i <= (size*2)/3
        svmA_GND(i) = b;
        svmB_GND(i) = 0;
        svmC_GND(i) = c;
    elseif i <= size
        svmA_GND(i) = 0; 
        svmB_GND(i) = b;
        svmC_GND(i) = a;
    end
end
subplot(4,1,1);
hold on;
plot(svmA_GND);
plot(svmB_GND);
plot(svmC_GND);
legend(["A-GND","B-GND","C-GND"]);


% PHASE VOLTAGES
svmA = (svmB_GND-svmC_GND)/sqrt(3);
svmB = (svmC_GND-svmA_GND)/sqrt(3);
svmC = (svmA_GND-svmB_GND)/sqrt(3);

subplot(4,1,2);
hold on;
plot(svmA);
plot(svmB);
plot(svmC);
legend(["A","B","C"]);

% ALFA BETA
svm_alfa = svmA;
svm_beta = (svmB - svmC)/sqrt(3);

subplot(4,1,3);
hold on;
plot(svm_alfa);
plot(svm_beta);
legend(["Alfa","Beta"]);

% D-Q
% ZROBIC TO DLA WIEKSZEJ ROZDZIELCZOSCI
for i = 1:1:size
    tet = deg2rad(-ang(i)+30-90);
    svm_D(i) = cos(tet)*svm_alfa(i) + sin(tet)*svm_beta(i);
    svm_Q(i) = -sin(tet)*svm_alfa(i) + cos(tet)*svm_beta(i);
end
subplot(4,1,4);
hold on;
plot(svm_D);
plot(svm_Q);
ylim([-150,150]);
legend(["D","Q"]);

svmA_GND = round(svmA_GND,0);
out = svmA_GND;
out(1:5*size/6) = svmA_GND(size/6+1:size);
out(5*size/6+1:size) = svmA_GND(1:size/6);

%hold on;
%plot(ang,out);