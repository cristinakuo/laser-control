import serial
import sys
import time
import argparse

BAUDRATE = 9600
COMPORT_MOTORS = 'COM9'
COMPORT_SENSOR = 'COM6' # TODO: Ver cual es 

parser = argparse.ArgumentParser()
parser.add_argument("-mo", "--motoroutput", required=False, help="Motor output file name")
parser.add_argument("-so", "--sensoroutput", required=False, help="Sensor output file name")

def parse_cmdline():
    # Parse commandline arguments
    parsed_arguments = parser.parse_args()
    # Convert to dictionary type
    args_dictionary = vars(parsed_arguments)
    
    motorfile = args_dictionary["motoroutput"]
    sensorfile = args_dictionary["sensoroutput"]
    print(motorfile) # DEBUG
    print(sensorfile) # DEBUG
    return [motorfile,sensorfile]

def establish_serial(comport,baudrate):
    try:
        connection = serial.Serial(comport,baudrate)
        #time.sleep(2) # Wait for arduino to start, otherwise whatever you send to it will be lost
        print("Serial "+ comport +" connection was succesful.")
        return connection
    except:
        print('Oh no! ' + comport + ' wasn\'t available. Please, try checking the connection.')
        sys.exit(1)



if __name__ == "__main__":

    comSensor = establish_serial(COMPORT_SENSOR,BAUDRATE)
    comMotors = establish_serial(COMPORT_MOTORS,BAUDRATE)
    [motorfile,sensorfile] = parse_cmdline()
    f_motor = open(motorfile,'w+')
    f_sensor = open(sensorfile,'w+')

    msg_from_motors = ""
    msg_from_sensor = ""
    
    while  msg_from_motors.startswith('Stop') == False:
    #while True:    
        while comMotors.in_waiting > 0:
            msg_from_motors = comMotors.readline().decode('UTF-8')                   
            print("Motor: "+msg_from_motors, end = "") # No new line
            if msg_from_motors.startswith('Stop') == False:
                f_motor.write(msg_from_motors[:-1]) # No new line

    #while comSensor.in_waiting >0:    
        while comSensor.in_waiting > 0:
            msg_from_sensor = comSensor.readline().decode('UTF-8')                   
            print("Sensor: "+msg_from_sensor, end = "") # No new line
            f_sensor.write(msg_from_sensor[:-1])

    print('Done python')

	