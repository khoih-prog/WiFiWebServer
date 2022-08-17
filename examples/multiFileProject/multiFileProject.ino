/****************************************************************************************************************************
  multiFileProject.ino
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#define WIFI_WEBSERVER_VERSION_MIN_TARGET      "WiFiWebServer v1.9.3"
#define WIFI_WEBSERVER_VERSION_MIN             1009003

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <WiFiWebServer.h>      // https://github.com/khoih-prog/WiFiWebServer

void setup() 
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  delay(500);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(WIFI_WEBSERVER_VERSION);

#if defined(WIFI_WEBSERVER_VERSION_MIN)
  if (WIFI_WEBSERVER_VERSION_INT < WIFI_WEBSERVER_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(WIFI_WEBSERVER_VERSION_MIN_TARGET);
  }
#endif

  Serial.print("You're OK now");
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
