#include "wifi.h"
//const char* ssid = "Pedro Buzatto";
//const char* password =  "abcd1234";
const char* ssid = "TESTECPID";
const char* password =  "solares1";
//const char* ssid = "NET_2GEA12B2";
//const char* password =  "E2EA12B2";
unsigned long previousMillis = 0;

void init_wifi(){
  WiFi.begin(ssid, password);

  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();

  Serial.println("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 12000) {
      previousMillis = currentMillis;
      return;
    }
  }
  
  Serial.println("Connected to the WiFi network");  
  delay(500);
}
