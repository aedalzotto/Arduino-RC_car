#include "SmoothDrive.h"

#define ACCELERATE      1
#define DEACCELERATE   -1
#define STANDSTILL      0
#define CHANGESIGNALD  -2
#define CHANGESIGNALA   2

/**************************************************************************/
/*!
    @brief  Instantiates a new SmoothDrive class

    @param[in]  timeTotal
                The total time (seconds) taken to go from 0 to VMAX

    @param[in]  vmax
                The maximum PWM value the output can reach
*/
/**************************************************************************/
SmoothDrive::SmoothDrive(unsigned long long int timeAccel,
                         unsigned long long int timeDeacc, unsigned int VMAX){
  this->VMAX = VMAX;                       //PWM máximo
  this->timeAccel = timeAccel;
  this->timeDeacc = timeDeacc;            //Tempo de 0 - VMAX (msecs)

  timeSecsAccel = (double)timeAccel/1000.0;    //Tempo de 0 - VMAX (secs)
  accelA2 = (double)VMAX*2.0/timeSecsAccel;         //Constante eq
  accelA3 = (double)VMAX/(timeSecsAccel*timeSecsAccel);  //Constante eq
  accelA2PWR  = accelA2*accelA2;         //Constante Delta
  accelA3FOUR = 4.0*accelA3;        //Constante Delta
  accelA3TWO  = 2.0*accelA3;        //Constante Delta

  timeSecsDeacc = (double)timeDeacc/1000.0;
  deaccA2 = (double)VMAX*2.0/timeSecsDeacc;         //Constante eq
  deaccA3 = (double)VMAX/(timeSecsDeacc*timeSecsDeacc);  //Constante eq
  deaccA2PWR  = deaccA2*deaccA2;         //Constante Delta
  deaccA3FOUR = 4.0*deaccA3;        //Constante Delta
  deaccA3TWO  = 2.0*deaccA3;        //Constante Delta

  pwmOut = 0;             //Valores iniciais
  pwmRef = 0;             //Parado
  deltaTime = 0;
  refState = STANDSTILL;
  outState = STANDSTILL;
}

/**************************************************************************/
/*!
    @brief  Sets the new target speed for the motor

    @param[in]  pwmRef
                The new target speed
*/
/**************************************************************************/
void SmoothDrive::setPwmRef(int pwmRef){
    //Melhorar com throw exception quando pwmRef > VMAX
    this->pwmRef = pwmRef;

    if(pwmOut == pwmRef)
      refState = STANDSTILL;
    else if(pwmOut == 0)
      refState = ACCELERATE;
    else if(pwmRef == 0)
      refState = DEACCELERATE;
    else if(pwmOut > 0 && pwmRef > 0){
        if(pwmRef > pwmOut)
            refState = ACCELERATE;
        else
            refState = DEACCELERATE;
    }else if(pwmOut < 0 && pwmRef < 0){
        if(pwmRef > pwmOut)
            refState = DEACCELERATE;
        else
            refState = ACCELERATE;
    }else //Sinais diferentes
      refState = CHANGESIGNALD;

    if(!((outState == DEACCELERATE && refState == CHANGESIGNALD) ||
      (refState == DEACCELERATE && outState == CHANGESIGNALD) ||
      outState == refState)){

          if(refState == ACCELERATE)
              calculateDeltaTime(abs(pwmOut), accelA2, accelA2PWR, accelA3TWO,
                                                                   accelA3FOUR);
          else if(refState != STANDSTILL)
              calculateDeltaTime((VMAX-abs(pwmOut)), deaccA2, deaccA2PWR,
                                                       deaccA3TWO, deaccA3FOUR);

        timeInit = millis() - deltaTime;
    }
    outState = refState;
}

/**************************************************************************/
/*!
    @brief  Gets the actual target speed

    @return  pwmRef
             The actual target speed
*/
/**************************************************************************/
int SmoothDrive::getPwmRef(){
    return pwmRef;
}

/**************************************************************************/
/*!
    @brief  Gets the actual speed applyable to the motor

    @return  pwmOut
             The PWM value to be written to the motor
*/
/**************************************************************************/
int SmoothDrive::getPwmOut(){
  if(outState == CHANGESIGNALD && (millis()-timeInit) >= (timeDeacc-deltaTime)){
    outState = ACCELERATE;
    //Se houver erro está aqui
    timeInit += (timeDeacc - deltaTime);
  }

  if(outState == ACCELERATE){
    if(pwmRef < 0)
      pwmOut = -1 * calculateOut(0, accelA2, -accelA3);
    else
      pwmOut = calculateOut(0, accelA2, -accelA3);
  }else if(outState == DEACCELERATE){
    if(pwmOut < 0)
      pwmOut = -1 * calculateOut(VMAX, -deaccA2, deaccA3);
    else
      pwmOut = calculateOut(VMAX, -deaccA2, deaccA3);
  }else if(outState == CHANGESIGNALD)
    if(pwmRef < 0)
      pwmOut = calculateOut(VMAX, -deaccA2, deaccA3);
    else
      pwmOut = -1 * calculateOut(VMAX, -deaccA2, deaccA3);

  if((millis()-timeInit >= timeAccel && outState == ACCELERATE) ||
    (millis()-timeInit >= timeDeacc  && outState != STANDSTILL && outState != ACCELERATE) ||
    (pwmOut >= pwmRef && pwmRef > 0 && outState == ACCELERATE) ||
    (pwmOut <= pwmRef && pwmRef > 0 && outState == DEACCELERATE) ||
    (pwmOut >= pwmRef && pwmRef < 0 && outState == DEACCELERATE) ||
    (pwmOut <= pwmRef && pwmRef < 0 && outState == ACCELERATE)){
    outState = STANDSTILL;
    pwmOut = pwmRef;
  }


  return pwmOut;

}


/**************************************************************************/
/*!
    @brief  Sets the time it would have taken to get to the actual position

    @param[in]  pwmEqu
                pwmOut when accelerating
                VMAX-pwmOut when deaccelerating
*/
/**************************************************************************/
void SmoothDrive::calculateDeltaTime(unsigned int pwmEqu, double a2,
                                     double a2pwr, double a3two, double a3four){
    deltaTime = ((a2-sqrt(a2pwr-a3four*(double)pwmEqu))/a3two)*1000.0;
}

/**************************************************************************/
/*!
    @brief  Sets the pwmOut desired

    @param[in]  speedInit
                0 when accelerating
                VMAX when deaccelerating

    @param[in]  a2
                A2 when accelerating
                -A2 when deaccelerating

    @param[in]  a3
                -A3 when accelerating
                A3 when deaccelerating
*/
/**************************************************************************/
int SmoothDrive::calculateOut(unsigned int speedInit, double a2, double a3){
    timeRef = (double)(millis()-timeInit)/1000.0;
    return (int)((double)speedInit + a2*timeRef + a3*timeRef*timeRef);
}
