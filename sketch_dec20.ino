/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define MOTION 13
#define MOTION2 26
#define MOTION3 12
const int OUTPUT1 =  27;
const char* ssid = "smartsite";
const char* password = "smartsite_jabo1";

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.175/smartsite/sensors_handler.php";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 60000;


long previousMillis = 0;
long interval = 300000;

void setup() {
  pinMode(MOTION, INPUT);
  pinMode(MOTION2, INPUT);
  pinMode(MOTION3, INPUT);
  pinMode(OUTPUT1, OUTPUT);
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  

  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
      bool motion = digitalRead(MOTION);
      bool motion2 = digitalRead(MOTION2);
      bool motion3 = digitalRead(MOTION3);
      
      Serial.println("Motion condition: ");
      Serial.print("Motion1: ");
      Serial.print(motion);
      Serial.print(", Motion2: ");
      Serial.print(motion2);      
      Serial.print(", Motion3: ");
      Serial.println(motion3);    
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
        String serverPath = serverName + "?siteId=JKT103&siteName=PINTUTMNMINI&isMotionDetected=0";
      if(motion == 0 || motion2 == 0 || motion3 == 0) {
        serverPath = serverName + "?siteId=JKT103&siteName=PINTUTMNMINI&isMotionDetected=1";
      } 
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();        
        Serial.println(payload);

        DynamicJsonDocument doc(150);
    
        DeserializationError error = deserializeJson(doc, payload);
        
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
          const char* site_id = doc["site_id"]; // "JKT016"
          const char* site_name = doc["site_name"]; // "BEKASI"
          const char* alarm_on = doc["alarm_on"]; // "1,0 atau ''"
          Serial.print("Output status: ");
          Serial.println(alarm_on);
//
//          unsigned long currentMillis = millis();
//          
//          if(motion_detected && currentMillis - previousMillis < interval) {
//              
//            previousMillis = currentMillis;   
//                digitalWrite(OUTPUT1, HIGH)
//              } else{
//                digitalWrite(OUTPUT1, LOW)       
//              }

            
//          if(alarm_on == "1") { // jika motion terdeteksi
//             digitalWrite(OUTPUT1, HIGH); // menyalakan output
//             unsigned long currentMillis = millis(); //definisi waktu sekarang
//              if(currentMillis - previousMillis > interval) { //jika interval tercapai maka eksekusi perintah di bawah nya
//                previousMillis = currentMillis;   
//                digitalWrite(OUTPUT1, LOW);
//              }
//          } 
//          if(alarm_on == "0") {
//            digitalWrite(OUTPUT1, LOW);
//          }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
