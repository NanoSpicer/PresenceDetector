## What does this file do?

This file requires you to run in administrator permission as it:

1. Stops any `mosquitto` services that may be running
2. Starts it's own mosquitto service using the confiquration under `.conf/`

### Re-generating the SSL certificates

It doesn't matter how you fill the forms requested by the `gen_{whatever}.sh` (under `.conf/tls/ca` or `.conf/tls/server` folders) scripts except for the field "Common Name". 

That is what is used to verify the server against the certificate.
If you have a domain name, use the domain name. If you don't have a domain name use the static IP address of your server.

### Notes
1. If your IP address changes, the verification of the server will fail on the client side.
2. We're not setting up "server authenticating the client" as the Arduino Nano 33 IoT does not support that feature.

### How to spin up the mosquitto
> sudo ./mosquitto.sh