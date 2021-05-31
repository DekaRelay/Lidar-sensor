import serial
import serial.tools.list_ports


port = input("Enter Port: ")


ser = serial.Serial(port, 9600, timeout=0.1)


count_curr = 0
max_count = 5
while (True):
        if count_curr >= max_count:
            break
        byte_data = ser.read(1)
        if len(byte_data) == 0:
            continue
        letter = byte_data.decode("UTF-8")
        print(letter, end="")


