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

def on_option_select():
    selected = selected_option.get()
    result_label.config(text=f"Selected Option: {selected}")

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

joystick_flag = 0

if selected_option.get() == 'Joystick':
    joystick_flag = 1
    print("Joystick Mode")

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
