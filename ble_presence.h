#include "./utils.h"
#include <ArduinoBLE.h>

/**
 * Initializes the module
 */ 
void ble_setup(BLEDeviceEventHandler eventHandler);

/**
 * Polls the BLE sensor for new devices.
 * New scanned devices will be notified through the setup provided callback
 */ 
void ble_loop();
