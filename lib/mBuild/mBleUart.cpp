#include "mBleUart.h"
#include <pthread.h>
mBleUart* mBleUart::_instance = 0;
mBleUart* mBleUart::shared()
{
    if (_instance == 0)
    {
        _instance = new mBleUart();
    }

    return _instance;
}

mBleUart::mBleUart()
{
}
void mBleUart::begin(std::string name)
{
    BLEDevice::init(name);
    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new mBleServerCallbacks());
    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);
    // Create a BLE Characteristic
    pTxCharacteristic = pService->createCharacteristic(
                                            CHARACTERISTIC_UUID_TX,
                                            BLECharacteristic::PROPERTY_NOTIFY
                                        );
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                                                CHARACTERISTIC_UUID_RX,
                                                BLECharacteristic::PROPERTY_WRITE
                                            );
    pRxCharacteristic->setCallbacks(new mBleCallbacks());
    // Start the service
    pService->start();
    // Start advertising
    pServer->getAdvertising()->start();

    pthread_t thread;
    pthread_create(&thread, NULL, loop_thread, (void*)1);
}
void mBleUart::send(uint8_t*buf,uint8_t count)
{
    if (deviceConnected) {
        pTxCharacteristic->setValue(buf, count);
        pTxCharacteristic->notify();
        delay(10);
	}
}
void mBleUart::onReceived(void(*callback)(std::string))
{
    _callback = callback;
}
void mBleUart::available()
{
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
void mBleUart::parse(std::string msg)
{
    if(_callback!=NULL){
        _callback(msg);
    }
}
void* mBleUart::loop_thread(void *threadid)
{
    mSerial::shared()->log("start ble thread!");
    while(true) {
        mBleUart::shared()->available();
        vTaskDelay(1);
    }
}