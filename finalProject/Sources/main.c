#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"

//Table variable which will contain vector 
double data;
double a, b; 


void setupSerialPt1(void) 
{

  // Set baudrate to 9600
  int baudRate = 156; // SBR: ((24*10^6)/(16*9600) = 156 (for 9600 baud rate)
  SCI1BDH = 0;
  SCI1BDL = baudRate; 
  
  // Enable SCI1
  SCI1CR1 = 0;
  
  SCI1CR2 = 12; // TE and RE
  
     
}


void main(void) {
  //function for scanning room
  a = 100; 
  b = a ;
  
  setupSerialPt1(); // Setup serial port 1 
  scanInit(); // Initialize lidar and servo
  
  while (1) // infinite loop scans for room
  {
    scanRoom(); 
    movementdelay(); // Add delay between each scan element 
  }
  
  
  // ***** UNCOMMENT BELOW LINE FOR 1 SCAN (DEMONSTRATION)
  //scanRoom();





  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
