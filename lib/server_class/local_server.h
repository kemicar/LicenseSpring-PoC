#ifndef LOCAL_SERVER_H
#define LOCAL_SERVER_H
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>



class ServerLocal {
private:
    WebServer server; // Change to WebServer

public:
    ServerLocal(); // Constructor
    //routes
    void home_route();
    void change_cert();
    void submit(String &globalLicenseKey,bool &licenseKeyReceived);
    void setup_cert();
    void stay_active();

    void handleFileUpload();
    void setupRoutes(String &globalLicenseKey, bool &licenseKeyReceived);
    // Other member functions...
};


#endif 



