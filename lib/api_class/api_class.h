#ifndef api_class_h
#define api_class_h
#include <Arduino.h>
#include <time.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <sha/sha256.h>
#include <ArduinoJson.h>

class APIcall
{
private:
    const char *root_ca;
    String shared_key;
    String uuid;
    String full_api_url;
    String hardware_id;

    WiFiClientSecure client;

public:
    APIcall() {}
    APIcall(const char *root_ca_char, String shared_key, String uuid, String api_version = "api/v4/", String api_url = "https://api.licensespring.com/");

    void certificate_setup();
    void hardware_id_setup();
    void auth_header_and_time();
    void add_header(HTTPClient &http, String &time, String &auth_header);

    String get_time();
    String auth_header();

    DynamicJsonDocument check_license(String product_short_code, String license_key);
    DynamicJsonDocument activate_license(String product_short_code, String license_key);
};
String HMAC_SHA256_To_Base64(String secretKey, String message);
String base64Encode(byte *byteArray, int len);

#endif