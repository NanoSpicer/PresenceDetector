#include "./utils.h"
#include "./config.h"
#define __str(x) String(x)

void assert(int precondition, String message){
    if (!precondition)
    {
        Serial.println(message);
        while (1)
            ;
    }
}

void log(String msg) {
    if(SERIAL_LOGS) {
      Serial.println(msg);  
    }
    
}

void awaitSerialReady(Serial_ s, int baudrate){
    s.begin(baudrate);
    while(!s);
}

String asJSONString(String param) 
{
  return param.length() == 0 ? "null" : "\"" + param + "\"";
}

String ipAddrAsString(IPAddress addr) {
  return String(addr[0], DEC) + "." + String(addr[1], DEC)+ "." + String(addr[2], DEC)+ "." + String(addr[3], DEC);
}


String localNameAsJSON(BLEDevice d) 
{
  auto res = d.hasLocalName() ? d.localName() : "";
  return asJSONString(res);
}
String bleAsJSON(BLEDevice d) {
    String json = "{\n";
    String address = __str("  ") + __str("address: ") + asJSONString(d.address())+__str(",\n");
    String localName = __str("  ") + __str("localName: ") + localNameAsJSON(d)+__str(",\n");
    String rssi = __str("  ") + __str("rssi: ") + __str(d.rssi())+__str("\n}");

    return json + address + localName + rssi;
}
