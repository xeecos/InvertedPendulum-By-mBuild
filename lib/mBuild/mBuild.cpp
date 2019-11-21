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
 * SLIDER
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