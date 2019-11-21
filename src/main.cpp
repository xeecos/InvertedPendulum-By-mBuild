#include <Arduino.h>
#include "mBuild.h"
mSlider slider;
mDCMotor motor;
mBuild angle;
struct PackData;
void cb(PackData *pack)
{
    
}
void sliderHandle(PackData *pack){
    String msg = "slider:";
    msg += pack->value;
    msg += " freeMemory:";
    msg += ESP.getFreeHeap();
    mBuild::log(msg);
}
void angleHandle(PackData *pack){
    String msg = "angle:";
    msg += pack->value;
    msg += " freeMemory:";
    msg += ESP.getFreeHeap();
    mBuild::log(msg);
}
void setup()
{
    mBuild::init();
    mBuild::broadcast();
    slider.begin(0x1);
    motor.begin(0x2);
    // angle.begin(0x1,0x64,0xe);
}
void loop()
{
    motor.setPower(slider.getSync());
    delay(100);
    // for(int i=0;i<7;i++){
    //     motor.setPower(i*10);
    //     delay(500);
    // }
    // delay(1000);
    // for(int i=0;i<7;i++){
    //     motor.setPower(60-i*10);
    //     delay(500);
    // }
    // motor.setPower(0);
    // delay(2000);
}