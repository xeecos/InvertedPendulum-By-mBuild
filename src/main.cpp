#include <Arduino.h>
#include "mBleUart.h"
#include "mBuild.h"
mDualRGBColor dualRGB;
void colorHandle(DualRGBColor color){
    String msg = "color:";
    msg += color.r1;
    msg += " ";
    msg += color.g1;
    msg += " ";
    msg += color.b1;
    msg += " ";
    msg += color.r2;
    msg += " ";
    msg += color.g2;
    msg += " ";
    msg += color.b2;
    msg += " ";
    msg += " freeMemory:";
    msg += ESP.getFreeHeap();
    mBuild::log(msg);
}
void setup()
{
    mBuild::init();
    mBuild::broadcast();
    delay(1000);
    dualRGB.begin(0x1);
}
void loop()
{
    dualRGB.getRGB(&colorHandle);
    delay(1000);
}