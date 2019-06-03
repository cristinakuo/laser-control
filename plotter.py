import matplotlib.pyplot as plt
import numpy as np

# Curva teorica
dt_arduino = np.loadtxt('tiempos_arduino.txt')
t_teorico = dt_arduino.cumsum(axis=0)

# Datos empiricos
MOTOR_FILE = 'motor_output.txt'
SENSOR_FILE= 'sensor_output.txt'

t_arduino = np.loadtxt(MOTOR_FILE)
t_sensor = np.loadtxt(SENSOR_FILE)

plt.figure(1)
# Teorico
x = np.linspace(10,0,len(t_teorico))
plt.plot(t_teorico,x,'g.', markersize=3, label='Teorico')
plt.grid(color='r', linestyle='dotted',which='both', linewidth=0.5)

# Arduino 
step = 0.05
end = 10 - len(t_arduino)*step
x = np.arange(10,end,-step)
plt.plot(t_arduino,x,'r.', markersize=3, label='Arduino')

# Sensor
step = 0.025
end = 10 - len(t_sensor)*step
x = np.arange(10,end,-step)
plt.plot(t_sensor,x,'b.', markersize=3, label='Sensor')
plt.title('Datos empiricos vs. teoricos')
legend = plt.legend(loc='upper right', shadow=False, fontsize='small')
plt.xlabel('Tiempo')
plt.ylabel('Posicion')
plt.show()



