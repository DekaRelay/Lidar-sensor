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
into a duty cycle ON period that can replicate the angle in the servomotor.

LiDAR operation is controlled by utilising the timer input counter. The pulse width of
the LiDAR signal is counted after the trigger signal is sent via PHO. The pulse width
is then converted to a length value that is returned and stored in the output.

Both the LiDAR and Servomotor are operated in the main body of the scanRoom function
inside a nested loop, the nested loop works to create a square-matrix of positions
to move the servo through as data points. The (x,y) positions are converted to angles
and then used as inputs to the servo position functions, the LiDAR distance value is
then taken at each position and stored in a matrix to be returned to the main function.
Prior to each position, the orientation module is called to check that the PTU position
has not changed to ensure a consistent scan is completed while being held by the user.


### Testing Plan & Procedure
The testing of the module includes the testing of the servomotors and LiDAR scanner
individually. The purpose of this is to ensure each component can operate correctly
when isolated from the rest of the program. The following procedures will help to
test the performance of each of the components:
  Testing the Servomotors:
  1. Connect the PTU and HCS-12 Board
  2. Select a known angle position to move the servomotor towards during the test.
     A preferable value is 90 degrees and observing if the position of the LiDAR will
     be flat and centred.
  3. Initialise the servomotor communication by running the function servosInit().
  4. Call the appropriate servomotor function for the desired axis that the
     movement will occur in. "horiztonalShift()" will move the servomotor in the
     horizontal axis, "verticalShift()" will move the servomotor in the vertical
     axis.
  5. Write the expected angle in degrees as an input to the function and run the
     code:
      int angle;

      servosInit();
      verticalShift(angle) or horizontalShift(angle);

  6. Observe the movement in the servomotor and check that the movement goes to
     the anticipated position specified in the function call.

  Testing the LiDAR Scanner:
  1. Connect the PTU and HCS-12 Board
  2. Initialise the LiDAR and servomotors using the functions initLidar() and
     servosIniti().
  3. Set the position of the servomotors to a flat, default position to ensure
     angles do not influence the measurement. This is achieved by the following
     lines:
     horizontalShift(90);
     verticalShift(90);
  4. Place a black sheet a known distance away from the LiDAR component.
  5. Within a continuous loop to check that distance can be measured continuously,
     call the getDistance() function to measure the distance of the sheet from the
     LiDAR and store the output in a variable "(double) distance":

        double distance;
        servosInit();
        horizontalShift(90);
        verticalShift(90);

        while (1){
          distance  = getDistance();
        }
    6. Run the above code and set a breakpoint at the getDistance function call.
    7. Observe the distance value (m) returned to the distance variable in memory
       and compare to the measured distance of the black sheet.
    8. Continue running the code to check that consistent measurements are taken
       and the LiDAR can operate correctly when in a loop.

## Orientation of PTU Module during scans  
The Orientation of the PTU Module is done by initialising the IIC sensor. The main
component of the IIC sensor that will be used is the accelerometer, which will be converted
into the primary components: Pitch and Roll.

The Orientation module is incorporated in the checkOrientation function, which takes in the
current x and y angles of the Lidar Servos. It then sets the minimum and maximum angles
that the Lidar sensor can stay in range before the Lidar Sensor loop stops and pauses.
The function then goes into a loop in which it reads the accelerometer reading, and then
calculates the Pitch and Roll using appropriate equations. It then checks if the angle is
within the range of +-10 degrees. If it is then it exits the loop and returns back to the
main function.

The checkOrientation function is called after every servo movement of the Lidar sensor.
This allows a moderately accurate check for orientation when taken into account with the
speed of the main function loop.

### Testing Plan & Procedure
As most of the error checking is done within the drivers given to us in the major project
drivers, the testing will therefore focus on the applications of the Gyro Orientation
detection.
The Orientation data will be checked using the Serial port, where the data will be output
periodically within a loop.

1. Check the accelerometer values when the PTU is stationary
2. Tilt the PTU in 10 degree increments in each compass direction and ensure the accelerometer
shows appropriate readings
3. Output the calculated Pitch and Roll readings
4. Tilt the PTU in 10 degree increments in each compass direction and ensure the readings
are correct (This can be aided with the use of a protractor)

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
which means the machine is scanning the code continuously, the hap shown by python will also follows that.
### Testing Plan & Procedure
1. Generate some random 8x8 matrix data using numpy and random libraries.
2. Input these data into the plot model.

If it shows the reasonable heatmap and always changing each of the pixel. The polt model could pass the test.
