import serial 
import time
import re
import matplotlib.pyplot as plt
import pygame 
import csv
import random


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

#flags for strength
level_1 = 'a'
level_2 = 'b'
level_3 = 'c'
level_4 = 'd'
level_5 = 'e'
null_detect = 'z'

def dummy_data_gen(rows):
    data = []
    for _ in range(rows):
        dummy_x = random.choice(['ff_flag', 'fm_flag','fs_flag',
                                 'bf_flag','bm_flag','bs_flag',
                                 'cwf_flag','cwm_flag','ccwf_flag','ccwm_flag',
                                 'ne_flag','nw_flag','se_flag','sw_flag',
                                 'no_movmt_flag'
                                 ])
        dummy_y = random.choice(['ff_flag', 'fm_flag','fs_flag',
                                 'bf_flag','bm_flag','bs_flag',
                                 'cwf_flag','cwm_flag','ccwf_flag','ccwm_flag',
                                 'ne_flag','nw_flag','se_flag','sw_flag',
                                 'no_movmt_flag'
                                 ])
        dummy_strength = random.choice(['level_1', 'level_2', 'level_3',
                                         'level_4', 'level_5', 'null_detect'])

        data.append([dummy_x, dummy_y, dummy_strength])

        return data
    
def dummy_file_gen(filename, data):
    with open(filename, 'w', newline='') as file:
        
def process_csv(filename):
    # Open the CSV file
    with open(filename, 'r') as file:
        # Create a CSV reader object
        reader = csv.reader(file)
        coord_map = [] #dynamic initialization of x,y
        strength_map = [] #dynamic initialization of strength value
        xpos = 0 #variables that are used as buffers 
        ypos = 0
        metal_strength = 0
        # Iterate through each row in the CSV file
        for row in reader:
            # Assuming one column in the CSV file
            data = row[0]
            
            # Check if the data contains location flags and process accordingly
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
            else: #default case is no movement so we add nothing new
                xpos += 0
                ypos += 0
            
            coord_map.append([xpos, ypos])

            if level_1 in data:
                metal_strength = 225
            elif level_2 in data:
                metal_strength = 300
            elif level_3 in data:
                metal_strength = 400
            elif level_4 in data:
                metal_strength = 500
            elif level_5 in data:
                metal_strength = 700
            elif null_detect in data:
                metal_strength = 0
            else:
                metal_strength += 0 #retain previous state as a default
            
            strength_map.append(metal_strength)

    return
        

#drawing the map below 


'''

ideas:

    * calibrate a starting location/home base, could use joy stick pushbutto to decide this
    * keep track of location of the car relative to the home base (could incorporate whatever george has with this)
    * when metal is detected it will save the xy cord into a list or something, maybe a dict
    * can map the whole thing out into like a treasure map of where all the coins are hidden 

'''

