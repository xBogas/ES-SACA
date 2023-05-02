import numpy as np
import matplotlib.pyplot as plt

radius = 10
center_x = 1
center_y = 2

num_points = 1000
noise = 0.7

theta = np.linspace(0, 2*np.pi, num_points)

x = center_x + radius * np.cos(theta) + np.random.normal(0, noise, num_points) #+ np.random.uniform(-1, 1, num_points)
y = center_y + radius * np.sin(theta) + np.random.normal(0, noise, num_points) #+ np.random.uniform(-1, 1, num_points)

plt.scatter(x, y)
plt.scatter(center_x,center_y)
plt.axis("equal")
plt.show()

File = open("Data.txt", "w")
for	i in range(num_points):
	File.write(f"{x[i]};{y[i]}\n")

File.close()