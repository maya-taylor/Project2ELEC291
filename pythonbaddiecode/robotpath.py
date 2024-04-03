import serial
import time
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import tkinter as tk
from tkinter import simpledialog

# voice recognition modules
# import pyaudio
# import speech_recognition as sr


# velocity at full is 0.8 second per breadboard
# 0.5 breadboards per second
# Global variables for joystick position
xpos_data, ypos_data = [], []
xpos = 0
ypos = 0
strength = 'None'
dis_trav = 0.0

# Robot path finding
canvas_scaling_factor = 0.16 # scaling final phasor magnitudes 
FORWARD_VELOCITY      = 15.0
CW_VELOCITY      	  = 119.0
CCW_VELOCITY      	  = 119.0

# Configure the serial port
ser = serial.Serial(
    port='COM16',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS
)
coordinates = {65: (0, 50), #A to N
               66: (0, 30),
               67: (0, 20),
               68: (0, -50),
               69: (0, -30),
               70: (0, -10),
               71: (50, 0),
               72: (30, 0),
               73: (-50, 0),
               74: (-30, 0),
               75: (20, 45),
               76: (-20, 45),
               77: (20, -45),
               78: (-20, -45), #N
               90: (0, 0)} #Z, AKA no movement

# Dictionary mapping magnitude and argument ranges to characters
ascii_mapping = {
    (0, 10, 0, 22.5): '0', (0, 10, 23, 45): '1', (0, 10, 46, 67.5): '2', (0, 10, 68, 90): '3',
    (0, 10, 91, 112.5): '4', (0, 10, 113, 135): '5', (0, 10, 136, 157.5): '6', (0, 10, 158, 180): '7',
    (0, 10, 181, 202.5): '8', (0, 10, 203, 225): '9', (0, 10, 226, 247.5): 'O', (0, 10, 248, 270): 'P',
    (0, 10, 271, 292.5): 'Q', (0, 10, 293, 315): 'R', (0, 10, 316, 337.5): 'S', (0, 10, 338, 360): 'T',

    (11, 20, 0, 22.5): 'U', (11, 20, 23, 45): 'V', (11, 20, 46, 67.5): 'W', (11, 20, 68, 90): 'X',
    (11, 20, 91, 112.5): 'Y', (11, 20, 113, 135): 'f', (11, 20, 136, 157.5): 'g', (11, 20, 158, 180): 'h',
    (11, 20, 181, 202.5): 'i', (11, 20, 203, 225): 'j', (11, 20, 226, 247.5): 'k', (11, 20, 248, 270): 'l',
    (11, 20, 271, 292.5): 'n', (11, 20, 293, 315): 'o', (11, 20, 316, 337.5): 'p', (11, 20, 338, 360): 'q',

    (21, 30, 0, 22.5): 'r', (21, 30, 23, 45): 's', (21, 30, 46, 67.5): 't', (21, 30, 68, 90): 'u',
    (21, 30, 91, 112.5): 'v', (21, 30, 113, 135): 'w', (21, 30, 136, 157.5): 'x', (21, 30, 158, 180): 'y',
    (21, 30, 181, 202.5): '`', (21, 30, 203, 225): '~', (21, 30, 226, 247.5): '!', (21, 30, 248, 270): '@',
    (21, 30, 271, 292.5): '#', (21, 30, 293, 315): '$', (21, 30, 316, 337.5): '%', (21, 30, 338, 360): '^',

    (31, 100, 0, 22.5): '&', (31, 100, 23, 45): '*', (31, 100, 46, 67.5): '(', (31, 100, 68, 90): ')',
    (31, 100, 91, 112.5): '-', (31, 100, 113, 135): '_', (31, 100, 136, 157.5): '+', (31, 100, 158, 180): '=',
    (31, 100, 181, 202.5): '{', (31, 100, 203, 225): '}', (31, 100, 226, 247.5): '[', (31, 100, 248, 270): ']',
    (31, 100, 271, 292.5): '|', (31, 100, 293, 315): '\\', (31, 100, 316, 337.5): ';', (31, 100, 338, 360): ':'
}

# GUI that allow the user to draw on canvas approximate their path, then send that data as instructions for the car

# latest - updated to incorporate basic functionality commands 

import tkinter as tk
import numpy as np
import cmath # for vector math


def on_option_select():
    selected = selected_option.get()
    result_label.config(text=f"Selected Option: {selected}")

    if selected == 'Draw':
       print("Draw Mode")
    elif selected == 'Joystick':
        print("Joystick Mode")
    elif selected == 'Voice Control':
        print("Voice Control Mode")
    else:
        print("Invalid Option")

root = tk.Tk()
root.title("Start by Manual Joystick Driving or Draw a Path")
root.geometry("400x100")
# Create a StringVar to hold the selected option
selected_option = tk.StringVar()
# Create the dropdown menu
options = ["Joystick", "Draw", "Voice Control"] # adding a voice control option
dropdown = tk.OptionMenu(root, selected_option, *options)
dropdown.pack(pady=10)
# Add a button to display the selected option
show_button = tk.Button(root, text="Show Selection", command=on_option_select)
show_button.pack()
# Label to display the selected option
result_label = tk.Label(root, text="")
result_label.pack()
root.mainloop()
# -- CHANGE MADE -- GL
# Moved this code to the `on_option_select` command

joystick_flag = 0 # should change this to a state variable?
if selected_option.get() == 'Joystick':
    joystick_flag = 1
    print("Joystick Mode")
elif selected_option.get() == 'Draw':
    joystick_flag = 0
    # root = tk.Tk()
    # app = PathDrawer(root)
    # root.mainloop()
    

class PathDrawer:
    def __init__(self, master):
        self.master = master
        self.canvas = tk.Canvas(self.master, width=400, height=400, bg="white") # canvas value previously 400, changed down to 120
        self.canvas.pack()

        # Data array attributes ot the class
        self.data_points = []   # Draw datapoints from canvas drawing
        self.approx_points = [] # Approximated waypoints of the drawn path
        self.vectors_list = []  # Turning into lists of vectors
        self.phasors_list = []  # Angle for CW rotation as a list calculated from previous vector

        master.protocol("WM_DELETE_WINDOW", self.on_closing)

        # ser = serial.Serial(
        #     port='COM16',
        #     baudrate=9600,
        #     parity=serial.PARITY_NONE,
        #     stopbits=serial.STOPBITS_TWO,
        #     bytesize=serial.EIGHTBITS
        # )

        self.canvas.bind("<Button-1>", self.on_click)
        self.canvas.bind("<B1-Motion>", self.on_drag)

        self.clear_button = tk.Button(self.master, text="Clear", command=self.clear_canvas) # call clear canvas command on press
        self.clear_button.pack(side=tk.LEFT)

        self.send_button = tk.Button(self.master, text="Send", command=self.send_data)   # call send data command on press
        self.send_button.pack(side=tk.LEFT)

        # Custom paths button
        self.square_button = tk.Button(self.master, text="Draw squre", command=self.draw_square)
        self.square_button.pack(side=tk.LEFT)

        self.approx_button = tk.Button(self.master, text="Approximate", command=self.approximate_and_draw) # call approximate_and_draw command on press
        self.approx_button.pack(side=tk.LEFT)
        
        # Input value which will be used to determine the resolution of path approximation
        self.num_segments_entry = tk.Entry(self.master)
        self.num_segments_entry.pack(side=tk.LEFT)
        self.num_segments_entry.insert(tk.END, "5")

    def on_click(self, event):
        self.data_points.append((event.x, event.y))

    def on_drag(self, event):
        self.data_points.append((event.x, event.y))
        self.draw_lines()

    def draw_lines(self):
        self.canvas.delete("line")
        if len(self.data_points) >= 2:
            for i in range(len(self.data_points) - 1):
                x0, y0 = self.data_points[i]
                x1, y1 = self.data_points[i + 1]
                self.canvas.create_line(x0, y0, x1, y1, tags="line")

    def clear_canvas(self):
        self.canvas.delete("all")
        self.data_points = []

    # go through datapoints array and select in even intervals
    # datapoints selected from points[] array (generated from on_drag or on_clikc events)
    # number of datapoints to use determined by num_segments, higher creates more
    def approximate_and_draw(self):
        approx_points = []
        num_segments = int(self.num_segments_entry.get())
        length_data = len(self.data_points)

        sampling_interval = int(length_data/num_segments)

        # path approximation is done by linearly sampling self.data_points
        # spaced evenly using the modulus operator then preserving end point
        for i in range (0, length_data):
            if (i % sampling_interval == 0):
                approx_points.append(self.data_points[i])

        # Due to the nature of sampling, if if `length_data` is k*num_segments, integer truncation doesn't happen
        # Result is that don't take the last datapoint
        if ((length_data) % num_segments == 0):
            approx_points.append(self.data_points[length_data-1])

        self.canvas.delete("line") # delete previous path on the canvas on press
        self.approx_points = approx_points #save as attribute to self

        # plot new approximated path
        for i in range(len(approx_points) - 1):
            x0, y0 = approx_points[i]
            x1, y1 = approx_points[i + 1]
            self.canvas.create_line(x0, y0, x1, y1, tags="line")

        self.convert_to_vectors()
        self.convert_to_phasors()

    def convert_to_vectors(self):
        self.vectors_list = []
        prev_point = self.approx_points[0]
        for point in self.approx_points[1:]:
            dx = point[0] - prev_point[0]
            dy = point[1] - prev_point[1]
            magnitude = abs(complex(dx, -dy))       #tkinter has downwards as positive, flipped here for calculations
            phase = cmath.phase(complex(dx, -dy))   #tkinter has downwards as positive, flipped here for calculations
            self.vectors_list.append((int(magnitude), int(phase*180/np.pi)))
            prev_point = point

    def convert_to_phasors(self):
        self.phasors_list = []
        prev_vector = [0, 90]
        for vector in self.vectors_list:
            phase = prev_vector[1] - vector[1]
            if (phase < 0):
                phase = phase + 360 # negative angles is a CCW turn, convert to a CW turn by adding 360 degrees

            self.phasors_list.append([int(vector[0]*canvas_scaling_factor), phase]) # phasor with scaled vector + phase
            prev_vector = vector
        
        print("Phasors List:", self.phasors_list)

    def send_data(self):
        print("Phasors:") # send data to serial on a set time  
        ser.write("..\r\n".encode())  # send over serial to JDY40
        for phasor in self.phasors_list:
            phasor_ascii_char = self.polar_to_ascii(phasor[0], phasor[1])
            
            # calculate amount of time that current path should take
            if (phasor[1] <= 180):
                wait_time = phasor[1] / CW_VELOCITY + phasor[0] / FORWARD_VELOCITY
            else:
                wait_time = abs(360-phasor[1]) / CCW_VELOCITY + phasor[0] / FORWARD_VELOCITY
            # adding a slight buffer so that new instruction isn't sent
            # while executing current instruction robotpath in .c is blocking
            wait_time = wait_time * 1.10 + 0.2
            # wait_time = 1

            ser.write(f"{phasor_ascii_char*2}\r\n".encode())  # send over serial to JDY40
            print(f"Sent string = {phasor_ascii_char*2}\r\n".encode())      # check this string
            print(f"{phasor},{phasor_ascii_char}, wait = {wait_time}") # print what character is sent
            
            time.sleep(wait_time)                 # pause program while we wait for phasors to be sent over
        ser.write(",,\r\n".encode())  # send over serial to JDY40
        print("Finished!")

    def draw_square(self):
        self.clear_canvas()
        side_length = min(self.canvas.winfo_width(), self.canvas.winfo_height()) / 2
        center_x = self.canvas.winfo_width() / 2
        center_y = self.canvas.winfo_height() / 2
        top_left = (center_x - side_length / 2, center_y - side_length / 2)
        top_right = (center_x + side_length / 2, center_y - side_length / 2)
        bottom_right = (center_x + side_length / 2, center_y + side_length / 2)
        bottom_left = (center_x - side_length / 2, center_y + side_length / 2)
        # self.data_points = [top_left, top_right, bottom_right, bottom_left, top_left]
        self.data_points = [bottom_left, top_left, top_right, bottom_right, bottom_left]
        self.draw_lines()

        # create phasors array
        self.phasors_list = []
        self.phasors_list.append([50,0])
        self.phasors_list.append([50,90])
        self.phasors_list.append([50,90])
        self.phasors_list.append([50,90])
        self.phasors_list.append([0,90]) #reset to original direction

    def polar_to_ascii(self, magnitude, argument):
    # Iterate through the dictionary to find the corresponding character
        for key, value in ascii_mapping.items():
            if key[0] <= magnitude <= key[1] and key[2] <= argument <= key[3]:
                return value
        
        # Return None if no matching character is found
        return None

    def on_closing(self):
        self.master.destroy()  # Destroy the Tkinter window

# check if joystick_flag is zero
if (joystick_flag == 0):
    # Create an instance of the PathDrawer class
    # print("draw yay")
    root = tk.Tk()
    drawer = PathDrawer(root)   
    # start main tkinter loop
    root.mainloop()
    # root.destroy()
    

    joystick_flag = 1



def get_coordinates(letter):
    return coordinates.get(letter, (0, 0))

def map_letter_to_level(letter):
    if letter == 97:
        return 'Level 1'
    elif letter == 98:
        return 'Level 2'
    elif letter == 99:
        return 'Level 3'
    elif letter == 100:
        return 'Level 4'
    elif letter == 101:
        return 'Level 5'
    else:
        return 'None'  # Return an empty string for 'z' or any other letter
    

def polar_to_ascii(magnitude, argument):
    # Iterate through the dictionary to find the corresponding character
    for key, value in ascii_mapping.items():
        if key[0] <= magnitude <= key[1] and key[2] <= argument <= key[3]:
            return value
    
    # Return None if no matching character is found
    return None


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
    plt.text(0.8, 0.8, f"Velocity in Breadboards Per Second: {round((ypos/50)*1.25,2)}\nMetal Strength: {strength}", fontsize=10, ha='right', va='top', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.5'))
    
    # Add label showing the x value in the lower left corner with box
    plt.text(-0.8, -0.8, f"Angular Velocity Factor: {xpos/10} \n Total Distance Travelled (Breadboards) = {round(dis_trav+ypos/50*1.25*0.3*0.6,2)}", fontsize=10, ha='left', va='bottom', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.5'))
    dis_trav += abs(ypos/50*1.25*0.3*0.6)
    plt.draw()
    plt.pause(0.002)


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

if(joystick_flag == 1):
    plt.show()

while joystick_flag:
    try:
        # Read data from serial port
        strin = ser.readline()
        # print(strin)
        if strin:
            letter = strin[0]
            #print(letter)
            #41 to 78 corresponds to ASCII A to N, 90 is 'Z'
            if (letter >= 41 and letter <= 78) or letter == 90:   
                #print(letter)
                xpos, ypos = get_coordinates(letter)
                #print("Joystick positions: %d, %d" % (xpos, ypos))
                update_joystick_position(xpos, ypos)
                #ser.write(b'o\r\n')

            #97 to 101 corresponds to ASCII a to e, 122 is 'z'
            elif (letter >= 97 and letter <= 101) or letter == 122:
               strength = map_letter_to_level(letter)
               #print(f"metal strength is {strength}")

    except serial.SerialException as e:
        print("Serial communication error:", e)
        # Attempt to reopen the serial port
        ser.close()
        ser.open()
    except Exception as e:
        print("An error occurred:", e)
