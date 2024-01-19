#include <read_data.h>

String read_certificate(const char *path)
{
  
  Serial.println(ESP.getFreeHeap());
  String certificate = "";
  File file = SPIFFS.open(path, "r");

  if (!file || file.isDirectory())
  {
    Serial.println("Failed to open file for reading");
    
    file.close();
    return certificate;
  }

  // Serial.println("Certificate contents:");
  while (file.available())
  {

    char ch = file.read();
    certificate += ch;
  }

  file.close();

  return certificate;
}

void read_data_json(String &uuid, String &product_short_code, String &secret_key, String &ssid, String &password)
{

  File file = SPIFFS.open("/config/config.json", "r");
  if (!file)
  {

    return;
  }

  // Deserialize the JSON document
  StaticJsonDocument<256> doc; // Adjust size according to your needs

  DeserializationError error = deserializeJson(doc, file);

  file.close();

  // Update global variables
  uuid = doc["uuid"].as<String>();
  product_short_code = doc["product_short_code"].as<String>();
  secret_key = doc["secret_key"].as<String>();
  ssid = doc["ssid"].as<String>();
  password = doc["password"].as<String>();
}

void connect_to_WiFi(String ssid, String password)
{
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);

    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("Server IP Address: ");
  Serial.print(WiFi.localIP());
}
