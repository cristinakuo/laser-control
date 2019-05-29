clc
clear all
% close all
%% Curva teorica
dt_arduino = dlmread('tiempos_arduino.txt');
t_teorico = cumsum(dt_arduino);
%% Datos empiricos
MOTOR_FILE = 'motor_output.txt';
SENSOR_FILE= 'sensor_output.txt';

t_arduino = dlmread(MOTOR_FILE,'\n')';
t_arduino = t_arduino(1:end-1);
t_sensor = dlmread(SENSOR_FILE,'\n')';
t_sensor = t_sensor(1:end-1);

mm_per_10step = 1/200*20;

%k = 0; % Interval counter
%t_arduino = zeros(200,1);
%n = 1;

% Separate arduino data from sensor data
%while n<length(data)
%   if data(n) == 0
%       t_arduino(k+1) = data(n+1);
%       data(n:n+1) = [];
%       k=k+1;
%   else
%       n = n+1;
%   end
%end

%t_sensor = data;
%clear data;
%% Grafico t_arduino vs. t_sensor vs. teorico
figure
hold on
% Teorico
x1 = linspace(10,0,length(t_teorico));
plot(t_teorico,x1,'.g','DisplayName', 'Teorico');
% Arduino
%x1 = linspace(10,0,length(t_arduino));
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


%% Segundo filtrado
t_sensor_f2 = t_sensor
% Arreglo de distancias
dist_array = diff(t_arduino);


new_array = zeros(199,1); % Inicializo array nuevo
new_array(1) = t_sensor(1); % Tomamos que el primero es el 1er punto
t_sensor(1)=[];
center = new_array(1)+dist_array(1); % Inicializo centro
%%
n = 2;
while n <  200
    [new_array(n),ind] = closest_to_value(center,t_sensor);
    center= new_array(n)+dist_array(n);
    t_sensor(ind)=[];
    n = n+1;
end
%%
t_sensor_f2 = new_array;
figure
hold on
x1 = linspace(10,0,length(original_t_sensor));
plot(original_t_sensor,x1,'.k','DisplayName', 'Original')
x2 = linspace(10,0,length(t_sensor_f1));
plot(t_sensor_f1,x2,'.g','DisplayName', 'Con filtro 1')
x3 = linspace(10,0,length(t_sensor_f2));
plot(t_sensor_f2,x3,'.r','DisplayName', 'Con filtro 2');
legend('show')
title('Comparacion sensor vs. sensor filtrado 1 y 2')
grid on
grid minor


figure
hold on
%subplot(2,1,1)
x1 = linspace(10,0,length(t_arduino));
plot(t_arduino,x1,'.r','DisplayName', 'Arduino');


%subplot(2,1,2)
%x2 = linspace(10,0,length(t_sensor_f1));

plot(t_sensor_f1,x2,'.b','DisplayName', 'Sensor filtrado');
legend('show')
grid on
grid minor







