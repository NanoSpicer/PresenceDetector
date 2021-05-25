/**
 * @author Miquel Àngel Román
 */
#include <Arduino.h>
#include <ArduinoBLE.h>

/**
 * Blocks the program if condition evaluates to false or 0.
 * Prints out 
 * @param precondition
 */
void assert(int precondition, String message);

/**
 * Logs @param msg into the Serial object if logs are enabled
 */ 
void log(String msg);


/**
 * Performs @param s.begin() at @param baudrate
 * And awaits it's ready.
 */ 
void awaitSerialReady(Serial_ s, int baudrate);

/**
 * Formats a given device into a JSON arduino::String type
 * @param d The BLEDevice that is going to be formatted.
 * @returns an arduino::String with nicely formatted JSON.
 */ 
String bleAsJSON(BLEDevice d);

/**
 * Formats an IPAddress object into a human-readable String
 * @param addr The IPAddress object to be formated
 * @returns an arduino::String that looks something like: "192.168.1.1"
 */
String ipAddrAsString(IPAddress addr);
