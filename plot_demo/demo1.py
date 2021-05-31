import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sb
import random

PIXEL_SIZE = 64 # 8*8 figure
PIXEL_ROW = 8   # 8 pixel a row
PIXEL_COL = 8   # 8 pixel a col
DATA_SIZE = 16  # 16 bit oif one data


# Open the interact model of the plot
plt.ion()
while 1:
    np.random.seed(random.randint(20000000,30000000))       #
    x = np.random.randn(8, 8)*40                            #
    print(x)
    # Plot the data in the matrix
    cmap = "greys"
    sb.heatmap(x, vmin=0, # y axis range
                 vmax=40, cmap=plt.get_cmap('Greys')) #annot=True
    plt.show(block=False)
    plt.pause(0.1)
    plt.cla()  # Clear axis
    plt.clf()  # Clear figure