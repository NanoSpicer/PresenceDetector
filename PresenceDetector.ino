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

// Operation modes
#define MODE_SCANNING 0
#define MODE_REPORTING 1

void uploadPresenceReport();
void detectPresence();


std::set<BLEDevice> scannedDevices;
PresenceReporter* reporter = NULL;

unsigned int OPERATION_MODE = MODE_SCANNING;

void setup()
{
    awaitSerialReady(Serial, BAUD_RATE);
    reporter = PresenceReporter::instance(
        ssid,
        password,
        mqttServer,
        mqttUsername,
        mqttPassword,
        mqttPort
    );
    delay(100);
    log("presence detector is running...");
}

void loop()
{
    if (OPERATION_MODE == MODE_SCANNING)
    {
        detectPresence();
    }
    else
    {
        uploadPresenceReport();
        // Go back to scanning mode
        OPERATION_MODE = MODE_SCANNING;
    }
}

void detectPresence()
{
    log("Starting BLE scan cycle...");
    ble_start([](BLEDevice peri) {
        scannedDevices.insert(peri);
    });
    Timer switchToReportingMode(SECONDS(10), [](){
        OPERATION_MODE = MODE_REPORTING;
    });
    switchToReportingMode.start();
    // Await 10 seconds have passed
    while(OPERATION_MODE == MODE_SCANNING) {
        ble_loop();
        switchToReportingMode.tick();
    }
    ble_stop();
    log("BLE scan cycle finished!");
}


void uploadPresenceReport()
{
    reporter->connect();
    reporter->ensureConnected();

    auto totalDevices = String(scannedDevices.size(), DEC);
    String msg = 
      "Detected " 
      + totalDevices
      + " devices during the past " 
      + String(SCAN_INTERVAL, DEC) + " seconds";
    log(msg);

    reporter->publish(mqttTopic, totalDevices);

    reporter->disconnect();
    

    #ifdef LOG_DEVICES 
    for (auto device : scannedDevices) {
      log(bleAsJSON(device));
    }
    #endif 
    scannedDevices.clear();
}
