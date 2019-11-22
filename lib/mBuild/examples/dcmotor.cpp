#include <Arduino.h>
#include "mBuild.h"
mDCMotor motor;
int power = 0,dir=1;
void setup()
{
    mBuild::init();
    mBuild::broadcast();
    motor.begin(0x1);
}
void loop()
{
    power += dir;
    if(power>99){
        dir = -1;
    }else if(power<-99){
        dir = 1;
    }
    motor.setPower(power);
    delay(10);
}