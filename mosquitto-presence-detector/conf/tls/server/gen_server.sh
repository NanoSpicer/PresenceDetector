#!/bin/bash

openssl genrsa -out broker.key 2048

openssl req -out broker.csr -key broker.key -new

openssl x509 -req -in broker.csr -CA ../ca/ca.crt -CAkey ../ca/ca.key -CAcreateserial -out broker.crt -days 365