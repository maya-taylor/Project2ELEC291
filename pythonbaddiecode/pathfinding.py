# GUI that allow the user to draw on canvas approximate their path, then send that data as instructions for the car

# latest - updated to incorporate basic functionality commands 

import tkinter as tk
import numpy as np

class PathDrawer:
    def __init__(self, master):
        self.master = master
        self.canvas = tk.Canvas(self.master, width=400, height=400, bg="white")
        self.canvas.pack()
        self.data_points = []

        self.canvas.bind("<Button-1>", self.on_click)
        self.canvas.bind("<B1-Motion>", self.on_drag)

        self.clear_button = tk.Button(self.master, text="Clear", command=self.clear_canvas) # call clear canvas command on press
        self.clear_button.pack(side=tk.LEFT)

        self.send_button = tk.Button(self.master, text="Send", command=self.send_data)   # call send data command on press
        self.send_button.pack(side=tk.LEFT)

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
        num_segments = int(self.num_segments_entry.get())
        if len(self.data_points) >= 2 and num_segments > 0:
            approx_points = self.approximate_path(self.data_points, num_segments)
            self.canvas.delete("line")
            for i in range(len(approx_points) - 1):
                x0, y0 = approx_points[i]
                x1, y1 = approx_points[i + 1]
                self.canvas.create_line(x0, y0, x1, y1, tags="line")

    def approximate_path(self, points, num_segments):
        approx_points = []
        for i in range(num_segments):
            start_idx = int(i * len(points) / num_segments)
            end_idx = int((i + 1) * len(points) / num_segments)
            segment_points = points[start_idx:end_idx+1]
            if len(segment_points) > 1:
                start_point = segment_points[0]
                end_point = segment_points[-1]
                approx_points.extend(self.interpolate_line(start_point, end_point, 10))
        approx_points.append(points[-1])
        return np.array(approx_points)

    def interpolate_line(self, start_point, end_point, num_points):
        x0, y0 = start_point
        x1, y1 = end_point
        x_step = (x1 - x0) / num_points
        y_step = (y1 - y0) / num_points
        return [(int(x0 + i * x_step), int(y0 + i * y_step)) for i in range(num_points)]
    
    def send_data(self):
        approx_points = self.approximate_path(self.data_points, 1) # keep at 1, 2 generates too many datapoints
        print("Approximated points:")
        for point in approx_points:
            print(point)

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
    
    def draw_i(self):
        self.clear_canvas()
        center_x = self.canvas.winfo_width() / 2
        center_y = self.canvas.winfo_height() / 2
        top = (center_x, center_y - 50)
        bottom = (center_x, center_y + 50)
        self.data_points = [top, bottom]
        self.draw_lines()
    
    def draw_figure_8(self):
        self.clear_canvas()
        center_x = self.canvas.winfo_width() / 2
        center_y = self.canvas.winfo_height() / 2
        radius = min(self.canvas.winfo_width(), self.canvas.winfo_height()) / 3
        top_circle = [(int(center_x + radius * np.cos(theta)), int(center_y - radius * np.sin(theta))) for theta in np.linspace(0, np.pi, 50)]
        bottom_circle = [(int(center_x + radius * np.cos(theta)), int(center_y + radius * np.sin(theta))) for theta in np.linspace(np.pi, 2 * np.pi, 50)]
        self.data_points = top_circle + bottom_circle
        self.draw_lines()


root = tk.Tk()
app = PathDrawer(root)
root.mainloop()
