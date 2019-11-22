#include <Arduino.h>
#include "mBuild.h"
mAngle angle;
mLED8x16 ledmatrix;
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