#include "MapPWM.h"

MapPWM::MapPWM(int joyMax, int joyMin, int offset, int minPwm, int maxPwm){
  this->joyMax = joyMax;
  this->joyMin = joyMin;
  joyMid = (joyMax+joyMin)/2;
  upperOffset = joyMid + offset;
  lowerOffset = joyMid - offset;
  this->maxPwm = maxPwm;
  this->minPwm = minPwm;
  leftPwm = rightPwm = 0;
}

void MapPWM::updateAxis(int axisX, int axisY){
  int speedFwd;
  int speedTurn;

  if(axisY > upperOffset) //FWD
    speedFwd = map(axisY, upperOffset, joyMax, minPwm, maxPwm);
  else if(axisY < lowerOffset) //BWD
    speedFwd = map(axisY, lowerOffset, joyMin, -minPwm, -maxPwm);
  else
    speedFwd = 0;

  if(axisX > upperOffset) //right
    speedTurn = map(axisX, upperOffset, joyMax, minPwm, maxPwm);
  else if(axisX < lowerOffset) //left
    speedTurn = map(axisX, lowerOffset, joyMin, -minPwm, -maxPwm);
  else
    speedTurn = 0;

  leftPwm = speedFwd + speedTurn;
  rightPwm = speedFwd - speedTurn;

  leftPwm = constrain(leftPwm, -maxPwm, maxPwm);
  rightPwm = constrain(rightPwm, -maxPwm, maxPwm);
}

int MapPWM::getLeftPwm(){
  return leftPwm;
}
int MapPWM::getRightPwm(){
  return rightPwm;
}
