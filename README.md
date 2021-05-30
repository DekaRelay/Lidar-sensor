# MTRX2700---Group-3---Major-Project
Major Project for MTRX2700 Group 3

## INTRODUCTION:
The following project was produced by Devansh Shah,Rishabh Leelchand, Harry Jia
and James Hanna. The following major project aims to enhance a blind individuals'
spacial awareness by scanning their surroundings anddisplaying the output on a 3D
board that can be felt by the user to help themunderstand what is around them.
The project contains four main modules, these include:
  - LiDAR & Servometer Operation
  - Orientation Regulation
  - Serialisation of Output
  - Displaying Output as an Image in Python

## Group Members & Roles
  Devansh Shah
  - Storing of measurement data
  - Serialisation of Data

  James Hanna
  - LiDAR and Servometer Operation
  - Hardware Testing

  Rishabh Leelchand
  - Orientation of PTU Module during scans
  - Minutes

  Harry Jia
  - Python Image generation of output Data
  - Interfacing between Codewarrior and Python

## HIGH LEVEL CODE INFORMATION:
## LiDAR & Servometer Operation
Servometer operation is controlled by sending a 20 ms PWM signal to pins 5 & 7 of
Port T. The sub-functions 'horizontalShift' and 'verticalShift' control these actions
by taking the position as an angular position, in degrees, which is then converted
into a duty cycle ON period that can replicate the angle in the servometer.

LiDAR operation is controlled by utilising the timer input counter. The pulse width of
the LiDAR signal is counted after the trigger signal is sent via PHO. The pulse width
is then converted to a length value that is returned and stored in the output.

Both the LiDAR and Servometer are operated in the main body of the scanRoom function
inside a nested loop, the nested loop works to create a square-matrix of positions
to move the servo through as data points. The (x,y) positions are converted to angles
and then used as inputs to the servo position functions, the LiDAR distance value is
then taken at each position and stored in a matrix to be returned to the main function.
Prior to each position, the orientation module is called to check that the PTU position
has not changed to ensure a consistent scan is completed while being held by the user.


### Testing Plan & Procedure
Pending writing of test module

## Orientation of PTU Module during scans  
### Testing Plan & Procedure

## Serialisation of Output Data
One scan of a room involves positioning PTU at chosen 64 different poisitons by adjusting horizontal and vertical angles. At each of these points, lidar sensor gives a distance measurement. 64 of these points is used to create a 8x8 matrix to give a spatial representation of the room. The scanRoom() function creates one scan of the room as described, during the scan each data point is sent via serial port in this format: 
<br /> <br />
A|B||C|D||| 
<br /> <br />
A,B,C,D represent floating point numbers representing distance measurement from lidar sensor. A singular pipe symbol (|) is used to seperate the numerical values, two pipe symbols in a row indicate new row in matrix, and finally three pipes in a row indicate end of transmission for this particular scan. The above serial data represents matrix: 
<br /> <br />
\[\[A,B\],\[C,D\]\] 
<br /> <br />

This format for serial data transmission was to allow flexibility in code for different resolutions and aspect rations of the spatial scan of room, not just an 8x8 grid. The serial data is sent over via SCI1 port. scanRoom() function is continually called to create continuous scan of room.
### Testing Plan & Procedure
A premade array is created consisting of floating point values that represent lidar measurements. The sendSerial() command is used to send the serial data over SCI1 port, and using a virtual terminal we can check that whole data was sent correctly.

## Display of Data as a Python Image
In this part, the scan image of the room will be present as a 8x8 heatmap (64 pixel). The degree of 
heat will be shown by gray. Each pixel of the image represents the distance between the Lidar to the 
obstacles (usually the wall). So that the heatmap can shows the image of a rough room outline drawing.

Using a infinite loop in the polt model. When the data received from the serial port is keeping changing, 
which means the machine is scaning the code continuously, the hap shown by python will also follows that.
### Testing Plan & Procedure
1. Generate some random 8x8 matrix data using numpy and random libraries.
2. Input these data into the plot model.

If it shows the reasonable heatmap and always changing each of the pixel. The polt model could pass the test.
