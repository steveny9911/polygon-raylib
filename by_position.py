import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Define the rectangle and parameters using top-left and bottom-right corners
x1, y1 = 0, 0  # Top-left corner
x2, y2 = 6, 4  # Bottom-right corner

L = x2 - x1  # Length (width)
H = y2 - y1  # Height
P = 2 * (L + H)  # Perimeter
step_size = 0.1  # Distance moved per frame

# Initialize the starting position at the bottom-left corner
current_x, current_y = x1, y2  # Starting at the bottom-left
direction = "right"  # Start by moving along the bottom edge

# Function to get the next position based on the current position
def get_next_position(x, y, direction):
    if direction == "right":  # Move along the bottom edge
        if x + step_size <= x2:
            return x + step_size, y, "right"
        else:
            return x2, y, "up"  # Switch to moving up along the right edge
    elif direction == "up":  # Move up along the right edge
        if y - step_size >= y1:
            return x, y - step_size, "up"
        else:
            return x, y1, "left"  # Switch to moving left along the top edge
    elif direction == "left":  # Move along the top edge
        if x - step_size >= x1:
            return x - step_size, y, "left"
        else:
            return x1, y, "down"  # Switch to moving down along the left edge
    elif direction == "down":  # Move down along the left edge
        if y + step_size <= y2:
            return x, y + step_size, "down"
        else:
            return x, y2, "right"  # Switch to moving right along the bottom edge

# Set up the plot
fig, ax = plt.subplots()
ax.set_xlim(x1 - 1, x2 + 1)
ax.set_ylim(y1 - 1, y2 + 1)
rect = plt.Rectangle((x1, y1), L, H, fill=False, edgecolor='blue')
ax.add_patch(rect)

point, = ax.plot([], [], 'ro')  # Point to move

# Initialize the animation
def init():
    point.set_data([], [])
    return point,

# Update the animation frame-by-frame
def update(frame):
    global current_x, current_y, direction
    current_x, current_y, direction = get_next_position(current_x, current_y, direction)
    point.set_data([current_x], [current_y])  # Update point position
    return point,

# Create the animation
ani = FuncAnimation(fig, update, frames=np.arange(0, P, step_size), init_func=init, blit=True, interval=50)

# Show the animation
plt.show()
