# LicenseSpring API Client with SSL/TLS Support - ESP32 board (Arduino)

This project was part of my onboarding as a **student**. The goal was to establish **HTTPS** communication with the LicenseSpring server on an ESP32 board and perform license activation and license check. The actual code is written in C++ and follows the Arduino structure, utilizing libraries and functions suitable for microcontroller programming.

## Key features 
**LicenseSpring API Integration**
* Directly connect with LicenseSpring's client API to activate and deactivate software licenses
* Ensuring the security of data transmission, our project utilizes HTTPS requests combined with SSL/TLS encryption. 
  
**User-Friendly Web Interface**
* Manage your certificates and license keys using server running on the board flash memory
* Submit License key on running server to activate license 

## Installation 

clone GitHub repo: `git clone git@github.com:kemicar/LicenseSpring-PoC.git`

## Configuration

Inside data folder create config folder with **config.json** file:
* `cd data`
* `mkdir config`
* `touch config.json`

**config.json** should have:
```json
{
  "uuid": "your-LicenseSpring-uuid-here",
  "product_short_code": "your-LicenseSpring-product-short-code-here",
  "secret_key": "your-LicenseSpring-secret-key-here",
  "ssid": "your-network-ssid-here",
  "password": "your-network-password-here"
}
```

### Certificate 

To get certificate visit: **https://api.licensespring.com/**

Download **ROOT certificate** in .pem format and save it locally on your pc with name **ls_prod.pem**. 
If you are unsure how to acquire certificate from web browser check [here](https://medium.com/@menakajain/export-download-ssl-certificate-from-server-site-url-bcfc41ea46a2)

## Usage
### Code upload and setup
1. Upload code to your ESP32 board: `platformio run --target upload`
2. Open serial monitor: `platformio device monitor`
3. You should see server **IP adress** inside serial monitor --> visit that site
   
### Certificate and LicenseKey setup using UI
#### Certificate
1. To setup certificate for ESP32 board visit *Go to Certificate Update Page*

![Main page](/images/image.png)

2. Upload your **ls_prod.pem** Certifcate
   
   ![Certificate page](image.png)

After you've added certificate ESP32 will boot and reload page for key activation setup.
**You only need to once add certificate after that certificate is automatically loaded from flash**

#### LicenseKey
1. Create Key based license inside **[LicenseSpring platform](https://docs.licensespring.com/vendor-platform/issuing-new-licenses)**
2. Vist ESP32 server site
3. Enter License key in given field and click on acitvate.
   
![License activation](images/image2.png)

### Logic
After that ESP32 will close server connection, execute license activate and license check. Licese check will be repeated every 24 hours (there's specific timer for that). You can add your own 






  