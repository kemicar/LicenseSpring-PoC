#ifndef READ_DATA_H
#define READ_DATA_H
#include <Arduino.h>
#include <WiFi.h>

#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

String read_certificate(const char *path);
void read_data_json(String &uuid, String &product_short_code, String &secret_key, String &ssid, String &password);
void connect_to_WiFi(String ssid, String password);

#endif