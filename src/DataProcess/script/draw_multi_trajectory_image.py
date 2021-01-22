
import os
import sys
import numpy as np
import scipy.io as sio
import matplotlib.pyplot as plt
import math

folder_path = sys.argv[1]

#get the trajectory data files path in the folder
traj_file = []
file_list = os.listdir(folder_path)
for i in range(0, len(file_list)):
    com_path = os.path.join(folder_path, file_list[i])
    if os.path.isfile(com_path):
        if com_path[-8:] == 'TRAJ.mat':
            traj_file.append(com_path)

max_longitude = -180.0
min_longitude = 180.0
max_latitude = -90.0
min_latitude = 90.0

#plot the file data
traj_image = plt.subplot(111)
for i in range(0, len(traj_file)):
    traj_mat = sio.loadmat(traj_file[i])
    traj_matrix = np.array(traj_mat["traj_matrix"])
    for j in range(0, traj_matrix.shape[0]):
        if traj_matrix[j][1] > max_longitude:
            max_longitude = traj_matrix[j][1]
        if traj_matrix[j][1] < min_longitude:
            min_longitude = traj_matrix[j][1]
        if traj_matrix[j][2] > max_latitude:
            max_latitude = traj_matrix[j][2]
        if traj_matrix[j][2] < min_latitude:
            min_latitude = traj_matrix[j][2]
    
    cur_traj_x = traj_matrix[:, 1:2]
    cur_traj_y = traj_matrix[:, 2:3]
    plt.plot(cur_traj_x, cur_traj_y)

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

