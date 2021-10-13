/****************************************************************************************************************************
  WiFi_WebSocketClient.h - Dead simple HTTP WebSockets Client.
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
 
// (c) Copyright Arduino. 2016
// Released under Apache License, version 2.0

#pragma once

#include <Arduino.h>

#include "utility/WiFiDebug.h"

#include "WiFi_HTTPClient/WiFi_HttpClient.h"

static const int TYPE_CONTINUATION     = 0x0;
static const int TYPE_TEXT             = 0x1;
static const int TYPE_BINARY           = 0x2;
static const int TYPE_CONNECTION_CLOSE = 0x8;
static const int TYPE_PING             = 0x9;
static const int TYPE_PONG             = 0xa;

class WiFiWebSocketClient : public WiFiHttpClient
{
  public:
    WiFiWebSocketClient(Client& aClient, const char* aServerName, uint16_t aServerPort = WiFiHttpClient::kHttpPort);
    WiFiWebSocketClient(Client& aClient, const String& aServerName, uint16_t aServerPort = WiFiHttpClient::kHttpPort);
    WiFiWebSocketClient(Client& aClient, const IPAddress& aServerAddress, uint16_t aServerPort = WiFiHttpClient::kHttpPort);

    /** Start the Web Socket connection to the specified path
      @param aURLPath     Path to use in request (optional, "/" is used by default)
      @return 0 if successful, else error
    */
    int begin(const char* aPath = "/");
    int begin(const String& aPath);

    /** Begin to send a message of type (TYPE_TEXT or TYPE_BINARY)
        Use the write or Stream API's to set message content, followed by endMessage
        to complete the message.
      @param aURLPath     Path to use in request
      @return 0 if successful, else error
    */
    int beginMessage(int aType);

    /** Completes sending of a message started by beginMessage
      @return 0 if successful, else error
    */
    int endMessage();

    /** Try to parse an incoming messages
      @return 0 if no message available, else size of parsed message
    */
    int parseMessage();

    /** Returns type of current parsed message
      @return type of current parsedMessage (TYPE_TEXT or TYPE_BINARY)
    */
    int messageType();

    /** Returns if the current message is the final chunk of a split
        message
      @return true for final message, false otherwise
    */
    bool isFinal();

    /** Read the current messages as a string
      @return current message as a string
    */
    String readString();

    /** Send a ping
      @return 0 if successful, else error
    */
    int ping();

    // Inherited from Print
    virtual size_t write(uint8_t aByte);
    virtual size_t write(const uint8_t *aBuffer, size_t aSize);
    
    // Inherited from Stream
    virtual int   available();
    /** Read the next byte from the server.
      @return Byte read or -1 if there are no bytes available.
    */
    virtual int read();
    virtual int read(uint8_t *buf, size_t size);
    virtual int peek();

  private:
    void flushRx();

  private:
    bool      iTxStarted;
    uint8_t   iTxMessageType;
    uint8_t   iTxBuffer[128];
    uint64_t  iTxSize;

    uint8_t   iRxOpCode;
    uint64_t  iRxSize;
    bool      iRxMasked;
    int       iRxMaskIndex;
    uint8_t   iRxMaskKey[4];
};

