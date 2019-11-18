#include "mSerial.h"
mSerial* mSerial::_instance = 0;
mSerial* mSerial::shared()
{
    if (_instance == 0)
    {
        _instance = new mSerial();
    }

    return _instance;
}

mSerial::mSerial()
{

    _logger = new HardwareSerial(0);
    _logger->begin(115200);
    _uart = new HardwareSerial(2);
    _uart->begin(115200,SERIAL_8N1,22,23);

}

void mSerial::request(PackData* data,void(*callback)(void))
{
    CallbackData cb;
    cb.idx = data->idx;
    cb.callback = callback;
    _callbacks.push_back(cb);
    _callbacks.at(0).callback();
}

void mSerial::call(uint8_t*data)
{

}

void mSerial::log(String msg)
{
    _logger->println(msg);
}