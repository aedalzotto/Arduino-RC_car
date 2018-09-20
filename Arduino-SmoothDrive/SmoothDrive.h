//HEADER AQUI, POR FAVOR
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class SmoothDrive{
public:
  SmoothDrive(unsigned long long int timeAccel,
                          unsigned long long int timeDeacc, unsigned int VMAX);
  void setPwmRef(int pwmRef);
  int getPwmRef(void);
  int getPwmOut(void);

private:
  unsigned int VMAX;
  unsigned long long int timeAccel;
  unsigned long long int timeDeacc;
  double timeSecsAccel, timeSecsDeacc;
  double accelA2, accelA3, deaccA2, deaccA3;
  double accelA2PWR, accelA3FOUR, accelA3TWO;
  double deaccA2PWR, deaccA3FOUR, deaccA3TWO;
  int pwmRef, pwmOut;
  char refState;
  char outState;
  unsigned long long int deltaTime;
  unsigned long long int timeInit;
  double timeRef;

  int calculateOut(unsigned int speedInit, double a2, double a3);
  void calculateDeltaTime(unsigned int pwmEqu, double a2,
                                    double a2pwr, double a3two, double a3four);
};
