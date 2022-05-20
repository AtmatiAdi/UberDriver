close all;
clear all;

ang = 1:1:360;


                                                % SINOSUIDAL CONTROL
% PHASE TO GROUND VOLTAGES
sinA_GND = (sin(deg2rad(ang))+1)/2;
sinB_GND = (sin(deg2rad(ang+120))+1)/2;
sinC_GND = (sin(deg2rad(ang+240))+1)/2;

% subplot(4,4,1);
% hold on;
% plot(sinA_GND);
% plot(sinB_GND);
% plot(sinC_GND);
% legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
sinA = (sinB_GND-sinC_GND)/sqrt(3);
sinB = (sinC_GND-sinA_GND)/sqrt(3);
sinC = (sinA_GND-sinB_GND)/sqrt(3);

% subplot(4,4,2);
% hold on;
% plot(sinA);
% plot(sinB);
% plot(sinC);
% legend(["A","B","C"]);
% ylim([-1,1]);

% ALFA BETA
sin_alfa = sinA;
sin_beta = (sinB - sinC)/sqrt(3);

% subplot(4,4,3);
% hold on;
% plot(sin_alfa);
% plot(sin_beta);
% legend(["Alfa","Beta"]);
% ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(-ang(i));
    sin_D(i) = cos(tet)*sin_alfa(i) + sin(tet)*sin_beta(i);
    sin_Q(i) = -sin(tet)*sin_alfa(i) + cos(tet)*sin_beta(i);
end
% subplot(4,4,4);
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

% tmp = svmA;
% shift = 60;
% svmA(1:360 - shift) = tmp(1 + shift:360);
% svmA(360 - shift + 1:360) = tmp(1:shift);
% tmp = svmB;
% svmB(1:360 - shift) = tmp(1 + shift:360);
% svmB(360 - shift + 1:360) = tmp(1:shift);
% tmp = svmC;
% svmC(1:360 - shift) = tmp(1 + shift:360);
% svmC(360 - shift + 1:360) = tmp(1:shift);

% PHASE TO GROUND VOLTAGES
% subplot(4,4,6);
% hold on;
% plot(svmA_GND);
% plot(svmB_GND);
% plot(svmC_GND);
% legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
svmA = (svmB_GND-svmC_GND)/sqrt(3);
svmB = (svmC_GND-svmA_GND)/sqrt(3);
svmC = (svmA_GND-svmB_GND)/sqrt(3);

% subplot(4,4,7);
% hold on;
% plot(svmA);
% plot(svmB);
% plot(svmC);
% legend(["A","B","C"]);
% ylim([-1,1]);

% ALFA BETA
svm_alfa = svmA;
svm_beta = (svmB - svmC)/sqrt(3);

% subplot(4,4,8);
% hold on;
% plot(svm_alfa);
% plot(svm_beta);
% legend(["Alfa","Beta"]);
% ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(-ang(i)+30);
    svm_D(i) = cos(tet)*svm_alfa(i) + sin(tet)*svm_beta(i);
    svm_Q(i) = -sin(tet)*svm_alfa(i) + cos(tet)*svm_beta(i);
end
% subplot(4,4,9);
% hold on;
% plot(svm_D);
% plot(svm_Q);
% legend(["D","Q"]);
% ylim([-1,1]);

                                                    % SIX STEP CONTROL
sixA_GND = zeros(1,360);
sixA_GND(1:120) = 1;
sixB_GND = zeros(1,360);
sixB_GND(121:240) = 1;
sixC_GND = zeros(1,360);
sixC_GND(241:360) = 1;

% PHASE TO GROUND VOLTAGES
subplot(4,4,1);
hold on;
plot(sixA_GND);
plot(sixB_GND+1.5);
plot(sixC_GND+3);
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
sixA = zeros(1,360);
sixA(1:120) = 0.5;
sixA(181:300) = -0.5;
sixB = zeros(1,360);
sixB(121:240) = 0.5;
sixB(1:60) = -0.5;
sixB(301:360) = -0.5;
sixC = zeros(1,360);
sixC(241:360) = 0.5;
sixC(61:180) = -0.5;

subplot(4,4,2);
hold on;
plot(sixA);
plot(sixB+1.5);
plot(sixC+3);
legend(["A","B","C"]);

% ALFA BETA
six_alfa = sixA;
six_beta = (sixB - sixC)/sqrt(3);

subplot(4,4,3);
hold on;
plot(six_alfa);
plot(six_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);


% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)-60-90);
    six_D(i) = cos(tet)*six_alfa(i) + sin(tet)*six_beta(i);
    six_Q(i) = -sin(tet)*six_alfa(i) + cos(tet)*six_beta(i);
end
subplot(4,4,4);
hold on;
plot(six_D);
plot(six_Q);
legend(["D","Q"]);
ylim([-1,1]);


                                                        % BEMF_SINUS CONTROL

newA_GND = zeros(1,360);
newA_GND(1:120) = sinA_GND(31:150);
newB_GND = zeros(1,360);
newB_GND(121:240) = sinA_GND(31:150);
newC_GND = zeros(1,360);
newC_GND(241:360) = sinA_GND(31:150);

% PHASE TO GROUND VOLTAGES

subplot(4,4,5);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
newA = zeros(1,360);
newA(1:120) = newA_GND(1:120)/2;
newA(181:300) = -newA_GND(1:120)/2;
newB = zeros(1,360);
newB(121:240) = newB_GND(121:240)/2;
newB(1:60) = -newB_GND(121:180)/2;
newB(301:360) = -newB_GND(181:240)/2;
newC = zeros(1,360);
newC(241:360) = newC_GND(241:360)/2;
newC(61:180) = -newC_GND(241:360)/2;

subplot(4,4,6);
hold on;
plot(newA);
plot(newB+1.5);
plot(newC+3);
legend(["A","B","C"]);


% ALFA BETA
new_alfa = newA;
new_beta = (newB - newC)/sqrt(3);

subplot(4,4,7);
hold on;
plot(new_alfa);
plot(new_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)-60-90);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end
subplot(4,4,8);
hold on;
plot(new_D);
plot(new_Q);
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

subplot(4,4,9);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
legend(["A-GND","B-GND","C-GND"]);

% PHASE VOLTAGES
newA = zeros(1,360);
newA(1:120) = newA_GND(1:120)/2;
newA(181:300) = -newA_GND(1:120)/2;
newB = zeros(1,360);
newB(121:240) = newB_GND(121:240)/2;
newB(1:60) = -newB_GND(121:180)/2;
newB(301:360) = -newB_GND(181:240)/2;
newC = zeros(1,360);
newC(241:360) = newC_GND(241:360)/2;
newC(61:180) = -newC_GND(241:360)/2;

subplot(4,4,10);
hold on;
plot(newA);
plot(newB+1.5);
plot(newC+3);
legend(["A","B","C"]);


% ALFA BETA
new_alfa = newA;
new_beta = (newB - newC)/sqrt(3);

subplot(4,4,11);
hold on;
plot(new_alfa);
plot(new_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)-60-90);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end
subplot(4,4,12);
hold on;
plot(new_D);
plot(new_Q);
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

subplot(4,4,13);
hold on;
plot(newA_GND);
plot(newB_GND+1.5);
plot(newC_GND+3);
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


subplot(4,4,14);
hold on;
plot(newA);
plot(newB+1.5);
plot(newC+3);
%legend(["A","B","C"]);


% ALFA BETA
new_alfa = newA;
new_beta = (newB - newC)/sqrt(3);

subplot(4,4,15);
hold on;
plot(new_alfa);
plot(new_beta);
legend(["Alfa","Beta"]);
ylim([-1,1]);

% D-Q

for i = 1:1:360
    tet = deg2rad(ang(i)-60-90);
    new_D(i) = cos(tet)*new_alfa(i) + sin(tet)*new_beta(i);
    new_Q(i) = -sin(tet)*new_alfa(i) + cos(tet)*new_beta(i);
end
subplot(4,4,16);
hold on;
plot(new_D);
plot(new_Q);
legend(["D","Q"]);
ylim([-1,1]);



%return;
% 
% for i = 1:1:360
%    tmp = i;
%    if(tmp >360)
%        tmp = tmp - 360;
%    end
%    C(i) = svmA(tmp);
%    tmp = tmp + 120;
%    if(tmp >360)
%        tmp = tmp - 360;
%    end
%    A(i) = svmA(tmp);
%    tmp = tmp + 120;
%    if(tmp >360)
%        tmp = tmp - 360;
%    end
%    B(i) = svmA(tmp);
%    
%    alfa(i) = sinA(i);
%    beta(i) = (sinB(i) - sinC(i))/sqrt(3);
% end




% 
% figure(2);
% for i = 1:1:360
%     plot(six_alfa(1:i),six_beta(1:i));
%     hold on;
%     quiver(0,0,six_alfa(i),six_beta(i),1);
%     hold off;
%     set(gca,'XLim',[-1,1],'YLim',[-1,1]);
%     drawnow;
% end
% hold on;