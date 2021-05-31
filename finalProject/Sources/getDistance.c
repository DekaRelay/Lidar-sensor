#include <hidef.h>      /* common defines and macros */
#include <stdio.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"

unsigned int edge1, pulse_width; 
double length;                      
int p, q;


//void initLidar(void); 
void movementdelay(void);

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

/*void initLidar(void){

  DDRT = 0x00;                  //  Initialise PORT T for input
  DDRH = 0xFF;                  //  Initialise PORT H for output

  PTH_PTH0 = 1;                 //  Set PIN 0 of PORT H to HIGH

}   */
