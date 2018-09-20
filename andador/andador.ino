/**
   GEPID - Grupo de Estudo e Pesquisa em Inclusão Digital
    UPF  -          Universidade de Passo Fundo
   Autores : Angelo Elias Dalzotto
             Gabriel Boni Vicari
             Nicolas Lodéa
   Data    : Setembro de 2017
   Projeto : Andador
*/

#include <JoyBTCommander.h>
#include <SmoothDrive.h>
#include <MapPWM.h>
#include <RelayRobot.h>

#define leftGnd  4
#define rightGnd  11

#define BTBAUD 9600
#define BTRX   2
#define BTTX   3

#define TIMEACCEL 3000
#define TIMEDEACC 1000
#define MAXPWM 255

#define JOYMAX  100
#define JOYMIN -100
#define OFFSET   16
#define MINPWM   0

#define LEFTPWM    5
#define LEFTDIRL   6
#define LEFTDIRH   7
#define RIGHTPWM  10
#define RIGHTDIRL  9
#define RIGHTDIRH  8

JoyBTCommander Bt(BTBAUD, BTRX, BTTX);
SmoothDrive PwmLeft(TIMEACCEL, TIMEDEACC, MAXPWM*0.95);
SmoothDrive PwmRight(TIMEACCEL, TIMEDEACC, MAXPWM);
MapPWM Axis(JOYMAX, JOYMIN, OFFSET, MINPWM, MAXPWM);
RelayRobot Robot(LEFTPWM, LEFTDIRL, LEFTDIRH, RIGHTPWM, RIGHTDIRL, RIGHTDIRH);

void setup() {
  Serial.begin(38400);
  pinMode(leftGnd, OUTPUT);
  digitalWrite(leftGnd, LOW);
  pinMode(rightGnd, OUTPUT);
  digitalWrite(rightGnd, LOW);
}

void loop() {
  if(Bt.update() == 2){
    Axis.updateAxis(Bt.getAxisX(), Bt.getAxisY());
    PwmLeft.setPwmRef(Axis.getLeftPwm());
    PwmRight.setPwmRef(Axis.getRightPwm());
  }
  Serial.println(PwmLeft.getPwmOut());
  Robot.updateRobot(PwmLeft.getPwmOut(), PwmRight.getPwmOut());
}
