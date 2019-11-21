#include <Arduino.h>
#include "mBleUart.h"
#include "mBuild.h"
mSlider slider;
mDCMotor motor;
mBuild angle;
struct PackData;
void cb(std::string msg)
{
    String m = "";
    m+=msg.c_str();
    mBuild::log(m);
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
    mBleUart::shared()->begin("hello");
    mBleUart::shared()->onReceived(&cb);
    slider.begin(0x1);
    motor.begin(0x2);
    // angle.begin(0x1,0x64,0xe);
}
void loop()
{
    motor.setPower(slider.getSync());
    delay(100);
}