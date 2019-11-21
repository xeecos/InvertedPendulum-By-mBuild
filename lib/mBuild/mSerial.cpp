#include "mSerial.h"
#include <pthread.h>
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
    isReceiving = false;
    pthread_t thread;
    pthread_create(&thread, NULL, loop_thread, (void*)0);
}


void mSerial::request(PackData *pack,mBuild*module)
{
    if(!isExistCallback(pack->idx)){
        CallbackData *cb = new CallbackData();
        cb->idx = pack->idx;
        cb->module = module;
        cb->time = millis();
        _callbacks.push_back(cb);
    }
    call(pack);
}
void mSerial::write(uint8_t b)
{
    _uart->write(b);
    if(DEBUG)_logger->println(b,HEX);
}
void mSerial::call(PackData* pack)
{
    write(pack->header);
    write(pack->idx);
    write(pack->service);
    write(pack->subservice);
    if(pack->cmd>0){
        write(pack->cmd);
    }
    pack->checksum = pack->idx+pack->service+pack->subservice+pack->cmd;
    for(int i=0,count=pack->datalen;i<count;i++)
    {
        write(pack->data[i]);
        pack->checksum+=pack->data[i];
    }
    pack->checksum &= 0x7f;
    write(pack->checksum);
    write(pack->footer);
    if(pack->datalen>0)free(pack->data);
    free(pack);
    
}

void mSerial::log(String msg)
{
    _logger->println(msg);
}

void mSerial::available()
{
    if(_uart->available()){
        uint8_t b = _uart->read();
        if(b==0xf0)isReceiving=true;
        if(isReceiving){
            if(DEBUG)_logger->println(b,HEX);
            buffer.push_back(b);
        }
        if(b==0xf7){
            isReceiving = false;
            parse();
        }
    }
}

void mSerial::parse()
{
    unsigned long time = millis();
    PackData *pack = new PackData();
    pack->header = buffer.at(0);
    pack->idx = buffer.at(1);
    pack->service = buffer.at(2);
    pack->subservice = buffer.at(3);
    pack->datalen = 0;
    switch(pack->service)
    {
        case 0x10:
        {
            pack->value = buffer.at(1);
            pack->idx = 0xff;
            pack->checksum = buffer.at(5);
            pack->footer = buffer.at(6);
        }
        break;
        case 0x64:
        {
            switch(pack->subservice)
            {
                case 0xd:
                {
                    pack->cmd = buffer.at(4);
                    pack->value = buffer.at(5);
                    pack->checksum = buffer.at(6);
                    pack->footer = buffer.at(7);
                }
                break;
                case 0xe:
                {
                    pack->cmd = buffer.at(4);
                    Bytes2Long tran;
                    tran.bytes[0]=(buffer.at(5)&0x7f)+((buffer.at(6)<<7)&0xf0);
                    tran.bytes[1]=((buffer.at(6)>>1)&0x7f)+((buffer.at(7)<<6)&0xf0);
                    tran.bytes[2]=((buffer.at(7)>>2)&0x7f)+((buffer.at(8)<<5)&0xf0);
                    tran.bytes[3]=((buffer.at(8)>>3)&0x7f)+((buffer.at(9)<<4)&0xf0);
                    pack->value = tran.value;
                    pack->checksum = buffer.at(10);
                    pack->footer = buffer.at(11);
                }
                break;
            }
        }
        break;
    }
    for(int i=0,count=_callbacks.size();i<count;i++)
    {
        if(_callbacks.at(i)->idx==pack->idx)
        {
            _callbacks.at(i)->module->resp(pack);
            free(_callbacks.at(i));
            _callbacks.erase(_callbacks.begin()+i);
            break;
        }
    }

    for(int i=0,count=_callbacks.size();i<count;i++)
    {
        int t = time-_callbacks.at(i)->time;
        if(t>1000)
        {
            free(_callbacks.at(i));
            _callbacks.erase(_callbacks.begin()+i);
            break;
        }
    }
    if(pack->datalen>0)free(pack->data);
    free(pack);
    buffer.clear();
}
bool mSerial::isExistCallback(uint8_t idx)
{
    for(int i=0,count=_callbacks.size();i<count;i++)
    {
        if(_callbacks.at(i)->idx==idx){
            return true;
        }
    }
    return false;
}
void* mSerial::loop_thread(void *threadid)
{
    mSerial::shared()->log("start thread!");
    while(true) {
        mSerial::shared()->available();
        vTaskDelay(1);
    }
}