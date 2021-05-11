#include "./utils.h"
#include <WiFiNINA.h>
#include <PubSubClient.h>


class PresenceReporter
{
  public:
    void connect();
    bool publish(const char* topic, const char* payload);

    void ensureConnected();
    void connectWifi();
    bool connectMQTT();
    bool reconnectWifi();
    bool reconnectMQTT();

    void disconnect();
    
    static PresenceReporter* instance(
      const char* wifiSSID, const char* wifiPWD, 
      const char* mqttServer, const char* mqttUsername, const char* mqttPassword,
      unsigned int port
    );
  private:
      static PresenceReporter* _instance;
      PresenceReporter(
          const char* wifiSSID, const char* wifiPWD,
          const char* mqttServer, const char* mqttUsername, const char* mqttPassword,
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
    const char* wifiSSID;
    const char* wifiPWD;
    const char* mqttServer;
    const char* mqttUsername; 
    const char* mqttPassword;
    unsigned int mqttPort;

    WiFiClient wifiClient;
    PubSubClient mqtt;

    
};
