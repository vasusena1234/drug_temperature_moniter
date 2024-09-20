#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 4
#define DHTTYPE DHT11

// WiFi credentials
const char* ssid = "Your Wifi SSID";
const char* password = "Your Wifi PWD";

// OAuth 2.0 credentials
const char* oauth_token_url = "https://daf8aba4trial.authentication.us10.hana.ondemand.com/oauth/token";
const char* client_id = "Your Client ID";
const char* client_secret = "Your Client Secret";
String access_token = "";  // This will hold the access token
long token_expiry = 0;     // Token expiration time

// API endpoint for posting temperature data
const char* api_url = "https://daf8aba4trial-dev-drug-temperature-moniter-srv.cfapps.us10-001.hana.ondemand.com/odata/v4/drug-srv/temperature";

// DHT sensor
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure wifiClient;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  delay(10);

  // Connect to WiFi
  connectToWiFi();

  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Check if access token is valid or expired
    if (millis() > token_expiry || access_token == "") {
      if (!getOAuthToken()) {
        Serial.println("Failed to get OAuth token");
        delay(10000);
        return;
      }
    }

    // Get temperature from DHT sensor
    //int temperature = dht.readTemperature();

    float tc = dht.readTemperature(false);  //Read temperature in C
    float tf = dht.readTemperature(true);   //Read Temperature in F
    float hu = dht.readHumidity();          //Read Humidity

    // Send POST request with sensor data
    sendTemperatureData(tc, tf, hu);

    // Wait 10 seconds before sending the next request
    delay(10000);
  }
}

// Function to connect to WiFi
void connectToWiFi() {
  Serial.println();
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

// Function to get OAuth 2.0 token
bool getOAuthToken() {
  wifiClient.setInsecure();  // Skip SSL certificate verification
  http.begin(wifiClient, oauth_token_url);

  // Set up the headers and body for the token request
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "grant_type=client_credentials&client_id=" + String(client_id) + "&client_secret=" + String(client_secret);

  // Send POST request
  int httpCode = http.POST(postData);

  if (httpCode == 200) {
    String payload = http.getString();
    Serial.println("Token response: " + payload);

    // Parse JSON response
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    access_token = doc["access_token"].as<String>();
    int expires_in = doc["expires_in"].as<int>();   // Expires in seconds
    token_expiry = millis() + (expires_in * 1000);  // Calculate expiration time in milliseconds

    Serial.println("Access token: " + access_token);
    Serial.println("Token expires in: " + String(expires_in) + " seconds");

    http.end();
    return true;
  } else {
    Serial.println("Failed to get token. HTTP error: " + String(httpCode));
    http.end();
    return false;
  }
}

// Function to send temperature data
void sendTemperatureData(float tc, float tf, float hu) {
  wifiClient.setInsecure();
  http.begin(wifiClient, api_url);

  // Set up headers
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + access_token);

  // Prepare JSON body
  DynamicJsonDocument doc(256);
  doc["sensorID"] = 2;
  doc["temperatureInC"] = tc;
  doc["temperatureInF"] = tf;
  doc["humidity"] = hu;
  doc["IsActiveEntity"] = true;

  String requestBody;
  serializeJson(doc, requestBody);

  // Send POST request
  int httpCode = http.POST(requestBody);

  // Check response
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on sending POST: " + String(httpCode));
  }

  http.end();
}
