/**************************************************************************************************************************************
  WiFiRequestHandlerImpl.h - Dead simple web-server.
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.3.0

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
 ***************************************************************************************************************************************/

#pragma once

#include "RequestHandler.h"
#include "mimetable.h"

class FunctionRequestHandler : public RequestHandler
{
  public:

    FunctionRequestHandler(WiFiWebServer::THandlerFunction fn, WiFiWebServer::THandlerFunction ufn, const String &uri, HTTPMethod method)
      : _fn(fn)
      , _ufn(ufn)
      , _uri(uri)
      , _method(method)
    {
    }

    bool canHandle(HTTPMethod requestMethod, String requestUri) override
    {
      if (_method != HTTP_ANY && _method != requestMethod)
        return false;

      if (requestUri == _uri)
        return true;

      if (_uri.endsWith("/*"))
      {
        String _uristart = _uri;
        _uristart.replace("/*", "");

        if (requestUri.startsWith(_uristart))
          return true;
      }

      return false;
    }

    bool canUpload(String requestUri) override
    {
      if (!_ufn || !canHandle(HTTP_POST, requestUri))
        return false;

      return true;
    }

    bool handle(WiFiWebServer& server, HTTPMethod requestMethod, String requestUri) override
    {
      WFW_UNUSED(server);
      
      if (!canHandle(requestMethod, requestUri))
        return false;

      _fn();
      return true;
    }

    void upload(WiFiWebServer& server, String requestUri, HTTPUpload& upload) override
    {
      WFW_UNUSED(server);
      WFW_UNUSED(upload);
      
      if (canUpload(requestUri))
        _ufn();
    }

  protected:
    WiFiWebServer::THandlerFunction _fn;
    WiFiWebServer::THandlerFunction _ufn;
    String _uri;
    HTTPMethod _method;
};

class StaticRequestHandler : public RequestHandler
{
  public:

    bool canHandle(HTTPMethod requestMethod, String requestUri) override
    {
      if (requestMethod != HTTP_GET)
        return false;

      if ((_isFile && requestUri != _uri) || !requestUri.startsWith(_uri))
        return false;

      return true;
    }

#if USE_NEW_WEBSERVER_VERSION

    static String getContentType(const String& path)
    {
      using namespace mime;
      char buff[sizeof(mimeTable[0].mimeType)];

      // Check all entries but last one for match, return if found
      for (size_t i = 0; i < sizeof(mimeTable) / sizeof(mimeTable[0]) - 1; i++)
      {
        strcpy(buff, mimeTable[i].endsWith);

        if (path.endsWith(buff))
        {
          strcpy(buff, mimeTable[i].mimeType);
          return String(buff);
        }
      }

      // Fall-through and just return default type
      strcpy(buff, mimeTable[sizeof(mimeTable) / sizeof(mimeTable[0]) - 1].mimeType);
      return String(buff);
    }

#else

    static String getContentType(const String& path)
    {
      if (path.endsWith(".html"))           return "text/html";
      else if (path.endsWith(".htm"))       return "text/html";
      else if (path.endsWith(".css"))       return "text/css";
      else if (path.endsWith(".txt"))       return "text/plain";
      else if (path.endsWith(".js"))        return "application/javascript";
      else if (path.endsWith(".png"))       return "image/png";
      else if (path.endsWith(".gif"))       return "image/gif";
      else if (path.endsWith(".jpg"))       return "image/jpeg";
      else if (path.endsWith(".ico"))       return "image/x-icon";
      else if (path.endsWith(".svg"))       return "image/svg+xml";
      else if (path.endsWith(".ttf"))       return "application/x-font-ttf";
      else if (path.endsWith(".otf"))       return "application/x-font-opentype";
      else if (path.endsWith(".woff"))      return "application/font-woff";
      else if (path.endsWith(".woff2"))     return "application/font-woff2";
      else if (path.endsWith(".eot"))       return "application/vnd.ms-fontobject";
      else if (path.endsWith(".sfnt"))      return "application/font-sfnt";
      else if (path.endsWith(".xml"))       return "text/xml";
      else if (path.endsWith(".pdf"))       return "application/pdf";
      else if (path.endsWith(".zip"))       return "application/zip";
      else if (path.endsWith(".gz"))        return "application/x-gzip";
      else if (path.endsWith(".appcache"))  return "text/cache-manifest";

      return "application/octet-stream";
    }

#endif

  protected:

    String _uri;
    String _path;
    String _cache_header;
    bool _isFile;
    size_t _baseUriLength;
};

