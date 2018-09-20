//HEADER AQUI, POR FAVOR
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class RelayRobot{
public:
  RelayRobot(uint8_t leftPwm, uint8_t leftDirL, uint8_t leftDirH,
  uint8_t rightPwm, uint8_t rightDirL, uint8_t rightDirH);
  void updateRobot(int leftSpeed, int rightSpeed);

private:
  uint8_t leftPwm, leftDirH, leftDirL;
  uint8_t rightPwm, rightDirH, rightDirL;
};
