#uses the same format of strings as george (to be processed into letters before sending

import time
import keyboard
import serial

# Initialize serial port
ser = serial.Serial(
    port='/dev/tty.URT0',  # Change this to your serial port
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)
'''
# Function to send coordinates
def send_coordinates(x, y):
    ser.write(f"{x},{y}\n".encode())

# Mapping of keys to coordinates
key_to_coords = {
    'w': (0, 30), # forward mid, char 'C'
    'a': (-30, 0), #  CCW in place mid (Left), char 'J'
    's': (0, -30), # backward mid, char 'E'
    'd': (30, 0) # CW in place mid (Right), char 'H'
}

# Main loop
try:
    while True:
        x, y = 0, 0
        
        # Check for keypresses
        for key, (dx, dy) in key_to_coords.items():
            if keyboard.is_pressed(key):
                x += dx
                y += dy


        # Check for diagonal movements
        # NW Diagonal
        if keyboard.is_pressed('w') and keyboard.is_pressed('a'):
            x -= 10
            y += 10

        # SW Diagonal
        elif keyboard.is_pressed('a') and keyboard.is_pressed('s'):
            x -= 10
            y -= 10

        # SE Diagonal
        elif keyboard.is_pressed('s') and keyboard.is_pressed('d'):
            x += 10
            y -= 10

        # NE Diagonal
        elif keyboard.is_pressed('w') and keyboard.is_pressed('d'):
            x += 10
            y += 10
          
        # Send coordinates
        send_coordinates(x, y)
'''

# Send letters
# Mapping of keys to letters
key_to_coords = {
    'w': 'C', # forward mid, char 'C'
    'a': 'J', #  CCW in place mid (Left), char 'J'
    's': 'E', # backward mid, char 'E'
    'd': 'H', # CW in place mid (Right), char 'H'

    #diagonals
    # NW, SW, SE, NE

}    
# Function to send coordinates
def send_coordinates(x, y):
    ser.write(f"{x},{y}\n".encode())

# Main Loop
try: 
    while (True):
        # Check for keypresses
        for key, (dx, dy) in key_to_coords.items():
            if keyboard.is_pressed(key):
        
        # Wait for 1 second to reduce errors
        time.sleep(1)

except KeyboardInterrupt:
    pass

finally:
    ser.close()  # Close the serial port when done
