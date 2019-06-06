clc
clear all
% close all

%% Curva teorica
dt_arduino = dlmread('tiempos_arduino.txt');
t_teorico = cumsum(dt_arduino);
t_teorico = [t_teorico; -flipud(t_teorico)+2*t_teorico(end)];

%% Datos empiricos
MOTOR_FILE = 'motor_output.txt';
SENSOR_FILE= 'sensor_output.txt';

t_arduino = dlmread(MOTOR_FILE,'\n')';
t_arduino = t_arduino(1:end-1);
t_sensor = dlmread(SENSOR_FILE,'\n')';
t_sensor = t_sensor(1:end-1);

mm_per_10step = 1/200*20;

%% Grafico t_arduino vs. t_sensor vs. teorico
figure
hold on
% Teorico
x1 = linspace(10,-10,length(t_teorico));
plot(t_teorico,x1,'.g','DisplayName', 'Teorico');
% Arduino
lim = 10 - length(t_arduino)*0.05;
x1 = 10:-0.05:lim+0.05;
plot(t_arduino,x1,'.r','DisplayName', 'Arduino');
% Sensor
%x2 = linspace(10,0,length(t_sensor));
lim = 10 - length(t_sensor)*0.025;
x2 = 10:-0.025:lim+0.025;
plot(t_sensor,x2,'.b','DisplayName', 'Sensor');


title('Datos empiricos vs. teorico')
xlabel('Tiempo [us]')
ylabel('Posicion [mm]')
legend('Location','NorthEast')
grid on
grid minor


%% Primer filtrado
n = 2;
tolerance = 10000;
t_sensor_f1 = t_sensor; 

while (n<length(t_sensor_f1))
   if (abs(t_sensor_f1(n)-t_sensor_f1(n-1)) < tolerance)
       t_sensor_f1(n) = [];
   else
       
       n = n+1;
   end
end

%% Grafico empirico filtrado vs. teorico
figure
hold on
% Teorico
x1 = linspace(10,0,length(t_teorico));
plot(t_teorico,x1,'.g','DisplayName', 'Teorico');
% Arduino
lim = 10 - length(t_arduino)*0.05;
x1 = 10:-0.05:lim+0.05;
plot(t_arduino,x1,'.r','DisplayName', 'Arduino');
% Sensor
%x1 = linspace(10,0,length(t_sensor));
%plot(t_sensor,x1,'.k','DisplayName', 'Sin filtro')
% Sensor filtrado
lim = 10 - length(t_sensor_f1)*0.025;
x2 = 10:-0.025:lim+0.025;
plot(t_sensor_f1,x2,'.b','DisplayName', 'Con filtro 1');
plot(t_sensor_f1(170*2),x2(170*2),'^m');

legend('show')
grid minor
grid on
title('Comparacion empirico filtrado vs. teorico')