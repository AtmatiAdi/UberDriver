close all;
clear all;


figure(1);
k = 8;
fsin = 1;
ftr = 4;
sim("Model2");
plot(ans.t, ans.sin);
hold on;
%plot(ans.t, ans.prostakat);
hold on;
plot(ans.t, ans.trojkat);
hold on;
plot(ans.t, ans.out);
ylim([-2 2]);
grid on;


figure(2);
k = 1;
fsin = 1;
ftr = 32;
sim("Model");
plot(ans.t, ans.sin);
hold on;
%plot(ans.t, ans.prostakat);
hold on;
plot(ans.t, ans.trojkat);
hold on;
plot(ans.t, ans.out);
ylim([-2 2]);
grid on;

figure(3);
k = 8;
fsin = 1;
ftr = 32;
sim("Model");
plot(ans.t, ans.sin);
hold on;
%plot(ans.t, ans.prostakat);
hold on;
plot(ans.t, ans.trojkat);
hold on;
plot(ans.t, ans.out);
ylim([-2 2]);
grid on;

figure(4);
k = 1;
fsin = 1;
ftr = 8;
sim("Model");
plot(ans.t, ans.sin);
hold on;
%plot(ans.t, ans.prostakat);
hold on;
plot(ans.t, ans.trojkat);
hold on;
plot(ans.t, ans.out);
ylim([-2 2]);
grid on;

figure(5);
k = 8;
fsin = 1;
ftr = 8;
sim("Model");
plot(ans.t, ans.sin);
hold on;
%plot(ans.t, ans.prostakat);
hold on;
plot(ans.t, ans.trojkat);
hold on;
plot(ans.t, ans.out);
ylim([-2 2]);
grid on;