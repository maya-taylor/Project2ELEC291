# GUI that allow the user to draw on canvas approximate their path, then send that data as instructions for the car

# latest - updated to incorporate basic functionality commands 

import tkinter as tk
import numpy as np
import cmath # for vector math


class PathDrawer:
    def __init__(self, master):
        self.master = master
        self.canvas = tk.Canvas(self.master, width=400, height=400, bg="white")
        self.canvas.pack()
        self.data_points = []
        self.approx_points = []
        self.vectors_list = []
        self.phasors_list = []

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

            self.phasors_list.append([vector[0], phase])
            prev_vector = vector

    def send_data(self):
        approx_points = self.approximate_path(self.data_points, 1) # keep at 1, 2 generates too many datapoints
        print("Approximated points:")
        for point in self.approx_points:
            print(point)

        print("Vectors:")
        for vector in self.vectors_list:
            print(vector)
        print("Phasors:")
        for phasor in self.phasors_list:
            print(phasor) 

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


root = tk.Tk()
app = PathDrawer(root)
root.mainloop()
