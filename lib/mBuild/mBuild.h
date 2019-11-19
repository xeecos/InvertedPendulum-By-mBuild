#ifndef __MBUILD__
#define __MBUILD__
#include "mSerial.h"
class mSerial;
struct PackData
{
    uint8_t header = 0xf0;
    uint8_t idx;
    uint8_t service;
    uint8_t subservice;
    uint8_t cmd;
    uint8_t datalen;
    uint8_t* data;
    long value;
    uint8_t checksum;
    uint8_t footer = 0xf7;
};
class mBuild
{
    public:
        mBuild();
        static void log(String msg);
        static void init();
        static void broadcast(void (*callback)(PackData*));
        void begin(uint8_t idx,uint8_t service,uint8_t subservice);
        void request(uint8_t cmd,uint8_t* data,uint8_t datalen,void(*callback)(PackData*));
        void call(uint8_t cmd,uint8_t* data,uint8_t datalen);
    private:
        uint8_t _idx;
        uint8_t _service;
        uint8_t _subservice;
};
#endif