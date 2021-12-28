/****************************************************************************************************************************
  ESP_RequestHandlersImpl.h - Dead simple web-server.
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.5.3

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
  1.5.0   K Hoang      19/12/2021 Reduce usage of Arduino String with std::string
  1.5.1   K Hoang      25/12/2021 Fix bug
  1.5.2   K Hoang      27/12/2021 Fix wrong http status header bug
  1.5.3   K Hoang      27/12/2021 Fix authenticate issue caused by libb64
 *************************************************************************************************************************************/

#pragma once

#include "RequestHandler.h"
#include "esp_detail/mimetable.h"
#include "FS.h"
#include "WString.h"
#include <MD5Builder.h>
#include <base64.h>

class FunctionRequestHandler : public RequestHandler
{
  public:

    FunctionRequestHandler(WiFiWebServer::THandlerFunction fn, WiFiWebServer::THandlerFunction ufn, const String &uri, const HTTPMethod& method)
      : _fn(fn)
      , _ufn(ufn)
      , _uri(uri)
      , _method(method)
    {
    }

    bool canHandle(const HTTPMethod& requestMethod, const String& requestUri) override
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

    bool canUpload(const String& requestUri) override
    {
      if (!_ufn || !canHandle(HTTP_POST, requestUri))
        return false;

      return true;
    }

    bool handle(WiFiWebServer& server, const HTTPMethod& requestMethod, const String& requestUri) override
    {
      WFW_UNUSED(server);
      
      if (!canHandle(requestMethod, requestUri))
        return false;

      _fn();
      return true;
    }

    void upload(WiFiWebServer& server, const String& requestUri, const HTTPUpload& upload) override
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
    using WebServerType = WiFiWebServer;
    
public:
    StaticRequestHandler(FS& fs, const char* path, const char* uri, const char* cache_header)
    : _fs(fs)
    , _uri(uri)
    , _path(path)
    , _cache_header(cache_header)
    {
        //DEBUGV("StaticRequestHandler: path=%s uri=%s, cache_header=%s\r\n", path, uri, cache_header == __null ? "" : cache_header);
        _isFile = fs.exists(path);
        _baseUriLength = _uri.length();
    }

    bool validMethod(HTTPMethod requestMethod){
        return (requestMethod == HTTP_GET) || (requestMethod == HTTP_HEAD);
    }

    /* Deprecated version. Please use mime::getContentType instead */
    static String getContentType(const String& path) __attribute__((deprecated)) 
    {
        return mime_esp::getContentType(path);
    }

protected:
    FS _fs;
    bool _isFile;
    String _uri;
    String _path;
    String _cache_header;
    size_t _baseUriLength;
};


class StaticFileRequestHandler
    :
public StaticRequestHandler 
{
    using SRH = StaticRequestHandler;
    using WebServerType = WiFiWebServer;

public:
    StaticFileRequestHandler(FS& fs, const char* path, const char* uri, const char* cache_header)
        :
    StaticRequestHandler{fs, path, uri, cache_header}
    {
        File f = SRH::_fs.open(path, "r");
        MD5Builder calcMD5;
        calcMD5.begin();
        calcMD5.addStream(f, f.size());
        calcMD5.calculate();
        calcMD5.getBytes(_ETag_md5);
        f.close();
    }

    bool canHandle(const HTTPMethod& requestMethod, const String& requestUri) override  
    {
        return SRH::validMethod(requestMethod) && requestUri == SRH::_uri;
    }

    bool handle(WiFiWebServer& server, const HTTPMethod& requestMethod, const String& requestUri) 
    {
        if (!canHandle(requestMethod, requestUri))
            return false;

        
        const String etag = "\"" + base64::encode(_ETag_md5, 16) + "\"";

        if(server.header("If-None-Match") == etag){
            server.send(304);
            return true;
        }

        File f = SRH::_fs.open(SRH::_path, "r");

        if (!f)
            return false;

        if (!_isFile) {
            f.close();
            return false;
        }

        if (SRH::_cache_header.length() != 0)
            server.sendHeader("Cache-Control", SRH::_cache_header);

        server.sendHeader("ETag", etag);

        server.streamFile(f, mime_esp::getContentType(SRH::_path), requestMethod);
        return true;
    }

protected:
    uint8_t _ETag_md5[16];
};

