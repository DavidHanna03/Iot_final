#define BLYNK_TEMPLATE_ID "TMPL2Qa5lNqbD"
#define BLYNK_TEMPLATE_NAME "IotProject"
#define BLYNK_AUTH_TOKEN "1ITa-QRXSdLNcuK_DRHJkSTB8pyM8B_s"

#include <WiFiEsp.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>

// Emulate Serial1 on pins 4/5 (RX, TX) for ESP communication
SoftwareSerial Serial1(4, 5); // RX, TX
ESP8266 wifi(&Serial1); // Create an instance of the ESP8266 module

char ssid[] = "Alex";
char pass[] = "12345678";

int moistureSensorPin = A0; // Moisture sensor connected to A0
int moistureValue = 0;
int moistureThreshold = 80; // Default threshold value, adjustable with slider

BLYNK_WRITE(V2) {
  // Read the slider value from Virtual Pin V2
  moistureThreshold = param.asInt();
}

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Start software serial for ESP module
  Serial1.begin(9600);
  
  // Initialize the ESP module with the WiFiEsp library
  WiFi.init(&Serial1);
  
  // Check if the WiFi shield is present
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true); // Halt execution if no WiFi shield is found
  }

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
  
  // Set moisture sensor pin as input
  pinMode(moistureSensorPin, INPUT);
}

void loop() {
  // Run Blynk to keep connection alive and interact with the app
  Blynk.run();
  
  // Read the moisture level from the sensor
  moistureValue = analogRead(moistureSensorPin);

  // Convert the analog reading to a percentage (0 - 100)
  int moisturePercentage = map(moistureValue, 0, 1023, 0, 100);
  
  // Send the moisture percentage to the Blynk app
  Blynk.virtualWrite(V1, moisturePercentage);

  // Trigger a notification if the moisture level is lower than the set threshold
  if (moisturePercentage > moistureThreshold) {
    Blynk.logEvent("low_moisture_alert", "Warning: Soil moisture level is low!");
    Serial.println("Warning: Soil moisture level is low!");
  }

  // Delay to control the frequency of sensor readings
  delay(1000);
}
