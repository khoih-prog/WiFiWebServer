/****************************************************************************************************************************
   WiFiRequestHandler.h - Dead simple web-server.
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

#ifndef WiFiRequestHandler_h
#define WiFiRequestHandler_h

class RequestHandler
{
  public:
    virtual ~RequestHandler() { }
    virtual bool canHandle(HTTPMethod method, String uri) {
      return false;
    }
    virtual bool canUpload(String uri) {
      return false;
    }
    virtual bool handle(WiFiWebServer& server, HTTPMethod requestMethod, String requestUri) {
      return false;
    }
    virtual void upload(WiFiWebServer& server, String requestUri, HTTPUpload& upload) {}

    RequestHandler* next() {
      return _next;
    }
    void next(RequestHandler* r) {
      _next = r;
    }

  private:
    RequestHandler* _next = nullptr;
};

#endif //WiFiRequestHandler_h
