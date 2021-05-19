import serial
import serial.tools.list_ports
import numpy as np

PIXEL_SIZE = 64 # 8*8 figure
PIXEL_ROW = 8   # 8 pixel a row
PIXEL_COL = 8   # 8 pixel a col
DATA_SIZE = 16  # 16 bit oif one data

class Map:
    # global x
    x = np.zeros(PIXEL_SIZE)

    def __init__(self,port_name):
        # Initialize the serial connection
        self.port = port_name
        self.ser = serial.Serial(port_name, 9600, timeout=0.1)

    def read_data_from_sci(self):
        # Read 8*8 data at once
        for i in range(PIXEL_SIZE):
            self.x[i] = self.ser.read().hex()
            # self.x[i] = i-20
        return self.x

    def data_output(self):
        return self.x
