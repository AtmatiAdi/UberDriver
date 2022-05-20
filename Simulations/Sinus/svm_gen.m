close all;
clear all;

size = 6*64 %% IT SHOULD BE MILTIPLICATION OF 6
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
svmA_GND = round(svmA_GND,0);
out = svmA_GND;
out(1:5*size/6) = svmA_GND(size/6+1:size);
out(5*size/6+1:size) = svmA_GND(1:size/6);
hold on;
plot(ang,out);
%plot(svmB_GND);
%plot(svmC_GND);