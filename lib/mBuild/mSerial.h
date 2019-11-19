#ifndef __MSERIAL__
#define __MSERIAL__
#include <Arduino.h>
#include "mBuild.h"
#define DEBUG 0
class mBuild;
struct PackData;
struct CallbackData
{
    uint8_t idx;
    unsigned long time;
    void(*callback)(PackData*);
};
union BytesTran
{
    unsigned long longValue;
    unsigned char bytes[4];
};
class mSerial
{
    public:
        static mSerial* shared();
        mSerial();
        void request(PackData* pack, void(*callback)(PackData*));
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