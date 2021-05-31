import serial
import serial.tools.list_ports


port = input("Enter Port: ")


ser = serial.Serial(port, 9600, timeout=0.1)


count_curr = 0
max_count = 5
while (True):
    while(ord(byte_data)):

        if count_curr >= max_count:
            break
        byte_data = ser.read(8)
        letter = byte_data.decode("ascii")
        print(letter)
        count_curr += 1


