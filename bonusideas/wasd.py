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

# Function to send coordinates
def send_coordinates(x, y):
    ser.write(f"{x},{y}\n".encode())

# Mapping of keys to coordinates
key_to_coords = {
    'w': (0, 20),
    'a': (-20, 0),
    's': (0, -20),
    'd': (20, 0)
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
        if keyboard.is_pressed('w') and keyboard.is_pressed('a'):
            x -= 10
            y += 10
        elif keyboard.is_pressed('a') and keyboard.is_pressed('s'):
            x -= 10
            y -= 10
        elif keyboard.is_pressed('s') and keyboard.is_pressed('d'):
            x += 10
            y -= 10
        elif keyboard.is_pressed('w') and keyboard.is_pressed('d'):
            x += 10
            y += 10
          
        # Send coordinates
        send_coordinates(x, y)
        
        # Wait for 1 second to reduce errors
        time.sleep(1)

except KeyboardInterrupt:
    pass

finally:
    ser.close()  # Close the serial port when done
