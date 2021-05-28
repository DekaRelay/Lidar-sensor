#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "structs.h"
#include "functions.h"
#include "interrupts.h"

//Table variable which will contain vector 
double data;
double a, b; 

void main(void) {
  //function for scanning room
  a = 100; 
  b = a ;
  data = scanRoom();





  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
