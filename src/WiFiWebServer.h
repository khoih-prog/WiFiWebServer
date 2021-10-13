/**************************************************************************************************************************************
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

#define WIFI_WEBSERVER_VERSION          "WiFiWebServer v1.4.2"

#define WIFI_WEBSERVER_VERSION_MAJOR    1
#define WIFI_WEBSERVER_VERSION_MINOR    4
#define WIFI_WEBSERVER_VERSION_PATCH    2

#define WEBSOCKETS2_GENERIC_VERSION_INT      1004002

#define USE_NEW_WEBSERVER_VERSION     true

#if ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #if defined(WIFI_USE_PORTENTA_H7)
    #undef WIFI_USE_PORTENTA_H7
  #endif
  #define WIFI_USE_PORTENTA_H7        true
  
  #if defined(USE_NEW_WEBSERVER_VERSION)
    #undef USE_NEW_WEBSERVER_VERSION
  #endif
  #define USE_NEW_WEBSERVER_VERSION   false
  
  #warning Use mbed-portenta architecture for PORTENTA_H7 from WiFiWebServer
#endif

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )
  #if defined(WIFI_USE_SAMD)
    #undef WIFI_USE_SAMD
  #endif
  #define WIFI_USE_SAMD      true
  #warning Use SAMD architecture from WiFiWebServer
#endif

#if (defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
     defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
     defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
     defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
  #if defined(WIFI_USE_NRF528XX)
    #undef WIFI_USE_NRF528XX
  #endif
  #define WIFI_USE_NRF528XX      true
  #warning Use nFR52 architecture from WiFiWebServer
  
  #include <Adafruit_TinyUSB.h>

#endif

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #if defined(WIFI_USE_SAM_DUE)
    #undef WIFI_USE_SAM_DUE
  #endif
  #define WIFI_USE_SAM_DUE      true
  #warning Use SAM_DUE architecture from WiFiWebServer
#endif

#if ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) ) && !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #warning STM32F/L/H/G/WB/MP1 board selected

  #if defined(WIFI_USE_STM32)
    #undef WIFI_USE_STM32
  #endif
  #define WIFI_USE_STM32      true
  #warning Use STM32 architecture from WiFiWebServer
#endif

#if ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || \
      defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) )
  #warning RP2040-based board selected

  #if defined(WIFI_USE_RP2040)
    #undef WIFI_USE_RP2040
  #endif
  #define WIFI_USE_RP2040      true
  #warning Use RP2040 architecture from WiFiWebServer
#endif

// To support lambda function in class
#include <functional-vlpp.h>

#if !defined(USE_WIFI_NINA)
  #define USE_WIFI_NINA     true
#endif

// Modify to use new WiFiNINA_Generic library to support boards besides Nano-33 IoT, MKRWiFi1010, Adafruit MetroM4, etc.
#if USE_WIFI_NINA
  #include <WiFiNINA_Generic.h>
  //#include <WiFiNINA.h>
  #warning Use WiFiNINA from WiFiWebServer
#elif USE_WIFI101
  #include <WiFi101.h>
  #warning Use WiFi101 from WiFiWebServer
#else
  #if USE_WIFI_CUSTOM
  #warning Use Custom WiFi for WiFiWebServer
#else
  #include <WiFi.h>
  #warning Use WiFi.h from WiFiWebServer
#endif

#endif

#include "utility/mimetable.h"
#include "utility/RingBuffer.h"

// KH, For PROGMEM commands
// ESP32/ESP8266 includes <pgmspace.h> by default, and memccpy_P was already defined there
#if !(ESP32 || ESP8266 || defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4))
  #include <avr/pgmspace.h>
  #define memccpy_P(dest, src, c, n) memccpy((dest), (src), (c), (n))
#endif

// Permit redefinition of SENDCONTENT_P_BUFFER_SZ in sketch, default is 4K, minimum is 256 bytes
#ifndef SENDCONTENT_P_BUFFER_SZ
  #define SENDCONTENT_P_BUFFER_SZ     4096
  #warning SENDCONTENT_P_BUFFER_SZ using default 4 Kbytes
#else
  #if (SENDCONTENT_P_BUFFER_SZ < 256)
    #undef SENDCONTENT_P_BUFFER_SZ
    #define SENDCONTENT_P_BUFFER_SZ   256
    #warning SENDCONTENT_P_BUFFER_SZ reset to min 256 bytes
  #endif
#endif

#ifndef PGM_VOID_P
  #define PGM_VOID_P const void *
#endif

//////

enum HTTPMethod 
{ 
  HTTP_ANY, 
  HTTP_GET,
  HTTP_HEAD,
  HTTP_POST, 
  HTTP_PUT, 
  HTTP_PATCH, 
  HTTP_DELETE, 
  HTTP_OPTIONS 
};

enum HTTPUploadStatus 
{ 
  UPLOAD_FILE_START, 
  UPLOAD_FILE_WRITE, 
  UPLOAD_FILE_END,
  UPLOAD_FILE_ABORTED
};

enum HTTPClientStatus 
{ 
  HC_NONE, 
  HC_WAIT_READ, 
  HC_WAIT_CLOSE 
};

enum HTTPAuthMethod 
{ 
  BASIC_AUTH, 
  DIGEST_AUTH 
};

#define HTTP_DOWNLOAD_UNIT_SIZE 1460

// Permit user to increase HTTP_UPLOAD_BUFLEN larger than default 2K
//#define HTTP_UPLOAD_BUFLEN 2048
#if !defined(HTTP_UPLOAD_BUFLEN)
  #define HTTP_UPLOAD_BUFLEN 2048
#endif

#define HTTP_MAX_DATA_WAIT    5000 //ms to wait for the client to send the request
#define HTTP_MAX_POST_WAIT    5000 //ms to wait for POST data to arrive
#define HTTP_MAX_SEND_WAIT    5000 //ms to wait for data chunk to be ACKed
#define HTTP_MAX_CLOSE_WAIT   2000 //ms to wait for the client to close the connection

#define CONTENT_LENGTH_UNKNOWN ((size_t) -1)
#define CONTENT_LENGTH_NOT_SET ((size_t) -2)

class WiFiWebServer;

typedef struct 
{
  HTTPUploadStatus status;
  String  filename;
  String  name;
  String  type;
  size_t  totalSize;      // file size
  size_t  currentSize;    // size of data currently in buf
  size_t  contentLength;  // size of entire post request, file size + headers and other request data.
  uint8_t buf[HTTP_UPLOAD_BUFLEN];
} HTTPUpload;

#include "utility/RequestHandler.h"

class WiFiWebServer
{
  public:
    WiFiWebServer(int port = 80);
    ~WiFiWebServer();

    void begin();
    void handleClient();

    void close();
    void stop();

    bool authenticate(const char * username, const char * password);
    void requestAuthentication();

    typedef vl::Func<void(void)> THandlerFunction;
    //typedef std::function<void(void)> THandlerFunction;
    //typedef void (*THandlerFunction)(void);

    void on(const String &uri, THandlerFunction handler);
    void on(const String &uri, HTTPMethod method, THandlerFunction fn);
    void on(const String &uri, HTTPMethod method, THandlerFunction fn, THandlerFunction ufn);
    void addHandler(RequestHandler* handler);
    void onNotFound(THandlerFunction fn);   //called when handler is not assigned
    void onFileUpload(THandlerFunction fn); //handle file uploads

    String uri() 
    {
      return _currentUri;
    }
    
    HTTPMethod method() 
    {
      return _currentMethod;
    }
    
    WiFiClient client() 
    {
      return _currentClient;
    }
    
    //KH
    #if USE_NEW_WEBSERVER_VERSION
    HTTPUpload& upload() 
    {
      return *_currentUpload;
    }
    #else
    HTTPUpload& upload() 
    {
      return _currentUpload;
    }
    #endif
    
    String arg(String name);            // get request argument value by name
    String arg(int i);                  // get request argument value by number
    String argName(int i);              // get request argument name by number
    int args();                         // get arguments count
    bool hasArg(String name);           // check if argument exists
    void collectHeaders(const char* headerKeys[], const size_t headerKeysCount); // set the request headers to collect
    String header(String name);         // get request header value by name
    String header(int i);               // get request header value by number
    String headerName(int i);           // get request header name by number
    int headers();                      // get header count
    bool hasHeader(String name);        // check if header exists

    String hostHeader();                // get request host header if available or empty String if not

    // send response to the client
    // code - HTTP response code, can be 200 or 404
    // content_type - HTTP content type, like "text/plain" or "image/png"
    // content - actual content body
    void send(int code, const char* content_type = NULL, const String& content = String(""));
    void send(int code, char* content_type, const String& content);
    void send(int code, const String& content_type, const String& content);
    //KH
    void send(int code, char*  content_type, const String& content, size_t contentLength);
    
    void setContentLength(size_t contentLength);
    void sendHeader(const String& name, const String& value, bool first = false);
    void sendContent(const String& content);
    void sendContent(const String& content, size_t size);

    // KH, Restore PROGMEM commands
    void send_P(int code, PGM_P content_type, PGM_P content);
    void send_P(int code, PGM_P content_type, PGM_P content, size_t contentLength);
    
    void sendContent_P(PGM_P content);
    void sendContent_P(PGM_P content, size_t size);
    //////

    static String urlDecode(const String& text);

    template<typename T> size_t streamFile(T &file, const String& contentType) 
    {
      using namespace mime;
      setContentLength(file.size());
      
      if (String(file.name()).endsWith(mimeTable[gz].endsWith) && contentType != mimeTable[gz].mimeType && contentType != mimeTable[none].mimeType) 
      {
        sendHeader("Content-Encoding", "gzip");
      }
      
      send(200, contentType, "");
      return _currentClient.write(file);
    }

  protected:
    void _addRequestHandler(RequestHandler* handler);
    void _handleRequest();
    void _finalizeResponse();
    bool _parseRequest(WiFiClient& client);
    
    //KH
    #if USE_NEW_WEBSERVER_VERSION
    void _parseArguments(const String& data);
    int  _parseArgumentsPrivate(const String& data, vl::Func<void(String&,String&,const String&,int,int,int,int)> handler);
    bool _parseForm(WiFiClient& client, const String& boundary, uint32_t len);
    #else
    void _parseArguments(String data);
    bool _parseForm(WiFiClient& client, String boundary, uint32_t len);
    #endif
    
    static String _responseCodeToString(int code);    
    bool _parseFormUploadAborted();
    void _uploadWriteByte(uint8_t b);
    uint8_t _uploadReadByte(WiFiClient& client);
    void _prepareHeader(String& response, int code, const char* content_type, size_t contentLength);
    bool _collectHeader(const char* headerName, const char* headerValue);
    
    struct RequestArgument 
    {
      String key;
      String value;
    };

    WiFiServer  _server;

    WiFiClient        _currentClient;
    HTTPMethod        _currentMethod;
    String            _currentUri;
    uint8_t           _currentVersion;
    HTTPClientStatus  _currentStatus;
    unsigned long     _statusChange;

    RequestHandler*   _currentHandler   = nullptr;
    RequestHandler*   _firstHandler     = nullptr;
    RequestHandler*   _lastHandler      = nullptr;
    THandlerFunction  _notFoundHandler;
    THandlerFunction  _fileUploadHandler;

    int               _currentArgCount;
    RequestArgument*  _currentArgs      = nullptr;

    //KH = nullptr
    #if USE_NEW_WEBSERVER_VERSION
    HTTPUpload*       _currentUpload    = nullptr;
    int               _postArgsLen;
    RequestArgument*  _postArgs         = nullptr;
    
    #else
    HTTPUpload        _currentUpload;
    #endif
    
    int               _headerKeysCount;
    RequestArgument*  _currentHeaders   = nullptr;
    size_t            _contentLength;
    String            _responseHeaders;

    String            _hostHeader;
    bool              _chunked;
};

#include "WiFiWebServer-impl.h"
#include "Parsing-impl.h"

