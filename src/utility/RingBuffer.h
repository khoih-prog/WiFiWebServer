/**************************************************************************************************************************************
  RingBuffer.h - Dead simple web-server.
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.6.2

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
  ...
  1.6.0   K Hoang      13/02/2022 Add support to new ESP32-S3 and ESP32_C3
  1.6.1   K Hoang      13/02/2022 Fix v1.6.0 issue
  1.6.2   K Hoang      22/02/2022 Add support to megaAVR using Arduino megaAVR core
 ***************************************************************************************************************************************/

#pragma once

#ifndef RingBuffer_H
#define RingBuffer_H

class WiFi_RingBuffer
{
  public:
    WiFi_RingBuffer(unsigned int size);
    ~WiFi_RingBuffer();

    void reset();
    void init();
    void push(char c);
    int getPos();
    bool endsWith(const char* str);
    void getStr(char * destination, unsigned int skipChars);
    void getStrN(char * destination, unsigned int skipChars, unsigned int num);


  private:

    unsigned int _size;
    char* ringBuf;
    char* ringBufEnd;
    char* ringBufP;
};

#endif    // RingBuffer_H
