import serial
import csv 

# Configure the serial port
ser = serial.Serial(
    port='/dev/tty.URT0',  # Change this to your serial port
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

# Open a CSV file for writing
csv_file = open('serial_data.csv', 'w', newline='')
csv_writer = csv.writer(csv_file)

try:
    while True:
        # Read data from the serial port
        serial_data = ser.readline().decode().strip()
        
        # Split the data into individual values (if needed)
        data_values = serial_data.split(',')  # Adjust this based on your data format
        
        # Write the data to the CSV file
        csv_writer.writerow(data_values)
        
        # Print the data to the console
        # print(data_values)
finally:
    # Close the serial port and CSV file
    ser.close()
    csv_file.close()
