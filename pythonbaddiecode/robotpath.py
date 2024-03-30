import serial
import time
import matplotlib.pyplot as plt

#velocity at full is 0.8 second per breadboard
#1.25 breadboards per second
# Global variables for joystick position
xpos_data, ypos_data = [], []

# Function to update joystick position
def update_joystick_position(xpos, ypos):
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
    plt.text(0.8, 0.8, f"Velocity in Breadboards Per Second: {ypos/50*1.25}", fontsize=10, ha='right', va='top', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.5'))
    
    # Add label showing the x value in the lower left corner with box
    plt.text(-0.8, -0.8, f"Angular Velocity Factor: {xpos/10}", fontsize=10, ha='left', va='bottom', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.5'))
    
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
            if len(strin) == 9 and strin[0] != '1':
                xpos = int(strin[0:3])
                if (xpos > 50):
                    xpos = 0
                    ypos = 0 #this handles if it accidentally read the frequency strin
                ypos = int(strin[4:])

                print("Joystick positions: %d, %d" % (xpos, ypos))
                update_joystick_position(xpos, ypos)
            elif len(strin) == 11:
                mag_freq = float(strin[0:])
                print("Magnitude of frequency: %.2f" % mag_freq)

    except serial.SerialException as e:
        print("Serial communication error:", e)
        # Attempt to reopen the serial port
        ser.close()
        ser.open()
    except Exception as e:
        print("An error occurred:", e)

