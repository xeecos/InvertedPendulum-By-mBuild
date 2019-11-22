#ifndef __MSERIAL__
#define __MSERIAL__
#include <Arduino.h>
#include "mBuild.h"
#include "mBleUart.h"
#define DEBUG 0
class mBuild;
struct PackData;
union Bytes2Long
{
    unsigned long value;
    unsigned char bytes[4];
};
union Bytes2Short
{
    int16_t value;
    unsigned char bytes[2];
};

struct CallbackData
{
    uint8_t idx;
    unsigned long time;
    mBuild *module;
};
class mSerial
{
    public:
        static mSerial* shared();
        mSerial();
        void request(PackData* pack, mBuild*module);
        void call(PackData* pack);
        void log(String msg);
    private:
        void available();
        void write(uint8_t b);
        void parse();
        bool isExistCallback(uint8_t idx);
        static void* loop_thread(void *threadid);
        static mSerial* _instance;
        bool isReceiving;
        std::vector<uint8_t> buffer;
        HardwareSerial *_uart;
        HardwareSerial *_logger;
        std::vector<CallbackData*> _callbacks;
};
#endif