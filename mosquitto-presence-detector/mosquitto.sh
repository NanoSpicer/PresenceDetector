#!/bin/bash

if [ "$EUID" -ne 0 ]
then
    echo "Run as sudo to ensure mosquitto service is stopped"
    exit
else
    echo "Stopping already running mosquitto service"
    service mosquitto stop
fi

echo  "Starting mosquitto server with local config file…"
mosquitto -c ./conf/mosquitto.conf