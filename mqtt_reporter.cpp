#include "./mqtt_reporter.h"

PresenceReporter* PresenceReporter::_instance = nullptr;

bool PresenceReporter::publish(const char* topic, const char* payload)
{
    auto result = mqtt.publish(topic, payload);
    log(String((result ? "[SUCCESS]" : "[FAILURE]"))+" - " +String(mqttUsername)+" -> published '" +String(payload)+"' ON '"+String(topic)+"'");
    return result;
}

void PresenceReporter::connect()
{
    connectWifi();
    connectMQTT();
}

void PresenceReporter::disconnect()
{
    mqtt.disconnect();
    WiFi.end();
}

void PresenceReporter::ensureConnected()
{
    
    while(true)
    {
      bool isWifiConnected = WiFi.status() == WL_CONNECTED;

      if(!isWifiConnected){
          log("Fault detected: WiFi disconnected: reconnecting...");
          reconnectWifi();
      }
        
      bool isMqttConnected = mqtt.connected();

      if(!isMqttConnected) {
          log("Fault detected: MQTT disconnected: reconnecting...");
          reconnectMQTT();
      }

      // refresh variables after "fix" attempts
      isWifiConnected = WiFi.status() == WL_CONNECTED;
      isMqttConnected = mqtt.connected();
      // If both connected, break out
      
      if(isWifiConnected && isMqttConnected) {
          return;
      } else {
        log("Wifi connected " + String(isWifiConnected? "true" : "false"));
        log("Mqtt connected " + String(isMqttConnected? "true" : "false"));
      }
      delay(5000);
    }
}

void PresenceReporter::connectWifi() 
{
    auto wifi = WiFi;
    auto ssid = this->wifiSSID;
    auto wpwd = this->wifiPWD;
    log("WiFi: connecting to " + String(ssid));


    bool isConnected = false;

    do {
      isConnected = wifi.begin(ssid, wpwd) == WL_CONNECTED;
      if(!isConnected) {
        Serial.print(".");
        delay(500);
      }
    }while(!isConnected);
    
    randomSeed(micros());
    log("Success! WiFi connected: " + ipAddrAsString(wifi.localIP()));    
}

bool PresenceReporter::reconnectWifi()
{
    if(WiFi.status() != WL_CONNECTED) {
        connectWifi();
    }

    return true;
}

bool PresenceReporter::connectMQTT()
{
    mqtt.setClient(wifiClient);
    mqtt.setServer(mqttServer, mqttPort);
    return mqtt.connect(mqttUsername);
}

bool PresenceReporter::reconnectMQTT()
{
    while(!mqtt.connected()) 
    {
        if(!mqtt.connect(mqttUsername)){
          delay(5000);
        }
    }
    return mqtt.connected();
}

PresenceReporter* PresenceReporter::instance(
    const char* wifiSSID, const char* wifiPWD,
    const char* mqttServer, const char* mqttUsername, const char* mqttPassword, unsigned int port)
{
    if(!_instance) {
        PresenceReporter::_instance = new PresenceReporter(
          wifiSSID, wifiPWD, 
          mqttServer, mqttUsername, mqttPassword, port
        );
    }
    return PresenceReporter::_instance;
}
