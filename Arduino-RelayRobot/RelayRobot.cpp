#include "RelayRobot.h"

RelayRobot::RelayRobot(uint8_t leftPwm, uint8_t leftDirL, uint8_t leftDirH,
uint8_t rightPwm, uint8_t rightDirL, uint8_t rightDirH){

  this->leftPwm = leftPwm;
  this->leftDirL = leftDirL;
  this->leftDirH = leftDirH;

  this->rightPwm = rightPwm;
  this->rightDirL = rightDirL;
  this->rightDirH = rightDirH;

  pinMode(leftPwm, OUTPUT);
  pinMode(leftDirL, OUTPUT);
  pinMode(leftDirH, OUTPUT);

  pinMode(rightPwm, OUTPUT);
  pinMode(rightDirL, OUTPUT);
  pinMode(rightDirH, OUTPUT);

  analogWrite(leftPwm, 0);
  digitalWrite(leftDirL, LOW);
  digitalWrite(leftDirH, LOW);

  analogWrite(rightPwm, 0);
  digitalWrite(rightDirL, LOW);
  digitalWrite(rightDirH, LOW);

}

void RelayRobot::updateRobot(int leftSpeed, int rightSpeed){
  if(leftSpeed > 0){
    digitalWrite(leftDirH, HIGH);
    digitalWrite(leftDirL, LOW);
  }else if(leftSpeed < 0){
    digitalWrite(leftDirH, LOW);
    digitalWrite(leftDirL, HIGH);
  }else{
    digitalWrite(leftDirH, LOW);
    digitalWrite(leftDirL, LOW);
  }

  if(rightSpeed > 0){
    digitalWrite(rightDirH, HIGH);
    digitalWrite(rightDirL, LOW);
  }else if(rightSpeed < 0){
    digitalWrite(rightDirH, LOW);
    digitalWrite(rightDirL, HIGH);
  }else{
    digitalWrite(rightDirH, LOW);
    digitalWrite(rightDirL, LOW);
  }

  analogWrite(leftPwm, abs(leftSpeed));
  analogWrite(rightPwm, abs(rightSpeed));
}
