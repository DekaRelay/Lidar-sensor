import read_sci, threading
import numpy as ny
import matplotlib.pyplot as plt
import seaborn as sb

# loop1 is the function thread that plot the current data continuously
def loop1(obj, row, col):
    # Open the interact model of the plot
    plt.ion()
    while 1:
        x = []
        for i in range(row):
            col_array = []
            for j in range(col):
                col_array.append(obj[i*row+j])
            x.append(col_array)
        print(x)
        # Plot the data in the matrix
        cmap = "greys" # Set the color to gray
        sb.heatmap(x, vmin=0, # y axis range
                     vmax=40, cmap=plt.get_cmap('Greys')) #annot=True
        plt.show(block=False)
        plt.pause(0.1)
        plt.cla()  # Clear axis
        plt.clf()  # Clear figure
    return x


# loop2 is calling the function read_sci to read the data from sci continuously
def loop2(obj):
    # print('loop2', obj)
    pass

if __name__ == '__main__':
    # Define some values
    PIXEL_SIZE = 64  # 8*8 figure
    PIXEL_ROW = 8  # 8 pixel a row
    PIXEL_COL = 8  # 8 pixel a col
    DATA_SIZE = 16  # 16 bit oif one data

    # set a class called user to read sci
    user = read_sci.Map('com3')
    x = user.read_data_from_sci_arbitrary()

    td = threading.Thread(target=loop1, args=(user.x, PIXEL_ROW, PIXEL_COL))
    tr = threading.Thread(target=loop2, args=(user.port,))
    td.start()
    tr.start()
