#include "ble_presence.h"

/**
 * Initializes the module
 */
void ble_setup(BLEDeviceEventHandler eventHandler) 
{
    assert(BLE.begin(), "BLE initialization failed");
    BLE.setEventHandler(BLEDiscovered, eventHandler);
    assert(BLE.scan(true), "BLE scanning failed");
}

/**
 * Polls the BLE sensor for new devices.
 * New scanned devices will be notified through the setup provided callback
 */
void ble_loop() 
{
    BLE.poll();
}
