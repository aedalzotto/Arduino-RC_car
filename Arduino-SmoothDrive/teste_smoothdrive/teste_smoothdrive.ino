#include <SmoothDrive.h>
#include <JoyBTCommander.h>

#define TF 2000
#define PWMMAX 255

SmoothDrive motor(TF, PWMMAX);
JoyBTCommander BT(9600, 2, 3);
int mySpeed;

void setup() {
  Serial.begin(115200);

}

void loop() {

  pwmLoop(-255);
  pwmLoop(255);
  pwmLoop(255);
  pwmLoop(255);

  while(1);
  
  

}

void pwmLoop(int target){
  int level;
  motor.setPwmRef(target);
  do{
    level = motor.getPwmOut();
    Serial.print(millis());
    Serial.print(' ');
    Serial.println(level);
    
    delay(100);
    if(level <= -200) return;
  }while(level != target);
}
