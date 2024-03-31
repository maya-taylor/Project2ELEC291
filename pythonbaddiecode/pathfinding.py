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

        self.clear_button = tk.Button(self.master, text="Clear", command=self.clear_canvas)
        self.clear_button.pack()

        self.send_button = tk.Button(self.master, text="Send", command=self.send_data)
        self.send_button.pack()

    def on_click(self, event):
        self.data_points.append((event.x, event.y))

    def on_drag(self, event):
        self.data_points.append((event.x, event.y))
        self.draw_lines()

    def draw_lines(self):
        self.canvas.delete("line")
        if len(self.data_points) >= 2:
            approx_points = self.approximate_path(self.data_points, 5)
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

    def clear_canvas(self):
        self.canvas.delete("all")
        self.data_points = []

    def send_data(self):
        approx_points = self.approximate_path(self.data_points, 1)
        print("Approximated points:")
        for point in approx_points:
            print(point)

root = tk.Tk()
app = PathDrawer(root)
root.mainloop()
