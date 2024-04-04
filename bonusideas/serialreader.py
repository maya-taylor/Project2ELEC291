import serial
import csv 

# Configure the serial port (change accordingly)
ser = serial.Serial(
    port='COM16',  
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

# Open a CSV file for writing; use as a log of all the serial outputs
csv_file = open('serial_data_log.csv', 'w', newline='')
csv_writer = csv.writer(csv_file)

try:
    while True: #continuously read lines from the serial port
        # Read data from the serial port
        serial_data = ser.readline().decode().strip()
        
        
        # Write the data to the CSV file
        csv_writer.writerow(serial_data)
        
finally:
    # Close the serial port and CSV file
    ser.close()
    csv_file.close()
