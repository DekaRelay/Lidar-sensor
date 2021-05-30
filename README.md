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
### Testing Plan & Procedure

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