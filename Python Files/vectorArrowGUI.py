import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Create a figure and axis
fig, ax = plt.subplots()

# Create a vector
vector = ax.quiver(0, 0, 1, 1, scale=1, scale_units='xy', angles='xy')

# Function to update the vector's direction
def update_quiver(num, data, vector):
    vector.set_offsets([0, 0])
    vector.set_UVC(data[num, 0], data[num, 1])
    return vector,

# Generate some data (x, y) for the vector direction
data = np.random.rand(100, 2) * 2 - 1  # Random direction data

# Create the animation
ani = FuncAnimation(fig, update_quiver, frames=len(data), fargs=(data, vector), interval=50, blit=True)

# Show the plot
plt.xlim(-1.5, 1.5)
plt.ylim(-1.5, 1.5)
plt.show()
