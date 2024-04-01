
import serial
import time
import matplotlib.pyplot as plt

#velocity at full is 0.8 second per breadboard
#0.5 breadboards per second
# Global variables for joystick position
xpos_data, ypos_data = [], []
dis_trav = 0.0

def get_coordinates(letter):
    coordinates = {'A': (0, 50),
                   'B': (0, 30),
                   'C': (0, 20),
                   'D': (0, -50),
                   'E': (0, -30),
                   'F': (0, -10),
                   'G': (50, 0),
                   'H': (30, 0),
                   'I': (-50, 0),
                   'J': (-30, 0),
                   'K': (20, 45),
                   'L': (-20, 45),
                   'M': (20, -45),
                   'N': (-20, -45),
                   'Z': (0, 0)}

    return coordinates.get(letter.upper(), (0, 0))

def map_letter_to_level(letter):
    if letter == 'a':
        return 'level1'
    elif letter == 'b':
        return 'level2'
    elif letter == 'c':
        return 'level3'
    elif letter == 'd':
        return 'level4'
    elif letter == 'e':
        return 'level5'
    else:
        return 'NADA'  # Return an empty string for 'z' or any other letter

# Function to update joystick position
def update_joystick_position(xpos, ypos):
    global dis_trav
    xpos_data.append(xpos/50)
    ypos_data.append(ypos/50)
    
    
    # Limit data to last 100 points for better visualization
    if len(xpos_data) > 1:
        xpos_data.pop(0)
        ypos_data.pop(0)
    # Update plot
    plt.clf()  # Clear the previous plot
    plt.plot(xpos_data, ypos_data, 'o-', color='blue')  # Plot joystick position
    plt.xlim(-1, 1)  # Set x-axis limit
    plt.ylim(-1, 1)  # Set y-axis limit
    plt.xlabel('X Position')
    plt.ylabel('Y Position')
    plt.title('Joystick Position')
    plt.grid(True)
    
    # Plot a unit circle
    circle = plt.Circle((0, 0), 1, color='lightgray', alpha=0.5)
    plt.gca().add_artist(circle)
    
    # Plot the line as a vector coming out from the center
    plt.plot([0, xpos/50], [0, ypos/50], color='red')
    
    # Add label showing the y value in the upper right corner with box
    plt.text(0.8, 0.8, f"Velocity in Breadboards Per Second: {round((ypos/50)*1.25,2)}", fontsize=10, ha='right', va='top', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.5'))
    
    # Add label showing the x value in the lower left corner with box
    plt.text(-0.8, -0.8, f"Angular Velocity Factor: {xpos/10} \n Total Distance Travelled (Breadboards) = {round(dis_trav+ypos/50*1.25*0.3*0.6,2)}", fontsize=10, ha='left', va='bottom', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.5'))
    dis_trav += abs(ypos/50*1.25*0.3*0.6)
    plt.draw()
    plt.pause(0.001)

# Configure the serial port
ser = serial.Serial(
    port='COM16',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS
)

plt.ion()  # Turn on interactive mode for Matplotlib

# Create a figure
fig = plt.figure()

# Function to handle figure close event
def on_close(event):
    plt.close()  # Close the plot window immediately
    ser.close()  # Close the serial port
    exit()  # Exit the program

# Attach the close event handler to the figure
fig.canvas.mpl_connect('close_event', on_close)

while True:

    
    try:
        # Read data from serial port
        strin = ser.readline()
        
        if strin:
            if len(strin) == 2: # I think the string we recieve is two long : )
                if strin[0] in ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'Z']: #This will only execute if we send in movement from joystick
                    letter = strin[0]
                #if (xpos > 50): #old code 
                #    xpos = 0
                 #   ypos = 0 #this handles if it accidentally read the frequency strin
                #ypos = int(strin[4:])
                    xpos, ypos = get_coordinates(letter) #This can be used to parse the letter into x y coordinates.
                    
                elif strin[0] in ['a', 'b', 'c', 'd', 'e', 'z']: #This will execute if we read frequency info
                   freq = map_letter_to_level(letter)
                    

                print("Joystick positions: %d, %d" % (xpos, ypos))
                update_joystick_position(xpos, ypos)
       #     elif len(strin) == 11:  #Were gonna have to change this to deal with levels -> maybe we show the beep level?
       #         mag_freq = float(strin[0:])
       #         print("Magnitude of frequency: %.2f" % mag_freq)
       

    except serial.SerialException as e:
        print("Serial communication error:", e)
        # Attempt to reopen the serial port
        ser.close()
        ser.open()
    except Exception as e:
        print("An error occurred:", e)

