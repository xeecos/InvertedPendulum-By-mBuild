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
    uint64_t value;
    uint8_t checksum;
    uint8_t footer = 0xf7;
};
struct DualRGBColor
{
    uint8_t r1;
    uint8_t g1;
    uint8_t b1;
    uint8_t r2;
    uint8_t g2;
    uint8_t b2;
    uint8_t adc1;
    uint8_t adc2;
    uint8_t evm1;
    uint8_t evm2;
    uint8_t ref1;
    uint8_t ref2;
};
class mBuild
{
    public:
        mBuild();
        static void log(String msg);
        static void init();
        static void broadcast();
        void setup(uint8_t idx,uint8_t service,uint8_t subservice);
        void request(uint8_t cmd,uint8_t* data,uint8_t datalen);
        void call(uint8_t cmd,uint8_t* data,uint8_t datalen);
        void resp(PackData*pack);
        virtual void response(PackData*pack){};
    protected:
        uint8_t _idx;
        uint8_t _service;
        uint8_t _subservice;
        void (*_callback)(PackData*);
        uint64_t _value;
        bool _isAvailable;
        uint64_t readValue();
        bool available();
};

/**
 * 
 * Drivers
 * 
 * */

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

/**
 * 
 * Sensors
 * 
 * */

class mSlider:public mBuild
{
    public:
        void begin(uint8_t idx);
        void get(void(*callback)(PackData*));
        uint8_t getSync();
        void response(PackData*pack) override
        {
            respValue(pack);
        }
    private:
        void (*respValue)(PackData*);
};

class mAngle:public mBuild
{
    public:
        void begin(uint8_t idx);
        void get(void(*callback)(PackData*));
        uint8_t getSync();
        void response(PackData*pack) override
        {
            respValue(pack);
        }
    private:
        void (*respValue)(PackData*);
};

class mDualRGBColor:public mBuild
{
    public:
        void begin(uint8_t idx);
        void getRGB(void(*callback)(DualRGBColor));
        DualRGBColor getRGBSync();
        void getLight(void(*callback)(DualRGBColor));
        DualRGBColor getLightSync();
        void getEvm(void(*callback)(DualRGBColor));
        DualRGBColor getEvmSync();
        void getRef(void(*callback)(DualRGBColor));
        DualRGBColor getRefSync();
        void response(PackData*pack) override
        {
            switch(pack->cmd){
                case 0x3:
                {
                    uint16_t v = pack->value;
                    _color.adc1 = v&0xff;
                    _color.adc2 = (v>>8)&0xff;
                }
                break;
                case 0x8:
                {
                    uint64_t v = pack->value;
                    uint32_t c1 = v&0xffffff;
                    uint32_t c2 = (v>>24)&0xffffff;
                    _color.r1 = (c1>>16)&0xff;
                    _color.g1 = (c1>>8)&0xff;
                    _color.b1 = c1&0xff;
                    _color.r2 = (c2>>16)&0xff;
                    _color.g2 = (c2>>8)&0xff;
                    _color.b2 = c2&0xff;
                }
                break;
                case 0x9:
                {
                    uint16_t v = pack->value;
                    _color.evm1 = v&0xff;
                    _color.evm2 = (v>>8)&0xff;
                }
                break;
                case 0xa:
                {
                    uint16_t v = pack->value;
                    _color.ref1 = v&0xff;
                    _color.ref2 = (v>>8)&0xff;
                }
                break;
            }
            respColor(_color);
        };
    private:
        DualRGBColor _color;
        void (*respColor)(DualRGBColor color);
};
#endif