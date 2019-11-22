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

void mBuild::request(uint8_t cmd,uint8_t* data,uint8_t datalen,void(*callback)(PackData*))
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
    _callback = callback;
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
    _value = pack->value;
    _isAvailable = true;
    if(_callback!=NULL)
    {
        _callback(pack);
    }
}

long mBuild::readValue()
{
    _isAvailable = false;
    return _value;
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

void mSlider::get(void(*callback)(PackData*))
{
    mBuild::request(0x1,NULL,0,callback);
}

uint8_t mSlider::getSync()
{
    mBuild::request(0x1,NULL,0,NULL);
    while(available()){
        break;
    }
    return readValue();
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

void mAngle::get(void(*callback)(PackData*))
{
    mBuild::request(0x1,NULL,0,callback);
}

uint8_t mAngle::getSync()
{
    mBuild::request(0x1,NULL,0,NULL);
    while(available()){
        break;
    }
    return readValue();
}