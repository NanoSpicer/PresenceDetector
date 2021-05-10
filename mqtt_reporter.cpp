#include "./mqtt_reporter.h"

PresenceReporter* PresenceReporter::_instance = nullptr;

bool PresenceReporter::publish(String topic, String payload)
{
    auto result = mqtt.publish(topic.c_str(), payload.c_str());
    log(String((result ? "[SUCCESS]" : "[FAILURE]"))+" - " +mqttUsername+" -> published '" +payload+"' ON '"+topic+"'");
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
    log("WiFi: connecting to " + ssid);


    bool isConnected = false;

    do {
      isConnected = wifi.begin(ssid.c_str(), wpwd.c_str()) == WL_CONNECTED;
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
    mqtt.setServer(mqttServer.c_str(), mqttPort);
    return mqtt.connect(mqttUsername.c_str());
}

bool PresenceReporter::reconnectMQTT()
{
    while(!mqtt.connected()) 
    {
        if(!mqtt.connect(mqttUsername.c_str())){
          delay(5000);
        }
    }
    return mqtt.connected();
}

PresenceReporter* PresenceReporter::instance(
    String wifiSSID, String wifiPWD,
    String mqttServer, String mqttUsername, String mqttPassword, unsigned int port)
{
    if(!_instance) {
        PresenceReporter::_instance = new PresenceReporter(
          wifiSSID, wifiPWD, 
          mqttServer, mqttUsername, mqttPassword, port
        );
    }
    return PresenceReporter::_instance;
}
