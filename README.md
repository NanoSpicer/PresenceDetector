# Presence Detection over TLS
Presence detection using Bluetooth Low Energy with Arduino Nano 33 IoT reporting to a mosquitto server with MQTT hosted on a Raspberry Pi 4 running Telegraf and Grafana.



## Arduino Nano 33 IoT

<img src="https://store-cdn.arduino.cc/uni/catalog/product/cache/1/image/500x375/f8876a31b63532bbba4e781c30024a0a/a/b/abx00027_iso_1.jpg">

## Securing this application

 
### Using TLS
To ensure a high level of security of the application all established communication is encrypted with TLS
v1.2. This means that clients know who the server is and can verify that the server is who it claims
to be.
 
> It has not been possible to enable client verification by the server since the Arduino Nano node
33 IoT is a bit limited in this regard: it only allows you to install trusted root certificates inside the
communication chip and therefore it has not been possible to flash a client certificate to provide to the broker
IoT.
 
In this way the MQTT traffic circulates encrypted through the network and we are able to verify the
identity of our broker; protecting us from sniffing attacks and man in the middle.
 
### Disable log messages
In order to ease the diagnostics of the application during the development phase makes use of log messages
through the integrated serial port. These messages contain information about the execution status of the
sketch such as: which SSID you are trying to connect to, if there was an error and information about
it or if a failed operation is being re-attempted.
 
These messages come in very handy to us but they can also be handy _to **any** potential attacker_
that could get its hands on a device, which is not an unrealistic chance since the node would be deployed to the outside world. 
To access these precious logs all it would simply take is for the attacker to connect the IoT node to any computer with serial port reader software such as the one 
included on the Arduino IDE.
 
To prevent this, an option has been set in the `config.h` file that allows you to enable or disable
logging messages when the application is compiled. Another valid alternative which has not been implemented
could be to define a secret pin that in case it was bridged with VCC and hence a `HIGH` value  was read on this pin would mean that logging messages are enabled. The advantage of the latter method is that it would allow us to solve and diagnose problems of nodes that already have been deployed to the _battlefield_.
 
### Authorization of MQTT users
As mentioned in the first sections of this document, we can configure the credentials which our node will use to connect to the MQTT broker. This is because we make use of access control lists (ACL) on our `mosquitto`.
 
This allows us to limit the access of the MQTT clients. This is useful in case the credentials of our node would have been compromised and an attacker would have tried to make malicious use of them. In our case, this attacker could only post in the `presence` topic without being able to subscribe to any other topic and in case we detected a strange behavior in the logs of our server, we can always revoke the credentials and send the attacker back to the starting point.


## Sketch dependencies
* ArduinoBLE.h (by Arduino) 1.2.0
* WiFiNINA.h (by Arduino) 1.8.9
* PubSubClient.h (by Nick O'Leary) 2.8.0

## Approach

The MCU uses an ESP32 module to have access to WiFi and Bluetooth Low Energy.
Since both operate on the 2.42GHz band, the same module re-uses the antenna to provide one or the other **BUT NEVER BOTH AT THE SAME TIME**.
To address this issue, this sketch assumes a 10 second device scanning cycle and afterwards it just reports the device count to the MQTT server.


Feel free to customize your sketch by providing your own `config.h` file with your own parameters.

## Configuring TLS on your Nano 33 IoT

1. Make sure your firmware is updated. To do so make sure you have the latest version of the WiFiNINA library by running the CheckFirmwareVersion sketch under File > Samples > WiFiNINA > Tools > CheckFirmwareVersion. (In your Arduino IDE)

If you are not running the latest version read through, otherwise skip to step 3

2. To update the Firmware version we need to flash our node with the "FirmwareUpdater" sketch. It's under File > Samples > WiFiNINA > Tools > FirmwareUpdater.

Once you've done that, on your Arduino IDE do the following:
* Go to Tools > WiFi 101 / WiFiNINA Firmware Updater
* Select your Node from the available ports
* Select latest version of the NINA firmware (I had to install Arduino nightly build to actually get the latest version of the firmware)
* Click "Update Firmware"
* Done!

<img src="firmwareupdate.JPG">

3. Head to the section three of the above screenshot and click "Add domain". Fill the popup window with the IP address and port where your broker is serving MQTT so ArduinoIDE can download the root certificate and flash it into the WiFi module.

<img src="cert_addr.JPG">

And finally click "Upload Certificates to WiFi module" and we're done!

### Configuring TLS on your server

Feel free to use the provided bash scripts to generate the CA and broker Certificates alongside their private keys. I've generated mine in my own Raspberry Pi 4

**Important**: In case your server does not have a domain name make sure that when filling out your **broker** form from `openssl` make sure to fill the field `Common Name` with the IPv4 of your server.

Yes, DHCP may break the certificates if you're using DHCP so using a static IP is highly suggested.
