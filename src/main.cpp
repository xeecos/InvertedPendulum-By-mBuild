#include <Arduino.h>
#include "mBleUart.h"
#include "mBuild.h"
mAngle angle;
mLED8x16 ledmatrix;
void cb(std::string msg)
{
    String m = "";
    m+=msg.c_str();
    m += " freeMemory:";
    m += ESP.getFreeHeap();
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
    delay(1000);
    angle.begin(0x1);
    ledmatrix.begin(0x2);
}
void loop()
{
    String msg = "";
    msg+=angle.getSync();
    ledmatrix.setText(msg);
    delay(100);
}