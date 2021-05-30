#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "interrupts.h"     //Interrupt vector header
#include "functions.h"

//Initialisation of functions
void horizontalShift(double);                 //Controls horizontal movement
void verticalShift(double);                   //Controls vertical movement
void initLidar(void);                         //Initialises LiDAR communication
double getDistance(void);                     //Obtains the distance info from LiDAR
void servosInit(void);
void movementdelay(void);
void sendSerial(char* inStr);                     // Send given string over serial port 1 

//Initialisation of variables
unsigned int edge1, pulse_width; 
double length, distance;                      
int x, y, points, p, q, tdreResult, charData, index;

char convertor[10]; // Hold floating point to string conversion

void sendSerial(char* inStr) 
{


  // Function sends string data over serial port 1
  index = 0;
  while (1) 
  {
    charData = inStr[index];
    tdreResult = SCI1SR1 & 128;
    if (tdreResult != 0) 
    {
      if (charData == '\0') // Once end of string reached 
      {
        SCI1DRH = 0;
        SCI1DRL = '|'; // Indication of speration 
        break; 
      }
      
      SCI1DRH = 0;
      SCI1DRL = charData; // Send data
      index = index + 1;
    }
      
  }
}

void scanInit(void) 
{
  //Initialise LiDAR communication and servos
   initLidar();
   servosInit();
}

void scanRoom(void){

    // Initialise the operation range for the servos in degrees
   int motorMax = 160;
   int motorMin = 20;

   //Determine the data intervals position the LiDAR for data points
   double interval = (motorMax - motorMin)/8;

   //Initialise number of axis points for the loop to stop at 

   points = 8;

   //Loop through vertical positions, rows
   for (x = points; x >= 0; --x){
      //Position the servo
      verticalShift(x * interval);

      //Loop through horizontal positions
      for (y=1; y<= points; ++y){
        //Orientate LiDAR
        checkOrientation(x, y); 

        //Position the servo horizontally
        horizontalShift(y * interval);

        //Obtain the distance from the LiDAR
        distance = getDistance();
        
        sprintf(convertor, "%.2f", distance); // convert double into string
        sendSerial(convertor);

      }
      sendSerial("|"); // Double pipe indicates new row
      
   }
   sendSerial("|"); // Triple pipe indicates end of matrix transmission
   
   //Set the final position of the LiDAR to centre and turn off signals 
   horizontalShift(90); 
   verticalShift(90); 
   PWME_PWME5 = 0;
   PWME_PWME7 = 0; 
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
  double DutyCycle = (((vertical - 7)/180)*(1.2) + 0.9)/20;
  double ON = DutyCycle * 30000;

  PWMPER45 = 30000;              //  Period is set to 30,000
  PWMDTY45 = ON;                 //  Duty set according to input angle

  PWME_PWME5 = 1;                //  PWM5 is enabled
  movementdelay();               //  Delay to allow servo to complete movement 
}

//Function to enable movement in horizontal axis
void horizontalShift(double horizontal){

  /*
  Max Angle  = 10.5% Duty Cyle (Horizontal = 180)
  Middle Angle  = 7.5%  Duty Cycle (Horizontal  = 90)
  Min Angle  = 4.5% Duty Cycle (Horizontal = 0)
  */
  
  //Conversion from degrees to ON period
  double DutyCycle = (((horizontal + 5)/180)*(1.2) + 0.9)/20;
  double ON = DutyCycle * 30000;
  
  PWMPER67 = 30000;             //  Period is set to 30,000
  PWMDTY67 = ON;                //  Duty set according to input angle

  PWME_PWME7 = 1 ;              //  PWM7 is enabled
  movementdelay();              //  Delay to allow servo to complete movement 

}


//Function to initialise the LiDAR communication
void initLidar(void){

  DDRT = 0x00;                  //  Initialise PORT T for input
  DDRH = 0xFF;                  //  Initialise PORT H for output

  PTH_PTH0 = 1;                 //  Set PIN 0 of PORT H to HIGH

}

//Function to obtain distance value from LiDAR
double getDistance(void){

 TSCR1 = 0x90;                  //  Enable timer count and fast timer flag clear
 TSCR2 = 0x03;                  //  Set prescaler to 16
 TIOS_IOS1 = 0;                 //  Enable input capture
 DLYCT = 0x07;                  //  Set delay count to 1024 E cycles to eliminate noise
 ICOVW_NOVW1 = 1;               //  Disable overwrite to TC1 register
 ICSYS = 0;                     //  Disable queue mode
 TCTL4 = 0x04;                  //  Capture the rising edge
 TFLG1_C1F = 1;                 //  Clear C1F flag

 PTH_PTH0 = 0;                  //  Send trigger signal to LiDAR
 while (!(TFLG1_C1F));          //  Wait for rising edge

 TFLG2 = 0x80;                  //  Clear the TOF flag
 edge1 = TC1;                   //  Store the first edge time in edge1
 TCTL4 = 0x08;                  //  Capture the falling edge

 while (!(TFLG1_C1F));          //  Wait fro faling edge

 pulse_width = TC1 - edge1;     //  Calculate the pulse width as the difference of the edges

 PTH_PTH0 = 1;                  //  Disable LiDAR trigger
 length = ((double)pulse_width )/3000;   // Convert the width from counts to milliseconds
     TSCR1 = 0x00;  

 return length - 0.1;           // Return the timed width of the pulse

}


void servosInit(void){
  DDRP = 0x00;                   // Set direction of PORTP to output
  PWMCLK = 0x00;                 //  Clocks A & B are selected as the clock source
  PWMPOL = 0xA0;                 //  Initial Output is set to HIGH
  PWMPRCLK = 68;                 //  Prescaler of 16 selected
  PWMCTL = 0xCC;                 //  Concatenate PWM6:PWM7 and PWM4:PWM5
  PWMCAE = 0;                    //  Left Align selected
}


void movementdelay(void){              // Delay to allow servo to complete action 
     for (q = 0; q <= 5; q++){
      for (p = 0; p <= 30000; p++){
        asm{
          psha
          pula
          psha
          pula
          psha
          pula
          psha
          pula
          psha
          pula
          psha
          pula
          psha
          pula
          nop
          nop
        }
      }
    }
}




