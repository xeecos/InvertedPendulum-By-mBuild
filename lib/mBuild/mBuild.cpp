#include "mBuild.h"
#include "mSerial.h"

mBuild::mBuild()
{
}

void mBuild::init()
{
    mSerial::shared();
}

void mBuild::boardcast(void(*callback)(void))
{
    PackData *data = new PackData();
    data->idx = 0xff;
    data->service = 0x10;
    data->subservice = 0x00;
    data->datalen = 0;
    data->checksum = 0x0f;
    mSerial::shared()->request(data,callback);
}

void mBuild::begin(uint8_t idx,uint8_t service,uint8_t subservice)
{

}


void mBuild::request(uint8_t cmd,uint8_t* data,void *callback)
{

}

void mBuild::call(uint8_t cmd,uint8_t* data)
{

}

void mBuild::log(String msg)
{
    mSerial::shared()->log(msg);
}