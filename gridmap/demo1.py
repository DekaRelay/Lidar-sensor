import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sb
import math
import random

plt.ion()  # Open the interact model
while 1:
    # Read data
    # np.random.seed(20210511)
    np.random.seed(random.randint(20000000,30000000))

    x = np.random.randn(8, 8)*40
    print(x)

    # Format data
    # Create fig
    # fig, i = plt.subplots(figsize=(6,6))
    # print(fig)

    # Fill the fig
    cmap = "greys"
    sb.heatmap(x, vmin=0, # y axis range
                 vmax=40, cmap=plt.get_cmap('Greys')) #annot=True
    plt.show(block=False)
    plt.pause(0.1)
    plt.cla()  # Clear axis
    plt.clf()  # Clear figure
    # plt.close(fig)

    # break
    # Plot
