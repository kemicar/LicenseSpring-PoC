#include <api_class.h>

APIcall::APIcall(const char *root_ca_char, String shared_key, String uuid, String api_version, String api_url)
{
  // constructor root_ca and shared_key
  root_ca = root_ca_char;

  WiFiClientSecure client;

  this->shared_key = shared_key;
  this->uuid = uuid;

  hardware_id = "";

  full_api_url = api_url + api_version;
}

// extract MAC adress as hardware id
void APIcall::hardware_id_setup()
{
  uint8_t mac[6];

  // Read MAC address for WiFi station
  esp_read_mac(mac, ESP_MAC_WIFI_STA);

  for (int i = 0; i < 6; ++i)
  {
    if (mac[i] < 16)
    {
      hardware_id += "0";
    }
    hardware_id += String(mac[i], HEX);
    if (i < 5)
    {
      hardware_id += ":";
    }
  }

  hardware_id.toUpperCase();
}

// Setup certificate for HTTPS requests
void APIcall::certificate_setup()
{

  client.setCACert(root_ca);
}

// extract time in UTC
String APIcall::get_time()
{

  String time;
  const char *ntpServer = "pool.ntp.org";
  configTime(0, 0, ntpServer);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    // Exit the function if time cannot be obtained
  }
  char buffer[30];

  strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &timeinfo);

  time = String(buffer);
  return time;
}

// Authorization headers for API communication
String APIcall::auth_header()
{
  String auth_header;
  String time = get_time();

  String msg = "licenseSpring\ndate: " + time;

  String signiture = HMAC_SHA256_To_Base64(shared_key, msg); // passing signature to hmac_sha256 and doing base encoding

  auth_header = "algorithm=\"hmac-sha256\", headers=\"date\", signature=\"" + signiture + "\", apiKey=\"" + uuid + "\"";

  return auth_header;
}

void APIcall::add_header(HTTPClient &http, String &time, String &auth_header)
{
  // Add the headers to the request.
  http.addHeader("Date", time);
  http.addHeader("Authorization", auth_header);
  http.addHeader("Content-Type", "application/json");
}

DynamicJsonDocument APIcall::check_license(String product_short_code, String license_key)
{

  DynamicJsonDocument doc(2500);

  String time = get_time();
  String auth = auth_header();

  HTTPClient http;

  String url = full_api_url + "check_license/";

  url += "?product=" + product_short_code;
  url += "&hardware_id=" + hardware_id;
  url += "&license_key=" + license_key;

  http.begin(client, url);
  add_header(http, time, auth);
  int httpCode = http.GET();

  Serial.printf("[HTTP] GET... code: %d\n", httpCode);

  String data = http.getString();

  deserializeJson(doc, data);

  http.end();
  return doc;
}

DynamicJsonDocument APIcall::activate_license(String product_short_code, String license_key)
{

  DynamicJsonDocument doc(2500);

  String payload = "{\"product\":\"" + product_short_code + "\",\"license_key\":\"" + license_key + "\",\"hardware_id\":\"" + hardware_id + "\"}";

  String time = get_time();
  String auth = auth_header();

  HTTPClient http;

  String url = full_api_url + "activate_license/";

  http.begin(client, url);
  add_header(http, time, auth);

  int httpCode = http.POST(payload);

  Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  String data = http.getString();

  // Deserialize the JSON document
  deserializeJson(doc, data);

  http.end();
  return doc;
}

String HMAC_SHA256_To_Base64(String secretKey, String message)
{
  Sha256Wrapper sha256; // Declare an instance of the Sha256Wrapper class
                        // Declare an instance of the Sha256Wrapper class

  // Calculate HMAC-SHA256 hash
  sha256.initHmac((const uint8_t *)secretKey.c_str(), secretKey.length());
  sha256.print(message.c_str());
  uint8_t *result = sha256.resultHmac();

  // Convert HMAC-SHA256 result to byte array
  uint8_t byteAr[32];
  for (int i = 0; i < 32; i++)
  {
    byteAr[i] = result[i];
  }
  String encoded = base64Encode(byteAr, 32);
  // Base64 encoding
  return encoded;
}

String base64Encode(byte *byteArray, int len)
{

  String encodedString = "";

  const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  int i = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (len--)
  {
    char_array_3[i++] = *(byteArray++);
    if (i == 3)
    {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      for (i = 0; i < 4; i++)
        encodedString += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for (int j = i; j < 3; j++)
      char_array_3[j] = '\0';
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;
    for (int j = 0; (j < i + 1); j++)
      encodedString += base64_chars[char_array_4[j]];
    while ((i++ < 3))
      encodedString += '=';
  }

  return encodedString;
}
