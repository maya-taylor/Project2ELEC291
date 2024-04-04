import serial
import time
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import tkinter as tk
from tkinter import simpledialog

# voice recognition modules
import pyaudio
import speech_recognition as sr
import threading
import os
from PIL import Image, ImageTk

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

# Coordinates to map to for A to N going from EFM8 to STM
coordinates = {65: (0, 50),     #A to N
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
               78: (-20, -45),  #N
               90: (0, 0)}      #Z, AKA no movement

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

# Create an interface and drop down for options on how to control the car
def on_option_select():
    selected = selected_option.get()
    result_label.config(text=f"Selected Option: {selected}")
    # Select mode of operation
    if selected == 'Draw':
       print("Draw Mode")
    elif selected == 'Joystick':
        print("Joystick Mode")
    elif selected == 'Voice Control(GUI)':
        print("Voice Control Mode")
    elif selected == 'Voice Control(Terminal)':
        print("Voice Control Mode")
    elif selected == 'Keyboard':
        print("Keyboard Mode")
    else:
        print("Invalid Option")

root = tk.Tk()
root.title("Start by Manual Joystick Driving or Draw a Path")
root.geometry("400x100")
# Create a StringVar to hold the selected option
selected_option = tk.StringVar()
# Create the dropdown menu
options = ["Joystick", "Draw", "Voice Control(GUI)", "Voice Control(Terminal)", "Keyboard"] # adding a voice control option
dropdown = tk.OptionMenu(root, selected_option, *options)
dropdown.pack(pady=10)
# Button to display the selected option
show_button = tk.Button(root, text="Show Selection", command=on_option_select)
show_button.pack()
# Label to display the selected option
result_label = tk.Label(root, text="")
result_label.pack()
root.mainloop()


joystick_flag = 0 
if selected_option.get() == 'Joystick':
    joystick_flag = 1
    print("Joystick Mode")
elif selected_option.get() == 'Draw':
    joystick_flag = 0
elif selected_option.get() == 'Voice Control(GUI)':
    joystick_flag = 2
elif selected_option.get() == 'Voice Control(Terminal)':
    joystick_flag = 3
elif selected_option.get() == 'Keyboard':
    joystick_flag = 4
    
# Code to draw a path on a canvas
class PathDrawer:
    def __init__(self, master):
        self.master = master
        self.canvas = tk.Canvas(self.master, width=400, height=400, bg="white")                             # canvas value previously 400, changed down to 120
        self.canvas.pack()

        # Data array attributes ot the class
        self.data_points = []   # Draw datapoints from canvas drawing
        self.approx_points = [] # Approximated waypoints of the drawn path
        self.vectors_list = []  # Turning into lists of vectors
        self.phasors_list = []  # Angle for CW rotation as a list calculated from previous vector

        master.protocol("WM_DELETE_WINDOW", self.on_closing) 

        self.canvas.bind("<Button-1>", self.on_click) 
        self.canvas.bind("<B1-Motion>", self.on_drag) 

        self.clear_button = tk.Button(self.master, text="Clear", command=self.clear_canvas)                 # call clear canvas command on press
        self.clear_button.pack(side=tk.LEFT)

        self.send_button = tk.Button(self.master, text="Send", command=self.send_data)                      # call send data command on press
        self.send_button.pack(side=tk.LEFT)

        # Draw Square
        self.square_button = tk.Button(self.master, text="Draw squre", command=self.draw_square)
        self.square_button.pack(side=tk.LEFT)

        self.approx_button = tk.Button(self.master, text="Approximate", command=self.approximate_and_draw)  # call approximate_and_draw command on press
        self.approx_button.pack(side=tk.LEFT)
        
        # Input value which will be used to determine the resolution of path approximation
        self.num_segments_entry = tk.Entry(self.master)
        self.num_segments_entry.pack(side=tk.LEFT)
        self.num_segments_entry.insert(tk.END, "5")
    # Click to begin drawing
    def on_click(self, event):
        self.data_points.append((event.x, event.y))
    # Drag to draw line and find endpoint
    def on_drag(self, event):
        self.data_points.append((event.x, event.y))
        self.draw_lines()
    # Join the points together
    def draw_lines(self):
        self.canvas.delete("line")
        if len(self.data_points) >= 2:
            for i in range(len(self.data_points) - 1):
                x0, y0 = self.data_points[i]
                x1, y1 = self.data_points[i + 1]
                self.canvas.create_line(x0, y0, x1, y1, tags="line")
    # Clear all points
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

        self.canvas.delete("line")          # delete previous path on the canvas on press
        self.approx_points = approx_points  #save as attribute to self

        # plot new approximated path
        for i in range(len(approx_points) - 1):
            x0, y0 = approx_points[i]
            x1, y1 = approx_points[i + 1]
            self.canvas.create_line(x0, y0, x1, y1, tags="line")

        # convert values to vectors and phasors
        self.convert_to_vectors()
        self.convert_to_phasors()

    # Convert points to vectors
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

    # Convert points to phasors
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

    # Send data to the JDY-40
    def send_data(self):
        print("Phasors:")               # send data to serial on a set time  
        ser.write("..\r\n".encode())    # send over serial to JDY40
        time.sleep(0.1)
        ser.write("..\r\n".encode())    # send over serial to JDY40
        time.sleep(0.1)
        for phasor in self.phasors_list:
            phasor_ascii_char = self.polar_to_ascii(phasor[0], phasor[1])
            
            # calculate amount of time that current path should take
            if (phasor[1] <= 180):
                wait_time = phasor[1] / CW_VELOCITY + phasor[0] / FORWARD_VELOCITY
            else:
                wait_time = abs(360-phasor[1]) / CCW_VELOCITY + phasor[0] / FORWARD_VELOCITY
            # adding a slight buffer so that new instruction isn't sent
            # while executing current instruction robotpath in .c is blocking
            wait_time = wait_time * 1.2 + 0.25
            # wait_time = 1

            ser.write(f"{phasor_ascii_char*2}\r\n".encode())                # send over serial to JDY40
            time.sleep(0.01)
            ser.write(f"{phasor_ascii_char*2}\r\n".encode())
            print(f"Sent string = {phasor_ascii_char*2}\r\n".encode())      # check the string sent
            print(f"{phasor},{phasor_ascii_char}, wait = {wait_time}")      # print what character is sent
            
            time.sleep(wait_time)   
        time.sleep(0.1)                 # pause program while we wait for phasors to be sent over
        ser.write(",,\r\n".encode()) 
        time.sleep(0.1)
        ser.write(",,\r\n".encode())    # send over serial to JDY40
        print("Finished!")

    # Draw a suqare on the canvas
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

    # Convert polar coordinates to the corresponding ASCII character sent
    def polar_to_ascii(self, magnitude, argument):
    # Iterate through the dictionary to find the corresponding character
        for key, value in ascii_mapping.items():
            if key[0] <= magnitude <= key[1] and key[2] <= argument <= key[3]:
                return value
        
        # Return None if no matching character is found
        return None

    def on_closing(self):
        self.master.destroy()               # Destroy the Tkinter window

# Function: Python class with functions for voice control
# Creates a master object with a text attribute, icon attribute and functions
class VoiceControl:
    def __init__(self, master):
        self.master = master
        self.is_pressed = True              # boolean to track if button is pressed or not presesd

        self.text = ''
        self.recognizer = sr.Recognizer()   # speech recognition object

        # Check the value of joystick_flag to either open up voice control in GUI or terminal
        if (joystick_flag == 2):
            self.GUI_on = True              # GUI 
        else :
            self.GUI_on = False             # Terminal

        # Get the directory of the script so that this works on person's computer (assuming they download the files)
        script_dir = os.path.dirname(os.path.abspath(__file__))
        mic_on_path = os.path.join(script_dir, "mic_on.png")
        mic_off_path = os.path.join(script_dir, "mic_off.png")

        # Create image attributes using Pillow and resize using Lanczos resampling
        # Lanczos has the best downscaling quality but has poor performance
        # This is only being done once so this is acceptable

        self.icon_off = Image.open(mic_off_path)
        self.icon_on = Image.open(mic_on_path)
        self.icon_off = self.icon_off.resize((80, 80), Image.LANCZOS) # .resize() takes size, tuple, and a resampling method
        self.icon_on = self.icon_on.resize((80, 80), Image.LANCZOS)
        self.icon_photo_off = ImageTk.PhotoImage(self.icon_off)
        self.icon_photo_on = ImageTk.PhotoImage(self.icon_on)
        
        if (self.GUI_on is True):
            # Create voice recording button
            self.record_button = tk.Button(
                self.master, 
                image=self.icon_photo_off, 
                command=self.on_mic_press_GUI,  # on button perssed
                bd = 0,                         # Remove button border
                highlightthickness=0)           # Remove Button Shadow

            self.record_button.pack()
        
            # Create text label
            self.text_label = tk.Label(self.master, text="")
            self.text_label.pack()

            self.text_label.config(text="Muted")
        # terimnal code, only creates a button GUI
        else:
            print("Terminal Activated")
            self.record_button_terminal = tk.Button(
                self.master,
                text="Send Voice Command",
                command = self.on_mic_press_terminal
            )

            self.record_button_terminal.pack()

        # define behaviour for when the window is closed
        master.protocol("WM_DELETE_WINDOW", self.on_closing)

    def on_mic_press_terminal(self):
        # on press, it will then record speech then send over terminal
        with sr.Microphone() as source:
            print("Listening...")
            audio_data = self.recognizer.listen(source, timeout=5, phrase_time_limit=5)
            print("Reconizing...")

            try:
                # Recognize speech using Google Speech Recognition
                self.text = self.recognizer.recognize_google(audio_data)
                print(f"You said: {self.text}")
            except sr.UnknownValueError:
                    print("Speech Recognition could not understand audio")
            except sr.RequestError as e:
                    print(f"Could not request results from Google Speech Recognition service; {e}")     

            self.send_voice_data() # send the data
            print("Data Sent!")
            print("")
    # Mic press GUI
    def on_mic_press_GUI(self):
        if self.is_pressed:
            self.record_button.config(image=self.icon_photo_on)
  
            with sr.Microphone() as source:
                self.text_label.config(text="Listening...")
                self.master.update_idletasks()

                audio_data = self.recognizer.listen(source, timeout=5, phrase_time_limit=5)
                self.text_label.config(text="Reconizing...")
                self.master.update_idletasks()
                try:
                    # Recognize speech using Google Speech Recognition
                    self.text = self.recognizer.recognize_google(audio_data).lower()
                    self.text_label.config(text=f"You said: {self.text}")
                    self.master.update_idletasks()
                    # Wait for the recognition thread to finish
                    time.sleep(1.2)
                    self.send_voice_data()
                    self.text_label.config(text = "Command Sent!")
                except sr.UnknownValueError:
                    self.text_label.config(text="Speech Recognition could not understand audio")
                    self.master.update_idletasks()
                except sr.RequestError as e:
                    self.text_label.config(text=f"Could not request results from Google Speech Recognition service; {e}")
                    self.master.update_idletasks()

        else:
            self.record_button.config(image=self.icon_photo_off)
            self.text_label.config(text="Muted")

        self.is_pressed = not self.is_pressed

    # Detectable Phrases 
    def send_voice_data(self):
        # move forward
        voice_char = ' '
        if "straight" in self.text:
            voice_char = '&'
        if "forward" in self.text:
            voice_char = '&'

        # go back
        if "backward" in self.text:
            voice_char = '{'
        if "back" in self.text:
            voice_char = '{'

        # turns
        if "left" in self.text:
            voice_char = '|'
        if "right" in self.text:
            voice_char = '-'

        # move in a square
        if "Square" in self.text:
            voice_char = '/'
        if "square" in self.text:
            voice_char = '/'
        if "where" in self.text:
            voice_char = '/'
        
        # move in a circle
        if "Circle" in self.text:
            voice_char = "'"

        # move in a figure 8
        if "Figure" in self.text:
            voice_char = '"'
        if "igure" in self.text:
            voice_char = '"'
        if "8" in self.text:
            voice_char = '"'
        if "ight" in self.text:
            voice_char = '"'

        # print the char sent for debugging
        print("char_sent = ",voice_char)

        if (voice_char=='&' or voice_char=='{' or voice_char=='|' or voice_char=='-'):
            print("sent . and ,")
            ser.write("..\r\n".encode())            # send over serial to JDY40
            time.sleep(0.01)
            ser.write("..\r\n".encode())            # send over serial to JDY40
            time.sleep(0.1)
        if (voice_char == '/' or voice_char == "'" or voice_char == '"'):
            ser.write(",,\r\n".encode())            # send over serial to JDY40
            time.sleep(0.01)
            ser.write(",,\r\n".encode())            # send over serial to JDY40
            time.sleep(0.1)

        ser.write(f"{voice_char*2}\r\n".encode())   # send over serial to JDY40
        time.sleep(0.01)
        ser.write(f"{voice_char*2}\r\n".encode())   # send over serial to JDY40
        time.sleep(5)

        if (voice_char=='&' or voice_char=='{' or voice_char=='|' or voice_char=='-'):
            ser.write(",,\r\n".encode())            # send over serial to JDY40
            time.sleep(0.01)
            ser.write(",,\r\n".encode())            # send over serial to JDY40
            time.sleep(0.01)

    def on_closing(self):
        self.master.destroy()

class KeybordControl:
    def __init__(self, root):
        self.root = root
        self.root.title("Keybord Control")
        self.root.configure(bg="#222222")  # Set background color to dark gray for a darkmode keyboard
        
        # Define the keys on the virtual keyboard
        self.keys = [
            ['Q', 'W', 'E'],
            ['A', 'S', 'D'],
            ['Z', ' ', 'C']
        ]

        self.key_to_command = {
        ('w'): ('B'),            # N
        ('a'): ('J'),            # W 
        ('s'): ('E'),            # S 
        ('d'): ('H'),            # E
        ('q'): ('L'),            # NW
        ('e'): ('N'),            # NE
        ('z'):('M'),             # SW
        ('c'):('K')              # SE
        }

        self.key = ''
        self.time_elapsed = 0

        # Create a dictionary to store the labels
        self.labels = {}

        # Create and place the keys on the virtual keyboard
        # Places each level in a grid configuration 
        # https://www.pythonguis.com/tutorials/create-ui-with-tkinter-grid-layout-manager/
        for i, row in enumerate(self.keys):
            for j, key in enumerate(row):
                label = tk.Label(root, text=key, width=10, height=5, relief="flat", font=("Helvetica", 16), bg="#555555", fg="black")
                label.grid(row=i, column=j, padx=5, pady=5)
                self.labels[key] = label  # Store the label in the dictionary

        # Bind key press and release events to the root window
        # The frame widget in Tkinter has allows you to bind an event object to a callable
        # Character of keypress is obtained using `.char` attribute on the event
        root.bind("<KeyPress>", self.on_key_press)
        root.bind("<KeyRelease>", self.on_key_release)
        
        time.sleep(0.1)

    def on_key_press(self, event):
        # Handle key press event
        self.key = event.char.upper()
        if self.key in self.labels:
            self.labels[self.key].config(bg="#e3e3e3", fg="black")  # Change label color to indicate key press
        
        if self.key.lower() in self.key_to_command:
            self.key = self.key_to_command[self.key.lower()]
            self.send_letter()       
    
    def on_key_release(self, event):
        # Handle key release event
        self.time_elapsed = time.time() # set current time
        self.key = event.char.upper()
        if self.key in self.labels:
            self.labels[self.key].config(bg="#555555", fg="black")  # Restore label color to default

        self.key = 'Z' # This makes it so that the car stops on keyboard release
        self.send_letter()
        # print("Stop!!")

    def send_letter(self):
        # ser.write(f"{self.key*2}\r\n".encode()) 
        print(self.key+"\r\n")
    
# Joystick Flag

# If 0, run path drawer
if (joystick_flag == 0):
    root = tk.Tk()
    drawer = PathDrawer(root)   
    # start main tkinter loop
    root.mainloop()    

    joystick_flag = 1

# If 2 or 3, run voice commands
if (joystick_flag == 2 or joystick_flag == 3):
    root = tk.Tk()
    voiceController = VoiceControl(root)
    root.mainloop()

    joystick_flag = 1

# If 4, use keyboard commands
if (joystick_flag == 4):
    ser.write(f">>\n\n".encode())            # Tell the robot we are beginning keyboard control
    time.sleep(0.01)
    ser.write(f">>\n\n".encode())           
    time.sleep(0.1)
    root = tk.Tk()
    voiceController = KeybordControl(root)
    root.mainloop()

    # Tell the robot we are returning control from keyboard
    ser.write(f"<<\n\n".encode())
    time.sleep(0.01)
    ser.write(f"<<\n\n".encode())
    time.sleep(0.1)

    joystick_flag = 1 # set flag to 1 so that joystick mode will open


# Obtain coordinates from letter command
def get_coordinates(letter):
    return coordinates.get(letter, (0, 0))

# Map the letter received to a metal detection level
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
    
# Encode polar values in ascii
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
    plt.clf()                                           # Clear the previous plot
    plt.plot(xpos_data, ypos_data, 'o-', color='blue')  # Plot joystick position
    plt.xlim(-1, 1)                                     # Set x-axis limit
    plt.ylim(-1, 1)                                     # Set y-axis limit
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
    plt.pause(0.001)


plt.ion()  # Turn on interactive mode for Matplotlib
# Create a figure
fig = plt.figure()

# Function to handle figure close event
def on_close(event):
    plt.close()     # Close the plot window immediately
    ser.close()     # Close the serial port
    exit()          # Exit the program

# Attach the close event handler to the figure
fig.canvas.mpl_connect('close_event', on_close)

if(joystick_flag == 1):
    plt.show()

while (joystick_flag==1):
    try:
        # Read data from serial port
        strin = ser.readline()
        ser.reset_input_buffer()
        # print(strin)
        if strin:
            letter = strin[0]
            print(letter)
            # 41 to 78 corresponds to ASCII A to N, 90 is 'Z'
            if (letter >= 41 and letter <= 78) or letter == 90:   
                #print(letter)
                xpos, ypos = get_coordinates(letter)
                #print("Joystick positions: %d, %d" % (xpos, ypos))
                update_joystick_position(xpos, ypos)
                #ser.write(b'o\r\n')

            # 97 to 101 corresponds to ASCII a to e, 122 is 'z'
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
