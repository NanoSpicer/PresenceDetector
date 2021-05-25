# Presence Detection over TLS
Presence detection using Bluetooth Low Energy with Arduino Nano 33 IoT reporting to a mosquitto server with MQTT hosted on a Raspberry Pi 4 running Telegraf and Grafana.



## Arduino Nano 33 IoT

<img src="https://store-cdn.arduino.cc/uni/catalog/product/cache/1/image/500x375/f8876a31b63532bbba4e781c30024a0a/a/b/abx00027_iso_1.jpg">

### Dependencies
* ArduinoBLE.h (by Arduino) 1.2.0
* WiFiNINA.h (by Arduino) 1.8.9
* PubSubClient.h (by Nick O'Leary) 2.8.0

### Approach

The MCU uses an ESP32 module to have access to WiFi and Bluetooth Low Energy.

Since both operate on the 2.42GHz band, the same module re-uses the antenna to provide one or the other **BUT NEVER BOTH AT THE SAME TIME**.

To address this issue, this sketch assumes a 10 second device scanning cycle and afterwards it just reports the device count to the MQTT server.


Feel free to customize your sketch by providing your own `config.h` file with your own parameters.
