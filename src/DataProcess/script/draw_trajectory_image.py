import sys
import numpy as np
import scipy.io as sio
import matplotlib.pyplot as plt
import math

trajectory_data = sys.argv[1]

np.set_printoptions(suppress=True)
traj_mat = sio.loadmat(trajectory_data)
traj_matrix = np.array(traj_mat["traj_matrix"])
max_longitude = traj_matrix[0][1]
min_longitude = traj_matrix[0][1]
max_latitude = traj_matrix[0][2]
min_latitude = traj_matrix[0][2]
for i in range(0, traj_matrix.shape[0]):
    if traj_matrix[i][1] > max_longitude:
        max_longitude = traj_matrix[i][1]
    if traj_matrix[i][1] < min_longitude:
        min_longitude = traj_matrix[i][1]
    if traj_matrix[i][2] > max_latitude:
        max_latitude = traj_matrix[i][2]
    if traj_matrix[i][2] < min_latitude:
        min_latitude = traj_matrix[i][2]

traj_x = traj_matrix[:, 1:2]
traj_y = traj_matrix[:, 2:3]


traj_image = plt.subplot(111)
plt.plot(traj_x, traj_y)
xmajorLocator = plt.MultipleLocator((max_longitude-min_longitude)/10)
ymajorLocator = plt.MultipleLocator((max_latitude-min_latitude)/10)
if abs(max_longitude) < 180.1:
    xmajorFormatter = plt.FormatStrFormatter('%1.8f')
    ymajorFormatter = plt.FormatStrFormatter('%1.8f')
else:
    xmajorFormatter = plt.FormatStrFormatter('%1.1f')
    ymajorFormatter = plt.FormatStrFormatter('%1.1f')
traj_image.xaxis.set_major_locator(xmajorLocator)
traj_image.xaxis.set_major_formatter(xmajorFormatter)
traj_image.yaxis.set_major_locator(ymajorLocator)
traj_image.yaxis.set_major_formatter(ymajorFormatter)
plt.show()

