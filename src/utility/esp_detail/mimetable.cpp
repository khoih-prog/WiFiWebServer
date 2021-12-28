/****************************************************************************************************************************
  mimetable.cpp - Dead simple web-server.
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

#if (ESP32 || ESP8266)

#include "mimetable.h"
#include "pgmspace.h"
#include "WString.h"

namespace mime_esp
{
  static const char kHtmlSuffix[]     PROGMEM = ".html";
  static const char kHtmSuffix[]      PROGMEM = ".htm";
  static const char kTxtSuffix[]      PROGMEM = ".txt";
  
  #ifndef MIMETYPE_MINIMAL
  static const char kCssSuffix[]      PROGMEM = ".css";
  static const char kJsSuffix[]       PROGMEM = ".js";
  static const char kJsonSuffix[]     PROGMEM = ".json";
  static const char kPngSuffix[]      PROGMEM = ".png";
  static const char kGifSuffix[]      PROGMEM = ".gif";
  static const char kJpgSuffix[]      PROGMEM = ".jpg";
  static const char kJpegSuffix[]     PROGMEM = ".jpeg";
  static const char kIcoSuffix[]      PROGMEM = ".ico";
  static const char kSvgSuffix[]      PROGMEM = ".svg";
  static const char kTtfSuffix[]      PROGMEM =  ".ttf";
  static const char kOtfSuffix[]      PROGMEM = ".otf";
  static const char kWoffSuffix[]     PROGMEM = ".woff";
  static const char kWoff2Suffix[]    PROGMEM = ".woff2";
  static const char kEotSuffix[]      PROGMEM = ".eot";
  static const char kSfntSuffix[]     PROGMEM = ".sfnt";
  static const char kXmlSuffix[]      PROGMEM = ".xml";
  static const char kPdfSuffix[]      PROGMEM = ".pdf";
  static const char kZipSuffix[]      PROGMEM = ".zip";
  static const char kAppcacheSuffix[] PROGMEM = ".appcache";
  #endif // MIMETYPE_MINIMAL
  
  static const char kGzSuffix[]       PROGMEM = ".gz";
  static const char kDefaultSuffix[]  PROGMEM = "";
  
  static const char kHtml[]           PROGMEM = "text/html";
  static const char kTxt[]            PROGMEM = "text/plain";
  
  #ifndef MIMETYPE_MINIMAL
  static const char kCss[]            PROGMEM = "text/css";
  static const char kJs[]             PROGMEM = "application/javascript";
  static const char kJson[]           PROGMEM = "application/json";
  static const char kPng[]            PROGMEM = "image/png";
  static const char kGif[]            PROGMEM = "image/gif";
  static const char kJpg[]            PROGMEM = "image/jpeg";
  static const char kJpeg[]           PROGMEM = "image/jpeg";
  static const char kIco[]            PROGMEM = "image/x-icon";
  static const char kSvg[]            PROGMEM = "image/svg+xml";
  static const char kTtf[]            PROGMEM = "application/x-font-ttf";
  static const char kOtf[]            PROGMEM = "application/x-font-opentype";
  static const char kWoff[]           PROGMEM = "application/font-woff";
  static const char kWoff2[]          PROGMEM = "application/font-woff2";
  static const char kEot[]            PROGMEM = "application/vnd.ms-fontobject";
  static const char kSfnt[]           PROGMEM = "application/font-sfnt";
  static const char kXml[]            PROGMEM = "text/xml";
  static const char kPdf[]            PROGMEM = "application/pdf";
  static const char kZip[]            PROGMEM = "application/zip";
  static const char kAppcache[]       PROGMEM = "text/cache-manifest";
  #endif // MIMETYPE_MINIMAL
  
  static const char kGz[]             PROGMEM = "application/x-gzip";
  static const char kDefault[]        PROGMEM = "application/octet-stream";
  
  const Entry mimeTable[maxType]      PROGMEM =
  {
    { kHtmlSuffix,      kHtml },
    { kHtmSuffix,       kHtml },
    { kTxtSuffix,       kTxt },
  #ifndef MIMETYPE_MINIMAL
    { kCssSuffix,       kCss },
    { kJsSuffix,        kJs },
    { kJsonSuffix,      kJson },
    { kPngSuffix,       kPng },
    { kGifSuffix,       kGif },
    { kJpgSuffix,       kJpg },
    { kJpegSuffix,      kJpeg },
    { kIcoSuffix,       kIco },
    { kSvgSuffix,       kSvg },
    { kTtfSuffix,       kTtf },
    { kOtfSuffix,       kOtf },
    { kWoffSuffix,      kWoff },
    { kWoff2Suffix,     kWoff2 },
    { kEotSuffix,       kEot },
    { kSfntSuffix,      kSfnt },
    { kXmlSuffix,       kXml },
    { kPdfSuffix,       kPdf },
    { kZipSuffix,       kZip },
    { kAppcacheSuffix,  kAppcache },
  #endif // MIMETYPE_MINIMAL
    { kGzSuffix,        kGz },
    { kDefaultSuffix,   kDefault }
  };
  
  String getContentType(const String& path) 
  {
    for (size_t i = 0; i < maxType; i++) 
    {
      if (path.endsWith(FPSTR(mimeTable[i].endsWith))) 
      {
        return String(FPSTR(mimeTable[i].mimeType));
      }
    }
    
    // Fall-through and just return default type
    return String(FPSTR(kDefault));
  }

}   // namespace mime_esp

#endif
