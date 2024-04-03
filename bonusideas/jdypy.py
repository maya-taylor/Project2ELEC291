import serial
import time

# Configure serial port
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)  # Adjust port and baud rate as needed

try:
    while True:
        # Send AT command to JDY-40 module
        ser.write(b'AT\r\n')
        time.sleep(1)  # Wait for response

        # Read response
        response = ser.readline().decode().strip()
        print("Response:", response)

        # Read data from JDY-40 module
        ser.write(b'AT+DATA?\r\n')
        time.sleep(1)  # Wait for response

        # Read response
        response = ser.readline().decode().strip()
        print("Data:", response)

        # Wait before sending next command
        time.sleep(2)

except KeyboardInterrupt:
    ser.close()  # Close serial port on Ctrl+C exit
