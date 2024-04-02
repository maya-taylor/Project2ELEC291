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
cwm_flag = 'F'
ccwf_flag = 'H'
ccwm_flag = 'I'
ne_flag = 'J'
nw_flag = 'K'
se_flag = 'L'
sw_flag = 'M'
no_movmt_flag = 'N'


# Open the CSV file
with open(FILE, 'r') as file:
    # Create a CSV reader object
    reader = csv.reader(file)
    
    # Iterate through each row in the CSV file
    for row in reader:
        # Assuming one column in the CSV file
        data = row[0]
        
        # Check if the data contains any of the markers
        if marker_1 in data:
            # Process data of type 1
            # For example:
            print("Type 1 data:", data)
        elif marker_2 in data:
            # Process data of type 2
            # For example:
            print("Type 2 data:", data)
        elif marker_3 in data:
            # Process data of type 3
            # For example:
            print("Type 3 data:", data)
        else:
            # Data does not match any of the markers, skip or handle it as needed
            pass


'''

ideas:

    * calibrate a starting location/home base, could use joy stick pushbutto to decide this
    * keep track of location of the car relative to the home base (could incorporate whatever george has with this)
    * when metal is detected it will save the xy cord into a list or something, maybe a dict
    * can map the whole thing out into like a treasure map of where all the coins are hidden 

'''

