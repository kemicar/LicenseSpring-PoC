#include <local_server.h>

// Constructor implementation
ServerLocal::ServerLocal() : server(80) {}
// Constructor code here

void ServerLocal::stay_active()
{
    server.handleClient();
}

void ServerLocal::handleFileUpload()
{
    static File file; // Make it static so it retains the value across function calls
    HTTPUpload &upload = this->server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        String filename = "/" + upload.filename;
        if (!filename.endsWith(".pem"))
        {
            Serial.println("File is not a .pem file");
            return;
        }

        file = SPIFFS.open(filename, "w");
        if (!file)
        {
            Serial.println("Failed to open file for writing");
            return;
        }

        Serial.println("Upload Start: " + filename);
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (file)
        {
            file.write(upload.buf, upload.currentSize);
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (file)
        {
            file.close();
        }
        Serial.println("Upload End: " + String(upload.totalSize) + " bytes for file: " + upload.filename);
        ESP.restart();
    }
}

void ServerLocal::home_route()
{

    server.on("/", HTTP_GET, [this]()
              {
    File file = SPIFFS.open("/server/home.html", "r");
    if (file) {
        this->server.streamFile(file, "text/html");
        file.close();
    } else {
        this->server.send(404, "text/plain", "File Not Found");
    } });
}

void ServerLocal::change_cert()
{

    server.on("/change_cert", HTTP_GET, [this]()
              {
    File file = SPIFFS.open("/server/cert.html", "r");
    if (file) {
        this->server.streamFile(file, "text/html");
        file.close();
    } else {
        this->server.send(404, "text/plain", "File Not Found");
    } });
}

void ServerLocal::submit(String &globalLicenseKey, bool &licenseKeyReceived)
{
    server.on("/submit", HTTP_POST, [this, &globalLicenseKey, &licenseKeyReceived]()
              {
    if (this->server.hasArg("licenseKey")) {
        globalLicenseKey = this->server.arg("licenseKey");
        Serial.println("Received License Key: " + globalLicenseKey);

        

        this->server.send(200, "text/plain", "License Key Received");
        licenseKeyReceived = true;
    } else {
        this->server.send(400, "text/plain", "License Key not received");
    } });
}

void ServerLocal::setup_cert()
{
    server.on(
        "/update-certificate", HTTP_POST, [this]()
        { this->server.send(200, "text/plain", "Certificate Uploaded"); },
        [this]()
        {
            handleFileUpload();
        });
    server.begin();
}
