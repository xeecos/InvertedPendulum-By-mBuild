#include <Arduino.h>
#include "mBuild.h"
mBuild *slider;
void cb()
{
    mBuild::log("hello");
}
void setup()
{
    mBuild::init();
    mBuild::boardcast(&cb);
    // slider = new mBuild();
    // slider->begin(0x1,0x64,0xd);
}
void loop()
{
}
/**
 * 
 * 
HardwareSerial Logger(0);
HardwareSerial mBuild(2);
void sendCMD(uint8_t cmd){
    mBuild.write(cmd);
}
void runMotor(){
    sendCMD(0xF0);
    uint8_t sum = (0x01+0x62+0x09+0x01+0x32+0x00)&0x7f;
    Logger.println(sum,HEX);
    sendCMD(0x01);
    sendCMD(0x62);
    sendCMD(0x09);
    sendCMD(0x01);
    sendCMD(0x32);
    sendCMD(0x00);
    sendCMD(sum);
    sendCMD(0xF7);
}
void getSlider(){
    sendCMD(0xF0);
    uint8_t sum = (0x01+0x64+0x0d+0x01)&0x7f;
    Logger.println(sum,HEX);
    sendCMD(0x01);
    sendCMD(0x64);
    sendCMD(0x0d);
    sendCMD(0x01);
    sendCMD(sum);
    sendCMD(0xF7);
}
void genDevices(){
    sendCMD(0xF0);
    sendCMD(0xFF);
    sendCMD(0x10); 
    sendCMD(0x00);
    sendCMD(0x0F);
    sendCMD(0xF7);
}
*/