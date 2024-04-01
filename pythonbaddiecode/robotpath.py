import serial
import time
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import tkinter as tk
from tkinter import simpledialog


# velocity at full is 0.8 second per breadboard
# 0.5 breadboards per second
# Global variables for joystick position
xpos_data, ypos_data = [], []
xpos = 0
ypos = 0
strength = 'None'
dis_trav = 0.0

# Robot path finding
canvas_scaling_factor = 0.5 # scaling final phasor magnitudes 
FORWARD_VELOCITY      = 15.0
CW_VELOCITY      	  = 119.0

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
    (0, 10, 0, 14): '0', (0, 10, 15, 29): '1', (0, 10, 30, 44): '2', (0, 10, 45, 59): '3',
    (0, 10, 60, 74): '4', (0, 10, 75, 89): '5', (0, 10, 90, 104): '6', (0, 10, 105, 119): '7',
    (0, 10, 120, 134): '8', (0, 10, 135, 149): '9', (0, 10, 150, 164): 'f', (0, 10, 165, 179): 'g',
    (0, 10, 180, 194): 'h', (0, 10, 195, 209): 'i', (0, 10, 210, 224): 'j', (0, 10, 225, 239): 'k',
    (0, 10, 240, 254): 'l', (0, 10, 255, 269): 'n', (0, 10, 270, 284): 'o', (0, 10, 285, 299): 'p',
    (0, 10, 300, 314): 'q', (0, 10, 315, 329): 'r', (0, 10, 330, 344): 's', (0, 10, 345, 359): 't',

    (11, 25, 0, 14): 'u', (11, 25, 15, 29): 'v', (11, 25, 30, 44): 'w', (11, 25, 45, 59): 'x',
    (11, 25, 60, 74): 'y', (11, 25, 75, 89): '`', (11, 25, 90, 104): '~', (11, 25, 105, 119): '!',
    (11, 25, 120, 134): '@', (11, 25, 135, 149): '#', (11, 25, 150, 164): '$', (11, 25, 165, 179): '%',
    (11, 25, 180, 194): '^', (11, 25, 195, 209): '&', (11, 25, 210, 224): '*', (11, 25, 225, 239): '(',
    (11, 25, 240, 254): ')', (11, 25, 255, 269): '-', (11, 25, 270, 284): '_', (11, 25, 285, 299): '+',
    (11, 25, 300, 314): '=', (11, 25, 315, 329): '{', (11, 25, 330, 344): '}', (11, 25, 345, 359): '[',

    (26, 35, 0, 14): ']', (26, 35, 15, 29): '|', (26, 35, 30, 44): '\\', (26, 35, 45, 59): ';',
    (26, 35, 60, 74): ':', (26, 35, 75, 89): '"', (26, 35, 90, 104): "'", (26, 35, 105, 119): '/',
    (26, 35, 120, 134): '?', (26, 35, 135, 149): '\\', (26, 35, 150, 164): '‚', (26, 35, 165, 179): 'ƒ',
    (26, 35, 180, 194): '„', (26, 35, 195, 209): '…', (26, 35, 210, 224): '†', (26, 35, 225, 239): '‡',
    (26, 35, 240, 254): 'ˆ', (26, 35, 255, 269): '‰', (26, 35, 270, 284): 'Š', (26, 35, 285, 299): '‹',
    (26, 35, 300, 314): 'Œ', (26, 35, 315, 329): 'Ž', (26, 35, 330, 344): '‘', (26, 35, 345, 359): '’',

    (36, 55, 0, 14): '“', (36, 55, 15, 29): '”', (36, 55, 30, 44): '•', (36, 55, 45, 59): '–',
    (36, 55, 60, 74): '—', (36, 55, 75, 89): '˜', (36, 55, 90, 104): '™', (36, 55, 105, 119): 'š',
    (36, 55, 120, 134): '›', (36, 55, 135, 149): 'œ', (36, 55, 150, 164): 'ž', (36, 55, 165, 179): 'Ÿ',
    (36, 55, 180, 194): '¡', (36, 55, 195, 209): '¢', (36, 55, 210, 224): '£', (36, 55, 225, 239): '¤',
    (36, 55, 240, 254): '¥', (36, 55, 255, 269): '¦', (36, 55, 270, 284): '§', (36, 55, 285, 299): '¨',
    (36, 55, 300, 314): '©', (36, 55, 315, 329): 'ª', (36, 55, 330, 344): '«', (36, 55, 345, 359): '¬',

    (56, 75, 0, 14): '­', (56, 75, 15, 29): '®', (56, 75, 30, 44): '¯', (56, 75, 45, 59): '°',
    (56, 75, 60, 74): '±', (56, 75, 75, 89): '²', (56, 75, 90, 104): '³', (56, 75, 105, 119): '´',
    (56, 75, 120, 134): 'µ', (56, 75, 135, 149): '¶', (56, 75, 150, 164): '·', (56, 75, 165, 179): '¸',
    (56, 75, 180, 194): '¹', (56, 75, 195, 209): 'º', (56, 75, 210, 224): '»', (56, 75, 225, 239): '¼',
    (56, 75, 240, 254): '½', (56, 75, 255, 269): '¾', (56, 75, 270, 284): '¿', (56, 75, 285, 299): 'À',
    (56, 75, 300, 314): 'Á', (56, 75, 315, 329): 'Â', (56, 75, 330, 344): 'Ã', (56, 75, 345, 359): 'Ä',

    (76, 100, 0, 14): 'Å', (76, 100, 15, 29): 'Æ', (76, 100, 30, 44): 'Ç', (76, 100, 45, 59): 'È',
    (76, 100, 60, 74): 'É', (76, 100, 75, 89): 'Ê', (76, 100, 90, 104): 'Ë', (76, 100, 105, 119): 'Ì',
    (76, 100, 120, 134): 'Í', (76, 100, 135, 149): 'Î', (76, 100, 150, 164): 'Ï', (76, 100, 165, 179): 'Ð',
    (76, 100, 180, 194): 'Ñ', (76, 100, 195, 209): 'Ò', (76, 100, 210, 224): 'Ó', (76, 100, 225, 239): 'Ô',
    (76, 100, 240, 254): 'Õ', (76, 100, 255, 269): 'Ö', (76, 100, 270, 284): '×', (76, 100, 285, 299): 'Ø',
    (76, 100, 300, 314): 'Ù', (76, 100, 315, 329): 'Ú', (76, 100, 330, 344): 'Û', (76, 100, 345, 359): 'Ü',
    
    (101, 125, 0, 14): 'Ý', (101, 125, 15, 29): 'Þ', (101, 125, 30, 44): 'ß', (101, 125, 45, 59): 'à',
    (101, 125, 60, 74): 'á', (101, 125, 75, 89): 'â', (101, 125, 90, 104): 'ã', (101, 125, 105, 119): 'ä',
    (101, 125, 120, 134): 'å', (101, 125, 135, 149): 'æ', (101, 125, 150, 164): 'ç', (101, 125, 165, 179): 'è',
    (101, 125, 180, 194): 'é', (101, 125, 195, 209): 'ê', (101, 125, 210, 224): 'ë', (101, 125, 225, 239): 'ì',
    (101, 125, 240, 254): 'í', (101, 125, 255, 269): 'î', (101, 125, 270, 284): 'ï', (101, 125, 285, 299): 'ð',
    (101, 125, 300, 314): 'ñ', (101, 125, 315, 329): 'ò', (101, 125, 330, 344): 'ó', (101, 125, 345, 359): 'ô',
}

# GUI that allow the user to draw on canvas approximate their path, then send that data as instructions for the car

# latest - updated to incorporate basic functionality commands 

import tkinter as tk
import numpy as np
import cmath # for vector math

# Draw path and returns a phasor list
# Draw path and returns a phasor list
class PathDrawer:
    def __init__(self, master):
        self.master = master
        self.canvas = tk.Canvas(self.master, width=400, height=400, bg="white")
        self.canvas.pack()

        # Data array attributes ot the class
        self.data_points = []   # Draw datapoints from canvas drawing
        self.approx_points = [] # Approximated waypoints of the drawn path
        self.vectors_list = []  # Turning into lists of vectors
        self.phasors_list = []  # Angle for CW rotation as a list calculated from previous vector

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

    def send_data(self):
        print("Phasors:") # send data to serial on a set time
        for phasor in self.phasors_list:
            phasor_ascii_char = self.polar_to_ascii(phasor[0], phasor[1])
            
            # calculate amount of time that current path should take
            wait_time = phasor[1] / CW_VELOCITY + phasor[0] / FORWARD_VELOCITY
            # adding a slight buffer so that new instruction isn't sent
            # while executing current instruction robotpath in .c is blocking
            wait_time = wait_time * 1.05 + 0.2 

            ser.write(phasor_ascii_char.encode()) # send over serial to JDY40
            print(phasor)                         # print what character is sent
            
            time.sleep(wait_time)                 # pause program while we wait for phasors to be sent over

            
    def draw_square(self):
        self.clear_canvas()
        side_length = min(self.canvas.winfo_width(), self.canvas.winfo_height()) / 2
        center_x = self.canvas.winfo_width() / 2
        center_y = self.canvas.winfo_height() / 2
        top_left = (center_x - side_length / 2, center_y - side_length / 2)
        top_right = (center_x + side_length / 2, center_y - side_length / 2)
        bottom_right = (center_x + side_length / 2, center_y + side_length / 2)
        bottom_left = (center_x - side_length / 2, center_y + side_length / 2)
        self.data_points = [top_left, top_right, bottom_right, bottom_left, top_left]
        self.draw_lines()

    def polar_to_ascii(self, magnitude, argument):
    # Iterate through the dictionary to find the corresponding character
        for key, value in ascii_mapping.items():
            if key[0] <= magnitude <= key[1] and key[2] <= argument <= key[3]:
                return value
        
        # Return None if no matching character is found
        return None
            
    def draw_square(self):
        self.clear_canvas()
        side_length = min(self.canvas.winfo_width(), self.canvas.winfo_height()) / 2
        center_x = self.canvas.winfo_width() / 2
        center_y = self.canvas.winfo_height() / 2
        top_left = (center_x - side_length / 2, center_y - side_length / 2)
        top_right = (center_x + side_length / 2, center_y - side_length / 2)
        bottom_right = (center_x + side_length / 2, center_y + side_length / 2)
        bottom_left = (center_x - side_length / 2, center_y + side_length / 2)
        self.data_points = [top_left, top_right, bottom_right, bottom_left, top_left]
        self.draw_lines()

def on_option_select():
    selected = selected_option.get()
    result_label.config(text=f"Selected Option: {selected}")

    if selected == 'Draw':
        root = tk.Tk()
        app = PathDrawer(root)
        root.mainloop()
    elif selected == 'Joystick':
        print("Joystick Mode")
    else:
        print("Invalid Option")

root = tk.Tk()
root.title("Start by Manual Joystick Driving or Draw a Path")
root.geometry("400x100")
# Create a StringVar to hold the selected option
selected_option = tk.StringVar()
# Create the dropdown menu
options = ["Joystick", "Draw"]
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

# joystick_flag = 0 

# if selected_option.get() == 'Joystick':
#     joystick_flag = 1
#     print("Joystick Mode")
# elif selected_option.get() == 'Draw':
#     root = tk.Tk()
#     app = PathDrawer(root)
#     root.mainloop()

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

plt.show()

while True:
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
