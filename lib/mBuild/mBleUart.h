#ifndef __MBLEUART__
#define __MBLEUART__
/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "mSerial.h"


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
class mBleUart
{
  public:
    static mBleUart* shared();
    mBleUart();
    void begin(std::string name);
    void send(uint8_t*buf,uint8_t count);
    void onReceived(void(*callback)(std::string));
    void parse(std::string msg);
    bool deviceConnected = false;
  private:
    void available();
    BLEServer *pServer = NULL;
    BLECharacteristic * pTxCharacteristic;
    bool oldDeviceConnected = false;
    uint8_t txValue = 0;
    void(*_callback)(std::string);
    static mBleUart* _instance;
    static void* loop_thread(void *threadid);
};
class mBleServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      mBleUart::shared()->deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      mBleUart::shared()->deviceConnected = false;
    }
};
class mBleCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        mBleUart::shared()->parse(rxValue);
      }
    }
};

#endif