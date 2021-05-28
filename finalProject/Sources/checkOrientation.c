#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

// need this for string functions
#include <stdio.h>
#include <math.h>

//#include "pll.h"
#include "simple_serial.h"
#include "l3g4200d.h"


float roll, pitch;
int maxAngle, minAngle;
AccelRaw read_accel;
AccelScaled scaled_accel;

int error_code;
unsigned char buffer[64];



void checkOrientation(void) {

  //minmax gyro angles
  maxAngle = 110;
  minAngle = 70;

  error_code = NO_ERROR;

  // initialise the simple serial
  SCI1_Init(BAUD_9600);

  // initialise the sensor suite
  error_code = iicSensorInit();

  // write the result of the sensor initialisation to the serial
  if (error_code == NO_ERROR) {
    sprintf(buffer, "NO_ERROR");
    SCI1_OutString(buffer);
  } else {
    sprintf(buffer, "ERROR %d");
    SCI1_OutString(buffer);
  }

	EnableInterrupts;

  for(;;) {

    // read the raw accel values
    getRawDataAccel(&read_accel);

    // convert the acceleration to a scaled value
    convertUnits(&read_accel, &scaled_accel);

    //calculate pitch and roll
    pitch = 57.3 * atan2(scaled_accel.x, sqrt(scaled_accel.y*scaled_accel.y + scaled_accel.z*scaled_accel.z));
    roll = 57.3 * atan2(scaled_accel.y, sqrt(scaled_accel.x*scaled_accel.x + scaled_accel.z*scaled_accel.z));

    //calculate the specific shift angle (might need work and further testing)
    pitch = 90 - pitch;
    roll = 90 + roll;

    //change the horizontalshift and verticalshift depending on angle

    //if(pitch > maxAngle){
    //  horizontalShift(maxAngle);
    //}else if(pitch < minAngle){
    //  horizontalShift(minAngle);
    //}else{
    //  horizontalShift(pitch);
    //}

    //if(roll > maxAngle){
    //  verticalShift(maxAngle);
    //}else if(roll < minAngle){
    //  verticalShift(minAngle);
    //}else{
    //  verticalShift(roll);
    //}

    // format the string of the sensor data to go the the serial
    sprintf(buffer, "%.2f, %.2f, %.2f, %.2f, %.2f \r\n", scaled_accel.x, scaled_accel.y, scaled_accel.z, pitch, roll);


    // output the data to serial
    SCI1_OutString(buffer);

    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */

  /* please make sure that you never leave main */
}
