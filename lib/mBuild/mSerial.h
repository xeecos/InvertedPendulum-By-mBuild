#ifndef __MSERIAL__
#define __MSERIAL__
#include <Arduino.h>
#include "mBuild.h"
class mBuild;
struct PackData;
struct CallbackData
{
    uint8_t idx;
    void(*callback)();
};
class mSerial
{
    public:
        static mSerial* shared();
        mSerial();
        void request(PackData * data, void(*callback)(void));
        void call(uint8_t*data);
        void log(String msg);
    private:
        static mSerial* _instance;
        HardwareSerial *_uart;
        HardwareSerial *_logger;
        std::vector<CallbackData> _callbacks;
};
#endif