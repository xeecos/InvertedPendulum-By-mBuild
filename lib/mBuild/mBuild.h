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
        static void broadcast();
        void setup(uint8_t idx,uint8_t service,uint8_t subservice);
        void request(uint8_t cmd,uint8_t* data,uint8_t datalen,void(*callback)(PackData*));
        void call(uint8_t cmd,uint8_t* data,uint8_t datalen);
        void resp(PackData*pack);
    protected:
        uint8_t _idx;
        uint8_t _service;
        uint8_t _subservice;
        void (*_callback)(PackData*);
        long _value;
        bool _isAvailable;
        long readValue();
        bool available();
};
class mDCMotor:public mBuild
{
    public:
        void begin(uint8_t idx);
        void setPower(int8_t power);
};

class mSlider:public mBuild
{
    public:
        void begin(uint8_t idx);
        void get(void(*callback)(PackData*));
        uint8_t getSync();
};
#endif