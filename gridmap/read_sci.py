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
            self.x[i] = int(self.ser.read(8),2)
            # self.x[i] = i-20
        return self.x
    
    def read_data_from_sci_arbitrary(self):
    
        # Below code is to give general outline of how serial data is to be read
        
        flag_count = 0
        got_num = False
        num_str = ""
        num_float = 0
        
        
        # These below can be caluclated
        num_of_cols = 0 
        num_of_rows = 0
        
        index = 0
        while(True):
            while (a == chr(self.ser.read(8))):
                data = ord(a) # Get ascii value
                if (data >= 48 and data <= 57): # Ascii check of number 0-9
                    got_num = True
                    num_str += str((data - 48)) # Add number to string
                elif (data == 46): # Ascii check for decimal point
                    num_str += "."
                elif (data == ord('|')): # Check ascii values
                    flag_count += 1

                if (flag_count >= 3):
                    break
                elif (flag_count == 1 and got_num):

                    num_float = float(num_str)
                    self.x[index] = num_float

                    # Reset all
                    flag_count = 0
                    got_num = False
                    num_str = ""

                index += 1

    def data_output(self
            ):
        return self.x
