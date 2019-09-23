import serial
import sys
import time
import argparse

BAUDRATE = 9600
COMPORT_MOTORS = 'COM13'
COMPORT_SENSOR = 'COM6' # TODO: Ver cual es 
SENSOR_FILE_DEFAULT = 'sensor_output.txt'
MOTOR_FILE_DEFAULT = 'motor_output.txt'

parser = argparse.ArgumentParser()
parser.add_argument("-mo", "--motoroutput", required=False, help="Motor output file name")
parser.add_argument("-so", "--sensoroutput", required=False, help="Sensor output file name")

# If no output file names are passed, default file names are used
def parse_cmdline():
    # Parse commandline arguments
    parsed_arguments = parser.parse_args()
    # Convert to dictionary type
    args_dictionary = vars(parsed_arguments)
    
    motorfile = args_dictionary["motoroutput"] 
    sensorfile = args_dictionary["sensoroutput"]
    
    if sensorfile == None:
        print("Using default sensor file.")
        sensorfile = SENSOR_FILE_DEFAULT
    if motorfile == None:
        print("Using default motor file.")
        motorfile = MOTOR_FILE_DEFAULT

    return [motorfile,sensorfile]


def establish_serial(comport,baudrate):
    try:
        connection = serial.Serial(comport,baudrate)
        print("Serial "+ comport +" connection was succesful.")
    except:
        print('Oh no! ' + comport + ' wasn\'t available. Please, try checking the connection.')
        sys.exit(1)

    return connection



if __name__ == "__main__":
    [motorfile,sensorfile] = parse_cmdline()

    # Sensor connection has to be established first, otherwise motors 
    # would start before sensor can detect steps.
    comSensor = establish_serial(COMPORT_SENSOR,BAUDRATE)
    comMotors = establish_serial(COMPORT_MOTORS,BAUDRATE)
    
    # Open files in writing mode
    f_motor = open(motorfile,'w+')
    f_sensor = open(sensorfile,'w+')

    msg_from_motors = ""
    msg_from_sensor = ""
    
    while  msg_from_motors.startswith('Stop') == False: 
        while comMotors.in_waiting > 0:
            msg_from_motors = comMotors.readline().decode('UTF-8')                   
            print("Motor: " + msg_from_motors, end = "") # No new line
            if msg_from_motors.startswith('Stop') == False:
                f_motor.write(msg_from_motors[:-1]) # Last char is '\n', but .write already adds new line
 
        while comSensor.in_waiting > 0:
            msg_from_sensor = comSensor.readline().decode('UTF-8')                   
            print("Sensor: " + msg_from_sensor, end = "")
            f_sensor.write(msg_from_sensor[:-1]) 

    print('Done python.')

	