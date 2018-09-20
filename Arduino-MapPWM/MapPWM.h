//HEADER AQUI, POR FAVOR
//Contém trechos de código do INDIANO
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class MapPWM{
public:
  MapPWM(int joyMax, int joyMin, int offset, int minPwm=0, int maxPwm=255);
  void updateAxis(int axisX, int axisY);
  int getRightPwm();
  int getLeftPwm();

private:
  int joyMax, joyMin, joyMid;
  int upperOffset, lowerOffset;
  int maxPwm, minPwm;
  int rightPwm, leftPwm;
};
