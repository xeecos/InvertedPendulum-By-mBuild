#include "mBuild.h"
#include "mSerial.h"

mBuild::mBuild()
{
    _isAvailable = false;
}

void mBuild::init()
{
    mSerial::shared();
}

void mBuild::broadcast()
{
    PackData *pack = new PackData();
    pack->idx = 0xff;
    pack->service = 0x10;
    pack->subservice = 0x0;
    pack->cmd = 0x0;
    pack->datalen = 0;
    mSerial::shared()->call(pack);
}

void mBuild::setup(uint8_t idx,uint8_t service,uint8_t subservice)
{
    _idx = idx;
    _service = service;
    _subservice = subservice;
}

void mBuild::request(uint8_t cmd,uint8_t* data,uint8_t datalen)
{
    PackData *pack = new PackData();
    pack->idx = _idx;
    pack->service = _service;
    pack->subservice = _subservice;
    pack->cmd = cmd;
    pack->datalen = datalen;
    if(datalen>0){
        pack->data = (uint8_t*)malloc(datalen);
        for(int i=0;i<datalen;i++){
            pack->data[i] = data[i];
        }
    }
    mSerial::shared()->request(pack,this);
}

void mBuild::call(uint8_t cmd,uint8_t* data,uint8_t datalen)
{
    PackData *pack = new PackData();
    pack->idx = _idx;
    pack->service = _service;
    pack->subservice = _subservice;
    pack->cmd = cmd;
    pack->datalen = datalen;
    if(datalen>0){
        pack->data = (uint8_t*)malloc(datalen);
        for(int i=0;i<datalen;i++){
            pack->data[i] = data[i];
        }
    }
    mSerial::shared()->call(pack);
}

void mBuild::resp(PackData*pack)
{
    response(pack);
    _isAvailable = true;
}

bool mBuild::available()
{   
    return _isAvailable;
}
void mBuild::log(String msg)
{
    mSerial::shared()->log(msg);
}
/**
 * 
 * DC MOTOR
 * 
 * */
void mDCMotor::begin(uint8_t idx)
{
    mBuild::setup(idx,0x62,0x09);
}

void mDCMotor::setPower(int8_t power)
{
    uint8_t*data = (uint8_t*)malloc(2);
    uint8_t p = power<0?power+0xff:power;
    data[0] = p&0x7f;
    data[1] = (p>>7)&0x7f;
    mBuild::call(0x1,data,2);
    free(data);
}
/**
 * 
 * RGB LED
 * 
 * */
void mRGBStrip::begin(uint8_t idx)
{
    mBuild::setup(idx,0x65,0x08);
}

void mRGBStrip::setColor(uint8_t index,uint8_t red, uint8_t green,uint8_t blue)
{
    const uint8_t count = 7;
    uint8_t*data = (uint8_t*)malloc(count);
    data[0] = index;
    data[1] = red&0x7f;
    data[2] = (red>>7)&0x7f;
    data[3] = green&0x7f;
    data[4] = (green>>7)&0x7f;
    data[5] = blue&0x7f;
    data[6] = (blue>>7)&0x7f;
    mBuild::call(0x1,data,count);
    free(data);
}

void mRGBStrip::setColor(uint8_t index,uint32_t color)
{
    setColor(index,color>>16,color>>8,color&0xff);
}
/**
 * mLED8x16
 * 
 * */
void mLED8x16::begin(uint8_t idx)
{
    mBuild::setup(idx,0x65,0x09);
}
void mLED8x16::setPixels(uint8_t*pixels)
{
    uint8_t *data = (uint8_t*)malloc(32);
    for(int i=0;i<16;i++)
    {
        data[i*2] = pixels[i]&0x7f;
        data[i*2+1] = pixels[i]>>7;
    }
    mBuild::call(0x2,data,32);
    free(data);
}
void mLED8x16::setPixel(uint8_t x, uint8_t y, uint8_t on)
{
    uint8_t *data = (uint8_t*)malloc(3);
    data[0] = x;
    data[1] = y;
    data[2] = on&0x1;
    mBuild::call(0x1,data,3);
    free(data);
}
void mLED8x16::invertPixel(uint8_t x, uint8_t y)
{
    uint8_t *data = (uint8_t*)malloc(2);
    data[0] = x;
    data[1] = y;
    mBuild::call(0x5,data,2);
    free(data);
}
void mLED8x16::setText(String text)
{
    uint8_t count = text.length();
    uint8_t *data = (uint8_t*)malloc(count+2);
    data[0] = count&0x7f;
    data[1] = count>>7;
    memcpy(data+2,text.c_str(),count);
    mBuild::call(0x7,data,count+2);
    free(data);
}
void mLED8x16::setText(uint8_t x, uint8_t y,String text)
{
    uint8_t count = text.length();
    uint8_t *data = (uint8_t*)malloc(count+6);
    data[0] = x&0x7f;
    data[1] = x>>7;
    data[2] = y&0x7f;
    data[3] = y>>7;
    data[4] = count&0x7f;
    data[5] = count>>7;
    memcpy(data+6,text.c_str(),count);
    mBuild::call(0x6,data,count+6);
    free(data);
}
void mLED8x16::clear()
{
    mBuild::call(0x3,NULL,0);
}
/**
 * 
 * Slider Sensor
 * 
 * */

void mSlider::begin(uint8_t idx)
{
    mBuild::setup(idx,0x64,0x0d);
}

void mSlider::get(void(*callback)(uint8_t))
{
    respValue = callback;
    mBuild::request(0x1,NULL,0);
}

uint8_t mSlider::getSync()
{
    mBuild::request(0x1,NULL,0);
    while(available()){
        break;
    }
    return _value;
}

/**
 * 
 * Angle Sensor
 * 
 * */

void mAngle::begin(uint8_t idx)
{
    mBuild::setup(idx,0x64,0x0e);
}

void mAngle::get(void(*callback)(long))
{
    respValue = callback;
    mBuild::request(0x1,NULL,0);
}

long mAngle::getSync()
{
    mBuild::request(0x1,NULL,0);
    while(available()){
        break;
    }
    return _value;
}

/**
 * 
 * DualRGBColor Sensor
 * 
 * */
void mDualRGBColor::begin(uint8_t idx)
{
    mBuild::setup(idx,0x63,0x11);
}
void mDualRGBColor::getRGB(void(*callback)(DualRGBColor))
{
    respColor = callback;
    mBuild::request(0x8,NULL,0);
}
DualRGBColor mDualRGBColor::getRGBSync()
{
    mBuild::request(0x8,NULL,0);
    while(available()){
        break;
    }
    return _color;
}
void mDualRGBColor::getLight(void(*callback)(DualRGBColor))
{
    respColor = callback;
    mBuild::request(0x3,NULL,0);
}
DualRGBColor mDualRGBColor::getLightSync()
{
    mBuild::request(0x3,NULL,0);
    while(available()){
        break;
    }
    return _color;
}
void mDualRGBColor::getEvm(void(*callback)(DualRGBColor))
{
    respColor = callback;
    mBuild::request(0x9,NULL,0);
}
DualRGBColor mDualRGBColor::getEvmSync()
{
    mBuild::request(0x9,NULL,0);
    while(available()){
        break;
    }
    return _color;
}
void mDualRGBColor::getRef(void(*callback)(DualRGBColor))
{
    respColor = callback;
    mBuild::request(0xa,NULL,0);
}
DualRGBColor mDualRGBColor::getRefSync()
{
    mBuild::request(0xa,NULL,0);
    while(available()){
        break;
    }
    return _color;
}
/**
 * 
    switch(pack->service)
    {
        case 0x10:
        {
            pack->value = buffer.at(1);
            pack->idx = 0xff;
            pack->checksum = buffer.at(5);
            pack->footer = buffer.at(6);
        }
        break;
        case 0x63:
        {

            switch(pack->subservice)
            {
                case 0x11:
                {
                    pack->cmd = buffer.at(4);
                    switch(pack->cmd){
                        case 0x3:
                        case 0x9:
                        case 0xa:{
                            Bytes2Short tran;
                            tran.bytes[0] = (buffer.at(5)&0x7f)+((buffer.at(6)<<7)&0xf0);
                            tran.bytes[1] = (buffer.at(7)&0x7f)+((buffer.at(8)<<7)&0xf0);
                            pack->value = tran.value;
                            pack->checksum = buffer.at(9);
                            pack->footer = buffer.at(10);
                        }
                        break;
                        case 0x8:{
                            Bytes2Long tran;
                            tran.bytes[2] = (buffer.at(5)&0x7f)+((buffer.at(6)<<7)&0xf0);
                            tran.bytes[1] = (buffer.at(7)&0x7f)+((buffer.at(8)<<7)&0xf0);
                            tran.bytes[0] = (buffer.at(9)&0x7f)+((buffer.at(10)<<7)&0xf0);
                            tran.bytes[5] = (buffer.at(11)&0x7f)+((buffer.at(12)<<7)&0xf0);
                            tran.bytes[4] = (buffer.at(13)&0x7f)+((buffer.at(14)<<7)&0xf0);
                            tran.bytes[3] = (buffer.at(15)&0x7f)+((buffer.at(16)<<7)&0xf0);
                            tran.bytes[6] = tran.bytes[7] = 0;
                            pack->value = tran.value;
                            pack->checksum = buffer.at(17);
                            pack->footer = buffer.at(18);
                        }
                        break;
                    }
                }
                break;
            }
        }
        break;
        case 0x64:
        {
            switch(pack->subservice)
            {
                case 0xd:
                {
                }
                break;
                case 0xe:
                {
                    
                }
                break;
            }
        }
        break;
    }
    */