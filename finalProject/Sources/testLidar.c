#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"

double testDistance; 

double testLidar(void){
  initLidar();
  servosInit(); 
  horizontalShift(90); 
  verticalShift(90); 
  testDistance = getDistance();
  return testDistance; 
}
