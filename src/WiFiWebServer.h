/*********************************************************************************************************************************
  WiFiWebServer.h - Dead simple web-server.
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.8.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
  ...
  1.6.0   K Hoang      13/02/2022 Add support to new ESP32-S3 and ESP32_C3
  1.6.1   K Hoang      13/02/2022 Fix v1.6.0 issue
  1.6.2   K Hoang      22/02/2022 Add support to megaAVR using Arduino megaAVR core
  1.6.3   K Hoang      02/03/2022 Fix decoding error bug
  1.7.0   K Hoang      05/04/2022 Fix issue with Portenta_H7 core v2.7.2+
  1.8.0   K Hoang      26/04/2022 Add WiFiMulti library support and examples.
 **********************************************************************************************************************************/

#pragma once

#ifndef WiFiWebServer_H
#define WiFiWebServer_H

#ifndef WIFI_WEBSERVER_VERSION
  #define WIFI_WEBSERVER_VERSION          "WiFiWebServer v1.8.0"

  #define WIFI_WEBSERVER_VERSION_MAJOR    1
  #define WIFI_WEBSERVER_VERSION_MINOR    8
  #define WIFI_WEBSERVER_VERSION_PATCH    0

  #define WIFI_WEBSERVER_VERSION_INT      1008000
#endif

#include "WiFiWebServer.hpp"
#include "WiFiWebServer-impl.h"
#include "Parsing-impl.h"


#endif    // WiFiWebServer_H
