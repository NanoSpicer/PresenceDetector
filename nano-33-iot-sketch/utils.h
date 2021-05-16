#include <Arduino.h>
#include <ArduinoBLE.h>

/**
 * Blocks the program if condition evaluates to false or 0.
 * Prints out 
 * @param precondition
 */
void assert(int precondition, String message);

/**
 * Logs @param msg into the Serial object
 */ 
void log(String msg);


/**
 * Performs @param s.begin() at @param baudrate
 * And awaits it's ready.
 */ 
void awaitSerialReady(Serial_ s, int baudrate);

String bleAsJSON(BLEDevice d);

String ipAddrAsString(IPAddress addr);
