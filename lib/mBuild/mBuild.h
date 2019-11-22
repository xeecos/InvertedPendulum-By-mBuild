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
class mRGBStrip:public mBuild
{
    public:
        void begin(uint8_t idx);
        void setColor(uint8_t index,uint8_t red, uint8_t green, uint8_t blue);
        void setColor(uint8_t index,uint32_t color);
};
class mLED8x16:public mBuild
{
    public:
        void begin(uint8_t idx);
        void setPixels(uint8_t*pixels);
        void setPixel(uint8_t x, uint8_t y, uint8_t on);
        void invertPixel(uint8_t x, uint8_t y);
        void setText(String text);
        void setText(uint8_t x, uint8_t y,String text);
        void clear();
};
class mSlider:public mBuild
{
    public:
        void begin(uint8_t idx);
        void get(void(*callback)(PackData*));
        uint8_t getSync();
};

class mAngle:public mBuild
{
    public:
        void begin(uint8_t idx);
        void get(void(*callback)(PackData*));
        uint8_t getSync();
};
#endif