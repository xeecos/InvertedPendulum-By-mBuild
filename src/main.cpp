#include <Arduino.h>
#include "mBuild.h"
mSlider slider;
mDCMotor motor;
void setup()
{
    mBuild::init();
    mBuild::broadcast();
    slider.begin(0x1);
    motor.begin(0x2);
}
void loop()
{
    motor.setPower(slider.getSync());
    delay(20);
}