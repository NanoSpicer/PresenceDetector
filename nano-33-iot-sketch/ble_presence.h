/**
 * @author Miquel Àngel Román
 */ 
#include "./utils.h"
#include <ArduinoBLE.h>

/**
 * Initializes the presence detection mechanism. It connects
 * @param eventHandler a callback or function reference that is going to be called 
 * whenever a new BLEDevice has been discovered.
 */ 
void ble_start(BLEDeviceEventHandler eventHandler);

/**
 * Polls the BLE sensor for new devices.
 * New scanned devices will be notified through the callback provided when ble_start was called.
 */ 
void ble_loop();

/**
 * Stops any scanning that was going on and shuts down the BLE Antenna.
 */ 
void ble_stop();
