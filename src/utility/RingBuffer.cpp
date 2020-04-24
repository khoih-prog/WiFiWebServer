/****************************************************************************************************************************
   RingBuffer.cpp - Dead simple web-server.
   For ESP32-based WiFi shields, such as WiFiNINA W101, W102, W13x, etc

   WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
   Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
   Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
   Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
   Licensed under MIT license
   Version: 1.0.4

   Original author:
   @file       Esp8266WebServer.h
   @author     Ivan Grokhotkov

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
    1.0.1   K Hoang      28/03/2020 Change to use new WiFiNINA_Generic library to support many more boards running WiFiNINA
    1.0.2   K Hoang      28/03/2020 Add support to SAMD51 and SAM DUE boards
    1.0.3   K Hoang      22/04/2020 Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, 
                                    Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc. 
    1.0.4   K Hoang      23/04/2020 Add support to MKR1000 boards using WiFi101 and custom WiFi libraries.
 *****************************************************************************************************************************/

#include "RingBuffer.h"

#include <Arduino.h>

WiFi_RingBuffer::WiFi_RingBuffer(unsigned int size)
{
  _size = size;
  // add one char to terminate the string
  ringBuf = new char[size + 1];
  ringBufEnd = &ringBuf[size];
  init();
}

WiFi_RingBuffer::~WiFi_RingBuffer() {}

void WiFi_RingBuffer::reset()
{
  ringBufP = ringBuf;
}

void WiFi_RingBuffer::init()
{
  ringBufP = ringBuf;
  memset(ringBuf, 0, _size + 1);
}

void WiFi_RingBuffer::push(char c)
{
  *ringBufP = c;
  ringBufP++;
  if (ringBufP >= ringBufEnd)
    ringBufP = ringBuf;
}

bool WiFi_RingBuffer::endsWith(const char* str)
{
  int findStrLen = strlen(str);

  // b is the start position into the ring buffer
  char* b = ringBufP - findStrLen;
  if (b < ringBuf)
    b = b + _size;

  char *p1 = (char*)&str[0];
  char *p2 = p1 + findStrLen;

  for (char *p = p1; p < p2; p++)
  {
    if (*p != *b)
      return false;

    b++;
    if (b == ringBufEnd)
      b = ringBuf;
  }

  return true;
}

void WiFi_RingBuffer::getStr(char * destination, unsigned int skipChars)
{
  //int len = ringBufP-ringBuf-skipChars;
  unsigned int len = ringBufP - ringBuf - skipChars;

  // copy buffer to destination string
  strncpy(destination, ringBuf, len);

  // terminate output string
  //destination[len]=0;
}

void WiFi_RingBuffer::getStrN(char * destination, unsigned int skipChars, unsigned int num)
{
  //int len = ringBufP-ringBuf-skipChars;
  unsigned int len = ringBufP - ringBuf - skipChars;

  if (len > num)
    len = num;

  // copy buffer to destination string
  strncpy(destination, ringBuf, len);

  // terminate output string
  //destination[len]=0;
}
