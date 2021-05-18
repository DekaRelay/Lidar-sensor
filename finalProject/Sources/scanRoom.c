#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "interrupts.h"     //Interrupt vector header

//Initialisation of sub-functions
void horizontalShift(double);                 //Controls horizontal movement
void verticalShift(double);                   //Controls vertical movement
void initLidar(void);                         //Initialises LiDAR communication
double getDistance(void);                     //Obtains the distance info from LiDAR
__interrupt void tovisr(void);                //Interrupt to manage timer count overflows
void  resetLiDAR(void);

//Initialisation of variables
int edge1;                                    //Stores the timer count at first edge
int overflow;                                 //Stores the overflow number of timer overflows
int pulse_width;                              //Stores the size of the pulse in timer counts
double length;                                //Stores the size of the pulse in milliseconds
double distance;                              //Recives the pulse length in m


double scanRoom(void) {

    // Initialise the operation range for the servos in degrees
   int motorMax = 180;
   int motorMin = 0;

   //Determine the data intervals position the LiDAR for data points
   double interval = (motorMax - motorMin)/8;

   //Initialise LiDAR communication
   initLidar();

   //Initialise loop variables and dimension of LiDAR position matrix
   int x;
   int y;
   int points = 8;

   //Loop through vertical positions, rows
   for (x = points; x >= 0; --x){
      //Position the servo
      verticalShift(x * interval);

      //Check the position of the LiDAR here

      //Reset internal loop counter
      y = 0;

      //Loop through horizontal positions
      for (y=1; y<= points; ++y){

        //Position the servo horizontally
        horizontalShift(y * interval);

        //Obtain the distance from the LiDAR
        distance = getDistance();

      }
   }


   //Individual function calls for testing: /*
/*    initLidar();
    horizontalShift(90);
    verticalShift(90);
    distance = getDistance();*/


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}




//Function to enable movement in vertical axis
// Function takes position in degrees (0 - 180);
void verticalShift(double vertical){

  /*
  Max Angle  = 10.5% Duty Cyle (Vertical = 180)
  Middle Angle  = 7.5%  Duty Cycle (Vertical  = 90)
  Min Angle  = 4.5% Duty Cycle (Vertical = 0)
  */


  //Conversion of degrees to ON period
  double DutyCycle   = ((vertical/180)*(1.2) + 0.9)/20;
  double ON = DutyCycle * 30000;


  PWMCLK = 0;                   //  Clock A is selected as the clock source
  PWMPOL = 32;                  //  Initial Output is set to HIGH
  PWMPRCLK = 4;                 //  Prescaler of 16 selected
  PWMCTL = 0x4C;                //  Concatenate PWM4:PWM5
  PWMCAE = 0;                   //  Left Align selected


  PWMPER45 = 30000;              //  Period is set to 30,000
  PWMDTY45 = ON;                 //  Duty set according to input angle

  PWME_PWME5 = 1;                //  PWM5 is enabled


  //Wait for 10 PWM cycles before disabling signal
  while (PWMCNT5 <= 100);

 //PWME = 0;                     //  Disable PWM Cycle
 //PWMCNT5 = 0;                  //  Reset the count

}


//Function to enable movement in horizontal axis
void horizontalShift(double horizontal){

  /*
  Max Angle  = 10.5% Duty Cyle (Horizontal = 180)
  Middle Angle  = 7.5%  Duty Cycle (Horizontal  = 90)
  Min Angle  = 4.5% Duty Cycle (Horizontal = 0)
  */


  //Conversion from degrees to ON period
  double DutyCycle   = ((horizontal/180)*(1.2) + 0.9)/20;
  double ON = DutyCycle * 30000;

  //Initialisation of PWM Signal
  PWMCLK = 0;                   //  Clock B is selected as the clock source
  PWMPOL = 128;                 //  Initial Output is set to HIGH
  PWMPRCLK = 4;                 //  Prescaler of 16 selected
  PWMCTL = 0x8C;                //  Concatenate PWM6:PWM7
  PWMCAE = 0;                   //  Left Align selected


  PWMPER67 = 30000;             //  Period is set to 30,000
  PWMDTY67 = ON;                //  Duty set according to input angle

  PWME_PWME7 = 1 ;              //  PWM7 is enabled


 //Disabling of PWM Signal after motor has moved
while (PWMCNT7 <= 100);         //  Run PWM for 10 Cycles to shift servo
//PWME = 0;                      //  Disable PWM Cycle
//PWMCNT7 = 0;                   //  Reset the count

}


//Function to initialise the LiDAR communication
void initLidar(void){

  DDRT = 0x00;                  //  Initialise PORT T for input
  DDRH = 0xFF;                  //  Initialise PORT H for output

  PTH_PTH0 = 1;                 //  Set PIN 0 of PORT H to HIGH

}

//Function to obtain distance value from LiDAR
double getDistance(void){

 PTH_PTH0 = 0;                  //  Send trigger signal to LiDAR

 TSCR1 = 0x90;                  //  Enable timer count and fast timer flag clear
 TSCR2 = 0x04;                  //  Set prescaler to 16

 TIOS_IOS1 = 0;                 //  Enable input capture
 DLYCT = 0x02;                  //  Set delay count to 1024 E cycles to eliminate noise
 ICOVW_NOVW1 = 1;               //  Disable overwrite to TCO register
 ICSYS = 0;                     //  Disable queue mode

 TCTL4 = 0x04;                  //  Capture the rising edge
 TFLG1_C1F = 1;                 //  Clear C1F flag

 while (!(TFLG1_C1F));          //  Wait for rising edge

 TFLG2 = 0x80;                  //  Clear the TOF flag

 TSCR2_TOI = 1;                 //  Enable TCNT overflow interrupt

 EnableInterrupts;              // Enable interrupts

 //TFLG1_C1F = 1;                 //  Clear the C1F flag
 edge1 = TC1;                   //  Store the first edge time in edge1

 TCTL4 = 0x08;                  //  Capture the falling edge

 while (!(TFLG1_C1F));          //  Wait for falling edge

 pulse_width = TC1 - edge1;     //  Calculate the pulse width as the difference of the edges

 if (TC1 < edge1){              //  Check if the second edge is smaller
  overflow--;                   //  Decrement the overflow counter
 }

 PTH_PTH0 = 1;                  //  Disable LiDAR trigger

 length = (2 * (double)pulse_width)/3000;   // Convert the width from counts to milliseconds

 return length;                 // Return the timed width of the pulse

}

//  Interrupt to increment overflows
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void tovisr(void){
  TFLG2 = 0x80;                  // Clear the timer overflow flag
  overflow++;                    // Increment the overflow
}

//Function to reset LiDAR values after measurement
void resetLiDAR(void){
   overflow = 0;
   TC1 = 0;
   pulse_width = 0;
   length = 0;
   edge1 = 0;

}
