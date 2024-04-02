# prints to terminal directly from keyboard input
# if using up-down-left-right, ie. WASD, type the letter followed by anything that isn't these four letters
# if using this for diagonals, type w then a, a then s, s then d, or d then w - to move first in the direction of the first value and then in a diagonal
# press ctrl+c to escape system
import time
import serial
import curses

# Initialize serial port
ser = serial.Serial(
    port='COM16',  # Change this to your serial port
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS
)

key_to_coords = {
    ord('w'): ('C'),  # forward mid, char 'C'
    ord('a'): ('J'),  # CCW in place mid (Left), char 'J'
    ord('s'): ('E'),  # backward mid, char 'E'
    ord('d'): ('H'),  # CW in place mid (Right), char 'H'
}

# Print letter to serial port / terminal
def send_letter(letter):
    #ser.write(letter.encode()) # change this if you want to print to serial
    print(letter)

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