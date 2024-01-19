#include <read_data.h>
#include <local_server.h>
#include <api_class.h>

// LicenseSpring
String uuid;
String product_short_code;
String secret_key;
String license_key;
String ssid;
String password;

String certificate;

bool licenseKeyReceived = false;
bool state = false;

// JSON respone
DynamicJsonDocument response(2500);

hw_timer_t *timer = NULL;

// Server and API client
ServerLocal *k;
APIcall *api;

volatile int hourCounter = 0;

// Timer function
void IRAM_ATTR onTimer()
{
  hourCounter++;
}

void setup()
{
  delay(5000);
  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    return;
  }

  k = new ServerLocal();

  certificate = read_certificate("/ls_prod.pem");

  read_data_json(uuid, product_short_code, secret_key, ssid, password);

  api = new APIcall(certificate.c_str(), secret_key, uuid);

  api->hardware_id_setup();
  api->certificate_setup();

  connect_to_WiFi(ssid, password);

  k->home_route();
  k->change_cert();
  k->submit(license_key, licenseKeyReceived);
  k->setup_cert();

  while (!licenseKeyReceived)
  {
    k->stay_active();
  }

  delete k; // Cleanup
  k = nullptr;

  response = api->activate_license(product_short_code, license_key);

  response = api->check_license(product_short_code, license_key);

  state = response["license_active"].as<bool>();

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 3600000000, true);
}

void loop()
{

  if (hourCounter == 24)
  {

    response = api->check_license(product_short_code, license_key);
    state = response["license_active"].as<bool>();

    hourCounter = 0;
  }

  if (state == true)
  {
    Serial.println("License is activate add functionality");
    delay(3000);
  }
  delay(20000);
}
