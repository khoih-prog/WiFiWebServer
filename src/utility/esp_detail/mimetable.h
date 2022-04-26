/****************************************************************************************************************************
  mimetable.h - Dead simple web-server.
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
 *****************************************************************************************************************************/

#pragma once

#ifndef __ESP_MIMETABLE_H__
#define __ESP_MIMETABLE_H__

#if (ESP32 || ESP8266)

#include "WString.h"

namespace mime_esp
{
  enum type
  {
    html,
    htm,
    txt,
  #ifndef MIMETYPE_MINIMAL    // allow to compile with only the strict minimum of mime-types
    css,
    js,
    json,
    png,
    gif,
    jpg,
    jpeg,
    ico,
    svg,
    ttf,
    otf,
    woff,
    woff2,
    eot,
    sfnt,
    xml,
    pdf,
    zip,
    appcache,
  #endif // MIMETYPE_MINIMAL
    gz,
    none,
    maxType
  };
  
  struct Entry
  {
    const char * endsWith;
    const char * mimeType;
  };
  
  extern const Entry mimeTable[maxType];
  
  String getContentType(const String& path);
}

#endif    // #if (ESP32 || ESP8266)
#endif    // #ifndef __ESP_MIMETABLE_H__
