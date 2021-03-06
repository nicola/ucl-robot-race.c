#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "rangefinders.h"

bool moveIR (int angle) {
  if (angle > 90 || angle < -90) return false;
  sprintf(buf, "I R %i\n", angle);
  #ifdef DEBUG
  printf("I R %i\n", angle);
  #endif
  nextCmd(); // TODO parseCmd
  r.s.rangeFRAngle = angle;
  return true;
}

int moveIL (int angle) {
  if (angle > 90 || angle < -90) return false;
  sprintf(buf, "I L %i\n", angle);
  #ifdef DEBUG
  printf("I L %i\n", angle);
  #endif
  nextCmd();
  r.s.rangeFLAngle = angle;
  return true;
}

int moveILR (int angle1, int angle2) {
  if (angle1 > 90 || angle2 < -90) return false;
  if (angle2 > 90 || angle1 < -90) return false;
  sprintf(buf, "I LR %i %i\n", angle1, angle2);
  #ifdef DEBUG
  printf(buf, "I LR %i %i\n", angle1, angle2);
  #endif
  nextCmd();
  r.s.rangeFLAngle = angle1;
  r.s.rangeFRAngle = angle2;
  return true;
}

void rangeFCmd() {
  sprintf(buf, "S IFLR\n");
  #ifdef DEBUG
  printf("S IFLR\n");
  #endif
  nextCmd();
}

void rangeFGet(sensors* Sensors) {
  char* elaborated[80];
  rangeFCmd();
  parseCmd(buf, elaborated, SIFLR, Sensors);
};

void rangeFSet(sensors* Sensors, int l, int r) {
  Sensors->rangeFL = l;
  Sensors->rangeFR = r;
};

double rangeFLSide(double distance) {
  int offset = 0;
  if (r.s.rangeFLAngle <= -56) offset = 26 - 8*sqrt(2)/2;
  return absDouble(cos((r.s.rangeFLAngle+45)*M_PI/180)*distance) - offset;
};

double rangeFRSide(double distance) {
  int offset = 0;
  if (r.s.rangeFRAngle <= -56) offset = 26 - 8*sqrt(2)/2;
  return absDouble(cos((r.s.rangeFRAngle-45)*M_PI/180)*distance) - offset;
};
double rangeFLFront(double distance){
  return absDouble(sin((r.s.rangeFLAngle+45)*M_PI/180)*distance);
};
double rangeFRFront(double distance){
  return absDouble(sin((r.s.rangeFRAngle-45)*M_PI/180)*distance);
};

int rangeFParse(char* elaborated[], sensors* Sensors) {
  if (elaborated[2] == NULL || elaborated[3] == NULL ||
      !strcmp(elaborated[2],"") || !strcmp(elaborated[3],"") ) return 1;

  rangeFSet(Sensors, gp2d12_ir_to_dist(atoi(elaborated[2])), gp2d12_ir_to_dist(atoi(elaborated[3])) );
  // it should be 0 for failure, 1 for silent, 2 for OK
  return 2;
};

/* encodersToBe
// return true when is not yet toBe, return false when it goes over toBe
*/

bool rangeFToBe(sensors* current, sensors* initial, sensors* toBe) {
  int l,r;

  if (sensorToBe(current->rangeFL, initial->rangeFL, toBe->rangeFL)) {l=0;}
  else {l=1;}
  if (sensorToBe(current->rangeFR, initial->rangeFR, toBe->rangeFR)) {r=0;}
  else {r=1;}

  if (l == 1 && r == 1) {
    return false;
  }
  return true; 
}


void rangeSCmd() {
    sprintf(buf, "S ISLR\n");
#ifdef DEBUG
    printf("S ISLR\n");
#endif
    nextCmd();
}

void rangeSGet(sensors* Sensors) {
    char* elaborated[80];
    rangeSCmd();
    parseCmd(buf, elaborated, SISLR, Sensors);
}

void rangeSSet(sensors* Sensors, int l, int r) {
    Sensors->rangeSL = l;
    Sensors->rangeSR = r;
}

int rangeSParse(char* elaborated[], sensors* Sensors) {
    if (elaborated[2] == NULL || elaborated[3] == NULL ||
        !strcmp(elaborated[2],"") || !strcmp(elaborated[3],"") ) return 1;
    
    rangeSSet(Sensors, gp2d120_ir_to_dist(atoi(elaborated[2]))-2, gp2d120_ir_to_dist(atoi(elaborated[3]))-2);
    // it should be 0 for failure, 1 for silent, 2 for OK
    return 2;
}

/* encodersToBe
 // return true when is not yet toBe, return false when it goes over toBe
 */

bool rangeSToBe(sensors* current, sensors* initial, sensors* toBe) {
    int l,r;
    
    if (sensorToBe(current->rangeSL, initial->rangeSL, toBe->rangeSL)) {l=0;}
    else {l=1;}
    if (sensorToBe(current->rangeSR, initial->rangeSR, toBe->rangeSR)) {r=0;}
    else {r=1;}
    
    if (l == 1 && r == 1) {
        return false;
    }
    return true; 
}