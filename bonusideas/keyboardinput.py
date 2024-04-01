# File to directly control car movements using letters on keyboard based on chart definitions. 

import serial
import time
import matplotlib.pyplot as plt
from pynput import keyboard

ser = serial.Serial(
    port='COM16',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS
)

# If key is pressed, send the ascii code to the serial
def pressed (key):
    try:
        # Convert the pressed key to its corresponding ASCII code
        key_char = key.char
        ascii_code = ord(key_char)
        
        # Send the ASCII code through the serial port
        ser.write(bytes([ascii_code]))
        
        # Print the pressed key
        print(f"Sent {key_char}")
    except AttributeError:
        # Ignore non-character keys
        pass

# check for release of key - default button to escape program is escape

def released (key):
    if key == keyboard.Key.esc: #keeps track of whether or not excape key is pressed
        return False

with keyboard.Listener (pressed = pressed, released = released) as listener:
    listener.join()

ser.close()