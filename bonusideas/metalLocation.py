import serial 
import time
import re
import matplotlib.pyplot as plt
import pygame 
import csv



'''
purpose: create a metalography chart in cartesian coordinates
info   : loci with higher metal density (a.k.a., frequency difference is bigger) will appear more 
         red, whilst loci with lower metal density will appear more purple 

'''


FILE = 'filename.csv' #edit!!!
# Define the ASCII characters marking the three specific kinds of data

#flags for x, y loci
ff_flag = 'A' #forward fast
fm_flag = 'B'
fs_flag = 'C'
bf_flag = 'D'
bm_flag = 'E'
bs_flag = 'F'
cwf_flag = 'G'
cwm_flag = 'H'
ccwf_flag = 'I'
ccwm_flag = 'J'
ne_flag = 'K'
nw_flag = 'L'
se_flag = 'M'
sw_flag = 'N'
no_movmt_flag = 'Z'


# Open the CSV file
with open(FILE, 'r') as file:
    # Create a CSV reader object
    reader = csv.reader(file)
    coord_map = [] #dynamic initialization of x,y
    strength_map = [] 
    xpos = 0 #variables that are used as buffers 
    ypos = 0
    metal_strength = 0
    # Iterate through each row in the CSV file
    for row in reader:
        # Assuming one column in the CSV file
        data = row[0]
        
        # Check if the data contains location flags:
        if ff_flag in data:
            xpos += 0
            ypos += 50
        elif fm_flag in data:
            xpos += 0
            ypos += 30
        elif fs_flag in data:
            xpos += 0
            ypos += 20
        elif bf_flag in data:
            xpos += 0
            ypos += -50
        elif bm_flag in data:
            xpos += 0
            ypos += -30
        elif bs_flag in data:
            xpos += 0
            ypos += -10
        elif cwf_flag in data:
            xpos += 50
            ypos += 0
        elif cwm_flag in data:
            xpos += 30
            ypos += 0
        elif ccwf_flag in data:
            xpos += -50
            ypos += 0
        elif ccwm_flag in data:
            xpos += -30
            ypos += 0
        elif ne_flag in data:
            xpos += 20
            ypos += 45
        elif nw_flag in data:
            xpos += -20
            ypos += 45
        elif se_flag in data:
            xpos += 20
            ypos += -45
        elif sw_flag in data:
            xpos += -20
            ypos += -45
        elif no_movmt_flag in data:
            xpos += 0
            ypos += 0
        else:
            # Data does not match any of the markers, skip or handle it as needed
            pass
        coord_map.append([xpos, ypos])




'''

ideas:

    * calibrate a starting location/home base, could use joy stick pushbutto to decide this
    * keep track of location of the car relative to the home base (could incorporate whatever george has with this)
    * when metal is detected it will save the xy cord into a list or something, maybe a dict
    * can map the whole thing out into like a treasure map of where all the coins are hidden 

'''

