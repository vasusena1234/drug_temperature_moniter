# Temperature Monitoring System with ESP8266 and SAP BTP CAPM

## Overview
This project demonstrates how to set up a temperature monitoring system that reads data from a DHT11 sensor connected to an ESP8266 microcontroller. The ESP8266 connects to a WiFi network, retrieves an OAuth 2.0 token for authentication, and sends temperature (in Celsius and Fahrenheit) and humidity data to an SAP BTP CAPM OData service. The SAP BTP CAPM backend stores the sensor readings in a database, with draft-enabled functionality.

## Prerequisites

### Hardware:
- ESP8266 microcontroller
- DHT11 temperature and humidity sensor
- Jumper wires
- Breadboard

### Software:
- Arduino IDE (with ESP8266 board support)
- SAP BTP account with Cloud Application Programming Model (CAPM) setup

### Libraries:
Install the following libraries in Arduino IDE:
- **ESP8266WiFi**: For WiFi connectivity.
- **ESP8266HTTPClient**: For making HTTP requests.
- **WiFiClientSecure**: For handling HTTPS connections.
- **ArduinoJson**: For constructing and parsing JSON objects.
- **DHT sensor library**: For reading data from the DHT11 sensor.

## Project Structure

### 1. **Backend (SAP BTP CAPM)**
- **Namespace**: `drug.temperature`
- **Entities**:
  - `Temperature_Moniter`:
    ```cds
    entity Temperature_Moniter : cuid {
      sensorID       : Integer    @title: 'Sensor ID';
      temperatureInC : Decimal    @title: 'Temperature in Celsius';
      temperatureInF : Decimal    @title: 'Temperature in Fahrenheit';
      humidity       : Decimal    @title: 'Humidity';
      createdAt      : Timestamp  @title: 'Created At Time' @cds.on.insert: $now @readonly;
    }
    ```

- **Service (drugSrv)**:
  The service is exposed as an OData draft-enabled service.
  ```cds
  using drug.temperature as my from '../db/drug-model';

  service drugSrv {
    @odata.draft.enabled @odata.draft.bypass
    entity temperature as projection on my.Temperature_Moniter;
  }
  ```

### 2. **ESP8266 Code**
- **WiFi Connection**: Connects to a WiFi network using SSID and password.
- **OAuth 2.0 Authentication**: Retrieves an access token from the OAuth token URL.
- **POST Request**: Sends temperature and humidity data to the CAPM OData service every 10 seconds.
- **Token Refresh**: Automatically refreshes the token when it expires.

## Steps to Set Up the Backend

1. **Create a Database Model in CAPM**:
   - Define the entity `Temperature_Moniter` in `db/drug-model.cds`.
   - This entity stores sensor data (temperature, humidity, and timestamps).

2. **Set Up the Service**:
   - Expose the entity `Temperature_Moniter` in a service called `drugSrv` in `srv/drug-srv.cds`.
   - Enable draft functionality with `@odata.draft.enabled` and bypass it with `@odata.draft.bypass`.

3. **Deploy the Database**:
   - Run the following commands to deploy the database and service:
     ```bash
     cds deploy --to sqlite:db.sqlite
     cds run
     ```
   - Ensure your CAPM service is running and accessible.

4. **Configure OAuth 2.0**:
   - Set up an OAuth 2.0 client in SAP BTP.
   - Copy the client ID and secret into your ESP8266 code for token retrieval.

## Steps to Set Up the ESP8266

1. **Set Up the Hardware**:
   - Connect the DHT11 sensor to the ESP8266:
     - **DHT11** Pinout:
       - VCC → 3.3V
       - GND → GND
       - Data → D4 (GPIO 2)

2. **Arduino Code**:
   - Install the necessary libraries in the Arduino IDE.
   - Use the following code to read data from the sensor and send it to the CAPM backend:

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 4
#define DHTTYPE DHT11

// WiFi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_Password";

// OAuth 2.0 credentials
const char* oauth_token_url = "https://your_token_url";
const char* client_id = "Your_Client_ID";
const char* client_secret = "Your_Client_Secret";
String access_token = "";
long token_expiry = 0;

// API endpoint
const char* api_url = "https://your_api_url";

// DHT sensor
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure wifiClient;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() > token_expiry || access_token == "") {
      if (!getOAuthToken()) {
        delay(10000);
        return;
      }
    }

    float tc = dht.readTemperature(false);
    float tf = dht.readTemperature(true);
    float hu = dht.readHumidity();
    
    sendTemperatureData(tc, tf, hu);
    delay(10000);
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1000);
}

bool getOAuthToken() {
  wifiClient.setInsecure();
  http.begin(wifiClient, oauth_token_url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "grant_type=client_credentials&client_id=" + String(client_id) + "&client_secret=" + String(client_secret);
  int httpCode = http.POST(postData);
  
  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    access_token = doc["access_token"].as<String>();
    token_expiry = millis() + (doc["expires_in"].as<int>() * 1000);
    http.end();
    return true;
  } else {
    http.end();
    return false;
  }
}

void sendTemperatureData(float tc, float tf, float hu) {
  wifiClient.setInsecure();
  http.begin(wifiClient, api_url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + access_token);

  DynamicJsonDocument doc(256);
  doc["sensorID"] = 2;
  doc["temperatureInC"] = tc;
  doc["temperatureInF"] = tf;
  doc["humidity"] = hu;
  doc["IsActiveEntity"] = true;
  
  String requestBody;
  serializeJson(doc, requestBody);
  int httpCode = http.POST(requestBody);

  if (httpCode > 0) {
    Serial.println(http.getString());
  } else {
    Serial.println("Error: " + String(httpCode));
  }
  http.end();
}
```

3. **Upload Code**:
   - Connect the ESP8266 to your computer and upload the code via the Arduino IDE.

4. **Monitor Output**:
   - Use the Serial Monitor to observe the ESP8266 output, ensuring that it sends the data every 10 seconds.

## Summary
This project demonstrates how to build an IoT solution with an ESP8266 and SAP BTP CAPM. It retrieves temperature and humidity data from a DHT11 sensor, authenticates via OAuth 2.0, and sends the sensor data to an SAP OData service for storage and future analysis.

