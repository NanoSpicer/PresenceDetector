#include "ble_presence.h"


void ble_start(BLEDeviceEventHandler eventHandler) 
{
    assert(BLE.begin(), "BLE initialization failed");
    BLE.setEventHandler(BLEDiscovered, eventHandler);
    assert(BLE.scan(true), "BLE scanning failed");
}

void ble_loop() 
{
    BLE.poll();
}

void ble_stop()
{
    BLE.stopScan();
}
