import sys
import numpy as np
import cv2
import scipy.io as sio
import matplotlib.pyplot as plt
from scipy import ndimage
from numpy import *

port_side_data = sys.argv[1]
stbd_side_data = sys.argv[2]
port_mat = sio.loadmat(port_side_data)
port_matrix = np.array(port_mat['port_intensity_matrix'])
stbd_mat = sio.loadmat(stbd_side_data)
stbd_matrix= np.array(stbd_mat['stbd_intensity_matrix'])


port_image = cv2.flip(port_matrix, 0)
stbd_image = cv2.flip(stbd_matrix, 0)

image_matrix = np.concatenate((port_image, stbd_image), axis = 1)

m_row = image_matrix.shape[0]
m_col = image_matrix.shape[1]

if image_matrix.max() > 255:   
    x_t = np.arange(0,65536,1)
    intensity_num = np.zeros(65536)
    
    for i in range(0, m_row):
        for j in range(0, m_col):
            intensity_num[math.ceil(image_matrix[i,j])] += 1
    
    cur_intensity_num = 0
    total_intensity_num = m_row * m_col
    intensity_threst_hold = 0.88
    max_intensity = 0

    for i in range(0,intensity_num.size):
        cur_intensity_num += intensity_num[i]
        if (cur_intensity_num/total_intensity_num) > intensity_threst_hold :
            break
        max_intensity = i
        
    for i in range(0, m_row):
        for j in range(0, m_col):
            if image_matrix[i,j] < max_intensity:
                image_matrix[i,j] = image_matrix[i,j] * 255 / max_intensity
            else:
                image_matrix[i,j] = 255
    
    plt.figure("image")
    plt.imshow(image_matrix, cmap = plt.cm.gray)
    plt.show()

else:
    plt.figure("image")
    plt.imshow(image_matrix, cmap = plt.cm.gray)
    plt.show()

