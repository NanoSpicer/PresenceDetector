#include "./utils.h"
#include <WiFiNINA.h>
#include <PubSubClient.h>


class PresenceReporter
{
  public:
    void connect();
    bool publish(String topic, String payload);

    void ensureConnected();
    void connectWifi();
    bool connectMQTT();
    bool reconnectWifi();
    bool reconnectMQTT();

    void disconnect();
    
    static PresenceReporter* instance(
      String wifiSSID, String wifiPWD, 
      String mqttServer, String mqttUsername, String mqttPassword,
      unsigned int port
    );
  private:
      static PresenceReporter* _instance;
      PresenceReporter(
          String wifiSSID, String wifiPWD,
          String mqttServer, String mqttUsername, String mqttPassword,
          unsigned int port
      )
      {
          this->wifiSSID = wifiSSID;
          this->wifiPWD = wifiPWD;
          this->mqttServer = mqttServer;
          this->mqttUsername = mqttUsername;
          this->mqttPassword = mqttPassword;
          this->mqttPort = port;
      }
    String wifiSSID;
    String wifiPWD;
    String mqttServer;
    String mqttUsername; 
    String mqttPassword;
    unsigned int mqttPort;

    WiFiClient wifiClient;
    PubSubClient mqtt;

    
};
