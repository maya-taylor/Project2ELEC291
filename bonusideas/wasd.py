#uses the same format of strings as george (to be processed into letters before sending

#redone for windows
import time
import serial
import curses

# Initialize serial port

#ser = serial.Serial(
#    port='/dev/cu.usbserial-D30APJ7X',  # Change this to your serial port
#    baudrate=115200,
#    parity=serial.PARITY_NONE,
#    stopbits=serial.STOPBITS_TWO,
#    bytesize=serial.EIGHTBITS
#)

# for jdy
ser = serial.Serial('COM16', 9600, timeout = 1)

key_to_command = {
    ord('w'): ('B'),  # forward mid, char 'B'
    ord('a'): ('J'),  # CCW in place mid (Left), char 'J'
    ord('s'): ('E'),  # backwards mid
    ord('d'): ('H'),  # CW in place mid (Right), char 'H'
    ord('q'): ('L'), # NW
    ord ('e'): ('N'),
    ord('z'):('M'),
    ord('c'):('K')
}

# Print letter to serial port / terminal
def send_letter(letter):
    #ser.write(letter.encode('ascii')) # change this if you want to print to serial
    
    #jdy
    ser.write(f"{letter*2}\r\n".encode())  # send over serial to JDY40 <- use this one
    #prints corresponding letter to terminal
    print(letter+"\r\n")


# Main loop
try:
    # Initialize curses
    stdscr = curses.initscr()# creates a curses window object
    #stdscr.nodelay(True)      # this configures getch() to be non-blocking and returns -1
    curses.noecho()  # Turn off echoing of keys
    stdscr.keypad(True)  # Enable keypad mode
    curses.cbreak()  # React to keys immediately without waiting for Enter key
    stdscr.clear() # added
    stdscr.addstr(0, 0, "Press 'ESC' to quit...")

    while True:        
        # Get user input
        key = stdscr.getch()
        print(key) # check if key is -1 for no inputs -- comment out later

        if key == 27: # if escape key is pressed, exit
            break
        # maybe add the z here? not sure how you want the logic here
        #ser.write(f"{letter*2}\r\n".encode())
        # Check for keypresses

        if key in key_to_command: # will be true if it's in a diagonal
            send_letter(key_to_command[key])        
        else:
            send_letter('Z') #stop

        
        if key == -1: # .getch returns -1 for no input
            send_letter('Z')

        # Wait for 1 second to reduce errors
        time.sleep(0.1)

except KeyboardInterrupt:
    pass

finally:
    # Cleanup curses
    #curses.initscr()
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

    ser.close()  # Close the serial port when done

    # call the main function to run it
#curses.wrapper(main)

'''
import time
import serial
import curses

# Initialize serial port

ser = serial.Serial(
    port='/dev/cu.usbserial-D30APJ7X',  # Change this to your serial port
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS
)

# for jdy
ser = serial.Serial('COM16', 9600, timeout = 1)

key_to_coords = {
    ord('w'): ('C'),  # forward mid, char 'C'
    ord('a'): ('J'),  # CCW in place mid (Left), char 'J'
    ord('s'): ('E'),  # backward mid, char 'E'
    ord('d'): ('H'),  # CW in place mid (Right), char 'H'
}

# Print letter to serial port / terminal
def send_letter(letter):
    #ser.write(letter.encode('ascii')) # change this if you want to print to serial
    #ser.write(f"{letter*2}\r\n".encode())  # send over serial to JDY40 <- use this one
    #jdy
    print(letter+"\r\n")


# Main loop
try:
    # Initialize curses
    stdscr = curses.initscr()
    curses.noecho()  # Turn off echoing of keys
    stdscr.keypad(True)  # Enable keypad mode
    curses.cbreak()  # React to keys immediately without waiting for Enter key

    while True:
        x, y = 0, 0
        
        # Get user input
        key = stdscr.getch()

        # Check for keypresses
        if key in key_to_coords:
            send_letter(key_to_coords[key])

        # Check for diagonal movements
        # NW Diagonal
        if key == ord('w') and stdscr.getch() == ord('a'):
            send_letter('L')

        # SW Diagonal
        elif key == ord('a') and stdscr.getch() == ord('s'):
            send_letter('N')

        # SE Diagonal
        elif key == ord('s') and stdscr.getch() == ord('d'):
            send_letter('M')

        # NE Diagonal
        elif key == ord('d') and stdscr.getch() == ord('w'):
            send_letter('K')

        # Wait for 1 second to reduce errors
        #time.sleep(1)

except KeyboardInterrupt:
    pass

finally:
    # Cleanup curses
    curses.initscr()
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

    ser.close()  # Close the serial port when done
'''

'''
# for testing that the curses library works

import curses

def test_curses():
    # Initialize curses
    stdscr = curses.initscr()
    curses.noecho()  # Turn off echoing of keys
    stdscr.keypad(True)  # Enable keypad mode
    curses.cbreak()  # React to keys immediately without waiting for Enter key

    try:
        stdscr.addstr("Press keys to see their ASCII values. Press 'q' to quit.")
        stdscr.refresh()

        while True:
            key = stdscr.getch()
            stdscr.clear()

            if key == ord('q'):
                break

            stdscr.addstr(f"Pressed key: {chr(key)} (ASCII: {key})")
            stdscr.refresh()

    finally:
        # Cleanup curses
        curses.nocbreak()
        stdscr.keypad(False)
        curses.echo()
        curses.endwin()

# Run the test function
test_curses()
''' 
'''
# test code below!! 

import curses

def main(stdscr):
    # Turn off echoing of keys, and enter cbreak mode,
    # where no buffering is performed on keyboard input
    curses.cbreak()
    stdscr.keypad(True)

    stdscr.clear()
    stdscr.addstr(0, 0, "Type some characters (Press 'q' to quit):")

    while True:
        # Get a character from the keyboard
        c = stdscr.getch()

        # Check if the character is 'q'
        if c == ord('q'):
            break

        # Print the character typed
        stdscr.addch(c)

    # Cleanup
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

# Run the main function using curses wrapper
curses.wrapper(main)
'''