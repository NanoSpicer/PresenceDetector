/**
 * A presence detector using Bluetooth Low Energy and MQTT over TLS.
 * @author Miquel Àngel Román
 */
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


// Operation modes
#define MODE_SCANNING 0
#define MODE_REPORTING 1

void uploadPresenceReport();
void detectPresence();

// BLEDevice uses the MAC address to uniquely identify objects of that kind
// A std::set will only keep different entries (AKA different MAC addresses) on the data-structure.
std::set<BLEDevice> scannedDevices;
PresenceReporter* reporter = NULL;

unsigned int OPERATION_MODE = MODE_SCANNING;

void setup()
{
    // Init the Serial object
    awaitSerialReady(Serial, BAUD_RATE);
    // Create one PresenceReporter instance
    reporter = PresenceReporter::instance(
        WIFI_SSID,
        WIFI_PASSWORD,
        MQTT_SERVER_IP,
        MQTT_USERNAME,
        MQTT_PASSWORD,
        MQTT_PORT
    );
    log("presence detector is running...");
}

void loop()
{
    // If it's time to scan, then detect devices
    if (OPERATION_MODE == MODE_SCANNING)
    {
        detectPresence();
    }
    // When it's not time to scan, submit report and go back to scanning.
    else
    {
        uploadPresenceReport();
        // Go back to scanning mode
        OPERATION_MODE = MODE_SCANNING;
    }
}

/**
 * Polls the BLE sensor to get notified if there are BLE devices nearby
 */ 
void detectPresence()
{
    log("Starting BLE scan cycle...");
    // Start the BLE and pass a callback
    ble_start([](BLEDevice peri) {
        scannedDevices.insert(peri);
    });
    // Configure a timer that's going to run for SCAN_INTERVAL_SECONDS seconds
    Timer switchToReportingMode(SECONDS(SCAN_INTERVAL_SECONDS), []() {
        OPERATION_MODE = MODE_REPORTING;
    });

    // Start the timer and await 10 seconds have passed
    switchToReportingMode.start();
    
    while(OPERATION_MODE == MODE_SCANNING) {
        ble_loop(); // Polling the BLE
        switchToReportingMode.tick();
    }
    // Stop the BLE antenna
    ble_stop();
    log("BLE scan cycle finished!");
}

/**
 * Uploads the presence report to the MQTT server
 */ 
void uploadPresenceReport()
{
    // Connect to the network and the server
    reporter->connect();
    reporter->ensureConnected();

    // Get the total amount of DIFFERENT devices we've detected
    auto totalDevices = String(scannedDevices.size(), DEC);
    String msg = 
      "Detected " 
      + totalDevices
      + " devices during the past " 
      + String(SCAN_INTERVAL_SECONDS, DEC) + " seconds";
    log(msg);

    // Submit results
    reporter->publish(MQTT_TOPIC, totalDevices.c_str());
    // Disconnect from server and WiFi
    reporter->disconnect();
    

    // Debug purposes
    #ifdef LOG_DEVICES 
    for (auto device : scannedDevices) {
      log(bleAsJSON(device));
    }
    #endif 
    // Clear the set of scanned devices
    scannedDevices.clear();
}
