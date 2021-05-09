#include "./utils.h"
#include "./config.h"
#include "./timer.h"
#include "./ble_presence.h"
#include "./mqtt_reporter.h"
#include <set>

// Uncomment the below line if we want to see details of scanned devices
// #define LOG_DEVICES

#define BAUD_RATE 115200
#define SECONDS(number) number*1000L
// In seconds!
#define SCAN_INTERVAL 10

void uploadPresenceReport();
void scanBLE();
// TODOOOOO!!! WE CAN'T USE THE BLE AND WIFI AT THE SAME TIME!!!!
Timer presenceScanner(SECONDS(SCAN_INTERVAL), scanBLE);
Timer presenceReporter(SECONDS(SCAN_INTERVAL), uploadPresenceReport);
std::set<BLEDevice> scannedDevices;
PresenceReporter* reporter = NULL;

void setup()
{
    awaitSerialReady(Serial, BAUD_RATE);
    ble_setup(on_device_discovered);
    delay(100);
    
    /*reporter = PresenceReporter::instance(
        ssid,
        password,
        mqttServer,
        mqttUsername,
        mqttPassword,
        mqttPort
    );
    reporter->connect();*/
    delay(100);
    log("presence detector is running...");
    presenceReporter.start();
}

void loop()
{
    ble_loop();

    presenceReporter.tick();

    //reporter->ensureConnected();
}


void scanBLE() {
  
}


void uploadPresenceReport()
{
    auto totalDevices = String(scannedDevices.size(), DEC);
    String msg = 
      "Detected " 
      + totalDevices
      + " devices during the past " 
      + String(SCAN_INTERVAL, DEC) + " seconds";
    log(msg);

    //reporter->publish(mqttTopic, totalDevices);
    

    #ifdef LOG_DEVICES 
    for (auto device : scannedDevices) {
      log(bleAsJSON(device));
    }
    #endif 
    scannedDevices.clear();
}

void on_device_discovered(BLEDevice peripheral)
{
  scannedDevices.insert(peripheral);
}
