#include <Arduino.h>
#include "mBuild.h"
mBuild slider;
mBuild motor;
mBuild angle;
struct PackData;
void cb(PackData *pack)
{
    
}
void sliderHandle(PackData *pack){
    String msg = "slider:";
    msg += pack->data[0];
    msg += " freeMemory:";
    msg += ESP.getFreeHeap();
    mBuild::log(msg);
    uint8_t*speed = (uint8_t*)malloc(2);
    speed[0] = pack->data[0];
    speed[1] = 0x0;
    motor.call(0x1,speed,2);
    free(speed);
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
    mBuild::broadcast(&cb);
    // slider.begin(0x1,0x64,0xd);
    // motor.begin(0x2,0x62,0x9);
    angle.begin(0x1,0x64,0xe);
    // uint8_t*data = (uint8_t*)malloc(1);
    // data[0] = 0x0;
    // angle.call(0x7f,data,1);
    // free(data);
}
void loop()
{
    // slider.request(0x1,NULL,0,&sliderHandle);
    angle.request(0x1,NULL,0,&angleHandle);
    delay(200);

}