import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Define the rectangle and parameters using top-left and bottom-right corners
x1, y1 = 0, 0  # Top-left corner
x2, y2 = 6, 4  # Bottom-right corner

L = x2 - x1  # Length (width)
H = y2 - y1  # Height
P = 2 * (L + H)  # Perimeter
N = 100  # Number of seconds for a full loop

# Compute distance per second
d = P / N  # Distance traveled per second

# Parametric function to get position at time t
def get_position(t):
    t_mod = t % P  # Wrap time around the perimeter length
    if 0 <= t_mod < L:  # Bottom edge
        return (x1 + t_mod, y2)
    elif L <= t_mod < L + H:  # Right edge
        return (x2, y2 - (t_mod - L))
    elif L + H <= t_mod < 2 * L + H:  # Top edge
        return (x2 - (t_mod - (L + H)), y1)
    else:  # Left edge
        return (x1, y1 + (t_mod - (2 * L + H)))

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
    x, y = get_position(frame * d)
    point.set_data([x], [y])  # Wrap x and y in lists
    return point,

# Create the animation
ani = FuncAnimation(fig, update, frames=N, init_func=init, blit=True, interval=1000)

# Show the animation
plt.show()
