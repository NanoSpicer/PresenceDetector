#include "./utils.h"
#include <WiFiNINA.h>
#include <PubSubClient.h>

/**
 * A utility class that nicely wraps any logic related to connecting to WiFi and the MQTT broker over TLS.
 * 
 * !! Warning !!
 * This utility assumes the connection is going to work over TLS.
 * Before using this utility make sure you've flashed the CA root certificate on the WiFi module or nothing will work.
 * 
 * Take note that if your MQTT broker does not have a domain name, you should fill the "Common Name" field with the IPv4 address of your broker.
 * 
 * Since there's no use to have more than one instance for this class it has been set up as a Singleton class.
 * 
 * @author Miquel Àngel Román
 */
class PresenceReporter
{
  public:
    /**
     * It connects to the WiFi network and the MQTT server. 
     * If either is not available it's going to retry until it succeeds.
     */ 
    void connect();

    /**
     * Publishes a certain message on a MQTT topic
     * @param payload A C string containing the message we want to send to the MQTT topic
     * @param topic A C string containing the topic we want to publish our payload on
     * @returns whether the operation succeeded
     */
    bool publish(const char* topic, const char* payload);

    /**
     * Ensures the node has a connection on the WiFi network and the MQTT server
     */ 
    void ensureConnected();

    /**
     * Connects to the WiFi network using the specified parameters on the static PresenceReporter::instance method
     */
    void connectWifi();

    /**
     * Connects to the MQTT broker using the specified parameters on the static PresenceReporter::instance method
     * @returns whether the operation succeeded
     */
    bool connectMQTT();

    /**
     * Tries to connect to the WiFi network using the specified parameters on the static PresenceReporter::instance method
     * @returns whether a connection was stablished
     */
    bool reconnectWifi();

    /**
     * Tries to connect to the MQTT broker using the specified parameters on the static PresenceReporter::instance method
     * @returns whether a connection was stablished
     */
    bool reconnectMQTT();

    /**
     * Disconnects from any MQTT broker and proceeds to disconnect from any WiFi network the node was connected to
     */ 
    void disconnect();

    /**
     * Utility method that ensures that one and only one instance of this class ever exists.
     * 
     * @param wifiSSID C string containing the WiFi network identifier
     * @param wifiPWD C string containing the WiFi's password
     * @param mqttServer C string containing an IPv4 pointing to the MQTT server
     * @param mqttUsername C string with the username assigned to this particular node
     * @param mqttPassword C string with the password assigned to this particular node
     * @param port The socket port where the MQTT broker is operating on. Typically 1883 or 8883 although it depends on your server
     * @returns an instance of the PresenceReporter, creating one first if it didn't exist.
     * 
     */
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

    WiFiSSLClient wifiClient;
    PubSubClient mqtt;

    
};
