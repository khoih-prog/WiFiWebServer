/**************************************************************************************************************************************
  WiFiDebug.h - Dead simple web-server.
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.4.2

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
  1.0.1   K Hoang      28/03/2020 Change to use new WiFiNINA_Generic library to support many more boards running WiFiNINA
  1.0.2   K Hoang      28/03/2020 Add support to SAMD51 and SAM DUE boards
  1.0.3   K Hoang      22/04/2020 Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, 
                                  Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc. 
  1.0.4   K Hoang      23/04/2020 Add support to MKR1000 boards using WiFi101 and custom WiFi libraries.
  1.0.5   K Hoang      21/07/2020 Fix bug not closing client and releasing socket.    
  1.0.6   K Hoang      24/07/2020 Add support to all STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 boards. Restructure examples 
  1.0.7   K Hoang      25/09/2020 Restore support to PROGMEM-related commands, such as sendContent_P() and send_P()
  1.1.0   K Hoang      17/11/2020 Add basic HTTP and WebSockets Client by merging ArduinoHttpClient
  1.1.1   K Hoang      27/12/2020 Suppress all possible compiler warnings
  1.2.0   K Hoang      26/05/2021 Add support to RP2040-based boards using Arduino-pico and Arduino mbed_rp2040 core
  1.3.0   K Hoang      14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
  1.3.1   K Hoang      06/09/2021 Add support to ESP32/ESP8266 to use in some rare use-cases
  1.4.0   K Hoang      07/09/2021 Add support to Portenta H7
  1.4.1   K Hoang      04/10/2021 Change option for PIO `lib_compat_mode` from default `soft` to `strict`. Update Packages Patches
  1.4.2   K Hoang      12/10/2021 Update `platform.ini` and `library.json`
 ***************************************************************************************************************************************/

#pragma once

#include <stdio.h>

#ifdef DEBUG_WIFI_WEBSERVER_PORT
#define WS_DEBUG_OUTPUT DEBUG_WIFI_WEBSERVER_PORT
#else
#define WS_DEBUG_OUTPUT Serial
#endif

// Change _WIFI_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _WIFI_LOGLEVEL_
#define _WIFI_LOGLEVEL_       0
#endif

///////////////////////////////////////

#define WS_LOGERROR(x)         if(_WIFI_LOGLEVEL_>0) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.println(x); }
#define WS_LOGERROR0(x)        if(_WIFI_LOGLEVEL_>0) { WS_DEBUG_OUTPUT.print(x); }
#define WS_LOGERROR1(x,y)      if(_WIFI_LOGLEVEL_>0) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(y); }
#define WS_LOGERROR2(x,y,z)    if(_WIFI_LOGLEVEL_>0) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(z); }
#define WS_LOGERROR3(x,y,z,w)  if(_WIFI_LOGLEVEL_>0) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(z); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(w); }

///////////////////////////////////////

#define WS_LOGWARN(x)          if(_WIFI_LOGLEVEL_>1) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.println(x); }
#define WS_LOGWARN0(x)         if(_WIFI_LOGLEVEL_>1) { WS_DEBUG_OUTPUT.print(x); }
#define WS_LOGWARN1(x,y)       if(_WIFI_LOGLEVEL_>1) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(y); }
#define WS_LOGWARN2(x,y,z)     if(_WIFI_LOGLEVEL_>1) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(z); }
#define WS_LOGWARN3(x,y,z,w)   if(_WIFI_LOGLEVEL_>1) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(z); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(w); }

///////////////////////////////////////

#define WS_LOGINFO(x)          if(_WIFI_LOGLEVEL_>2) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.println(x); }
#define WS_LOGINFO0(x)         if(_WIFI_LOGLEVEL_>2) { WS_DEBUG_OUTPUT.print(x); }
#define WS_LOGINFO1(x,y)       if(_WIFI_LOGLEVEL_>2) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(y); }
#define WS_LOGINFO2(x,y,z)     if(_WIFI_LOGLEVEL_>2) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(z); }
#define WS_LOGINFO3(x,y,z,w)   if(_WIFI_LOGLEVEL_>2) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(z); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(w); }

///////////////////////////////////////

#define WS_LOGDEBUG(x)         if(_WIFI_LOGLEVEL_>3) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.println(x); }
#define WS_LOGDEBUG0(x)        if(_WIFI_LOGLEVEL_>3) { WS_DEBUG_OUTPUT.print(x); }
#define WS_LOGDEBUG1(x,y)      if(_WIFI_LOGLEVEL_>3) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(y); }
#define WS_LOGDEBUG2(x,y,z)    if(_WIFI_LOGLEVEL_>3) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(z); }
#define WS_LOGDEBUG3(x,y,z,w)  if(_WIFI_LOGLEVEL_>3) { WS_DEBUG_OUTPUT.print("[WIFI] "); WS_DEBUG_OUTPUT.print(x); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(y); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.print(z); WS_DEBUG_OUTPUT.print(" "); WS_DEBUG_OUTPUT.println(w); }

