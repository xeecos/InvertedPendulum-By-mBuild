#include "mBuild.h"
#include "mSerial.h"

mBuild::mBuild()
{
}

void mBuild::init()
{
    mSerial::shared();
}

void mBuild::broadcast(void(*callback)(PackData*))
{
    PackData *pack = new PackData();
    pack->idx = 0xff;
    pack->service = 0x10;
    pack->subservice = 0x0;
    pack->cmd = 0x0;
    pack->datalen = 0;
    mSerial::shared()->request(pack,callback);
}

void mBuild::begin(uint8_t idx,uint8_t service,uint8_t subservice)
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
    mSerial::shared()->request(pack,callback);
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

void mBuild::log(String msg)
{
    mSerial::shared()->log(msg);
}