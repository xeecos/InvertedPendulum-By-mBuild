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

union Bytes2Long
{
    uint64_t value;
    unsigned char bytes[8];
};
union Bytes2Short
{
    int16_t value;
    unsigned char bytes[2];
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
        bool _isAvailable;
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
        void get(void(*callback)(uint8_t value));
        uint8_t getSync();
        void response(PackData*pack) override
        {
            _value = pack->data[1];
            if(respValue!=NULL)respValue(_value);
        }
    private:
        void (*respValue)(uint8_t value);
        uint8_t _value;
};

class mAngle:public mBuild
{
    public:
        void begin(uint8_t idx);
        void get(void(*callback)(long));
        long getSync();
        void response(PackData*pack) override
        {
            Bytes2Long tran;
            tran.bytes[0]=(pack->data[1]&0x7f)+((pack->data[2]<<7)&0xf0);
            tran.bytes[1]=((pack->data[2]>>1)&0x7f)+((pack->data[3]<<6)&0xf0);
            tran.bytes[2]=((pack->data[3]>>2)&0x7f)+((pack->data[4]<<5)&0xf0);
            tran.bytes[3]=((pack->data[4]>>3)&0x7f)+((pack->data[5]<<4)&0xf0);
            tran.bytes[4] = tran.bytes[5] = tran.bytes[6] = tran.bytes[7] = 0;
            _value = tran.value;
            if(respValue!=NULL)respValue(_value);
        }
    private:
        void (*respValue)(long);
        long _value;
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
            switch(pack->data[0]){//cmd
                case 0x3:
                {
                    _color.adc1 = (pack->data[1]&0x7f)+((pack->data[2]<<7)&0xf0);
                    _color.adc2 = (pack->data[3]&0x7f)+((pack->data[4]<<7)&0xf0);
                }
                break;
                case 0x9:
                {
                    _color.evm1 = (pack->data[1]&0x7f)+((pack->data[2]<<7)&0xf0);
                    _color.evm2 = (pack->data[3]&0x7f)+((pack->data[4]<<7)&0xf0);
                }
                break;
                case 0xa:{
                    _color.ref1 = (pack->data[1]&0x7f)+((pack->data[2]<<7)&0xf0);
                    _color.ref2 = (pack->data[3]&0x7f)+((pack->data[4]<<7)&0xf0);
                }
                break;
                case 0x8:{
                    _color.r1 = (pack->data[1]&0x7f)+((pack->data[2]<<7)&0xf0);
                    _color.g1 = (pack->data[3]&0x7f)+((pack->data[4]<<7)&0xf0);
                    _color.b1 = (pack->data[5]&0x7f)+((pack->data[6]<<7)&0xf0);
                    _color.r2 = (pack->data[7]&0x7f)+((pack->data[8]<<7)&0xf0);
                    _color.g2 = (pack->data[9]&0x7f)+((pack->data[10]<<7)&0xf0);
                    _color.b2 = (pack->data[11]&0x7f)+((pack->data[12]<<7)&0xf0);
                }
                break;
            }
            if(respColor!=NULL)respColor(_color);
        };
    private:
        DualRGBColor _color;
        void (*respColor)(DualRGBColor color);
};
#endif