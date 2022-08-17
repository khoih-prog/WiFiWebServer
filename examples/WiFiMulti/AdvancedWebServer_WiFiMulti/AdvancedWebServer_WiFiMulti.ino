/**************************************************************************************************************************************
  AdvancedWebServer_WiFiMulti.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
  
  Copyright (c) 2015, Majenko Technologies
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
  
  Neither the name of Majenko Technologies nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************************************************************************/

#include "defines.h"

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiWebServer server(80);

WiFiMulti_Generic wifiMulti;

#if ( defined(ARDUINO_RASPBERRY_PI_PICO_W) )

// Klugde to temporarily fix RP2040W WiFi.status() bug ( https://github.com/earlephilhower/arduino-pico/issues/762 )

bool WiFiConnected = false;

bool isWiFiConnected()
{
  // You can change longer or shorter depending on your network response
  // Shorter => more responsive, but more ping traffic
  static uint8_t theTTL = 10;

  // Use ping() to test TCP connections
  if (WiFi.ping(WiFi.gatewayIP(), theTTL) == theTTL)
  {
    WFM_LOGINFO1("Client connected, Local IP = ", WiFi.localIP());
    WiFiConnected = true;
  
    return true;
  }

  WFM_LOGINFO1("Connection failed. Local IP = ", WiFi.localIP());
  WiFiConnected = false;

  return false;
}

#endif

#if defined(LED_BUILTIN)
  const int led =  LED_BUILTIN;
#else
  #if (ESP32)
    // Using pin 13 will crash ESP32_C3
    const int led =  2;
  #else
    const int led =  13;
  #endif
#endif

void handleRoot()
{
#define BUFFER_SIZE     512
  
  digitalWrite(led, 1);
  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  hr = hr % 24;

  snprintf(temp, BUFFER_SIZE - 1,
           "<html>\
<head>\
<meta http-equiv='refresh' content='5'/>\
<title>%s</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>Hello from %s</h1>\
<h3>running WiFiWebServer WiFiMulti</h3>\
<h3>on %s</h3>\
<p>Uptime: %d d %02d:%02d:%02d</p>\
<img src=\"/test.svg\" />\
</body>\
</html>", BOARD_NAME, BOARD_NAME, SHIELD_TYPE, day, hr, min % 60, sec % 60);

  server.send(200, F("text/html"), temp);
  digitalWrite(led, 0);
}

void handleNotFound()
{
  digitalWrite(led, 1);
  
  String message = F("File Not Found\n\n");
  
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, F("text/plain"), message);
  
  digitalWrite(led, 0);
}

#define ORIGINAL_STR_LEN        2048

void drawGraph()
{
  static String out;
  static uint16_t previousStrLen = ORIGINAL_STR_LEN;

  if (out.length() == 0)
  {
    WS_LOGWARN1(F("String Len = 0, extend to"), ORIGINAL_STR_LEN);
    out.reserve(ORIGINAL_STR_LEN);
  }

  out = F( "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"310\" height=\"150\">\n" \
           "<rect width=\"310\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"3\" stroke=\"rgb(0, 0, 0)\" />\n" \
           "<g stroke=\"blue\">\n");

  char temp[70];
  
  int y = rand() % 130;

  for (int x = 10; x < 300; x += 10)
  {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  
  out += F("</g>\n</svg>\n");

  WS_LOGDEBUG1(F("String Len = "), out.length());

  if (out.length() > previousStrLen)
  {
    WS_LOGERROR3(F("String Len > "), previousStrLen, F(", extend to"), out.length() + 48);

    previousStrLen = out.length() + 48;
    
    out.reserve(previousStrLen);
  }
  else
  {
    server.send(200, "image/svg+xml", out);
  }
}

uint8_t connectMultiWiFi()
{
#if defined(ESP32)
  // For ESP32, this better be 0 to shorten the connect time.
  // For ESP32-S2/C3, must be > 500
  #if ( USING_ESP32_S2 || USING_ESP32_C3 )
    #define WIFI_MULTI_1ST_CONNECT_WAITING_MS           500L
  #else
    // For ESP32 core v1.0.6, must be >= 500
    #define WIFI_MULTI_1ST_CONNECT_WAITING_MS           800L
  #endif
#elif (defined(ESP8266))
  // For ESP8266, this better be 2200 to enable connect the 1st time
  #define WIFI_MULTI_1ST_CONNECT_WAITING_MS             2200L
#else
  // For general board, this better be 1000 to enable connect the 1st time
  #define WIFI_MULTI_1ST_CONNECT_WAITING_MS             1000L
#endif

#define WIFI_MULTI_CONNECT_WAITING_MS                   500L

  Serial.println("WiFi lost. Trying to scan and reconnect");

  WiFi.disconnect();

  int i = 0;

  uint8_t status = wifiMulti.run();

  delay(WIFI_MULTI_1ST_CONNECT_WAITING_MS);

  while ( ( i++ < 20 ) && ( status != WL_CONNECTED ) )
  {
    status = WiFi.status();

    if ( status == WL_CONNECTED )
      break;
    else
      delay(WIFI_MULTI_CONNECT_WAITING_MS);
  }

  if ( status == WL_CONNECTED )
  {
    WFM_LOGERROR1(F("WiFi connected after time: "), i);
    WFM_LOGERROR3(F("SSID:"), WiFi.SSID(), F(",RSSI="), WiFi.RSSI());

#if (defined(ESP32) || defined(ESP8266))
    WFM_LOGERROR3(F("Channel:"), WiFi.channel(), F(",IP address:"), WiFi.localIP() );
#else
    WFM_LOGERROR1(F("IP address:"), WiFi.localIP() );
#endif
  }
  else
  {
    WFM_LOGERROR(F("WiFi not connected"));

    if (wifiMulti.run() != WL_CONNECTED)
    {
      Serial.println("WiFi not connected!");
      delay(1000);
    }
  }

  return status;
}

void check_WiFi()
{
#if ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  // Workaround for bug in https://github.com/arduino/ArduinoCore-mbed/issues/381
  if ( (WiFi.status() != WL_CONNECTED) || (WiFi.RSSI() == 0) )
#elif ( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
  if (!isWiFiConnected())
#else
  if ( (WiFi.status() != WL_CONNECTED) )
#endif
  {
    Serial.println(F("\nWiFi lost. Call connectMultiWiFi in loop"));
    connectMultiWiFi();
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  // you're connected now, so print out the data
  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(WiFi.localIP());

  Serial.print(F("SSID: "));
  Serial.print(WiFi.SSID());

  // print the received signal strength:
  int32_t rssi = WiFi.RSSI();
  Serial.print(F(", Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  delay(200);

  Serial.print(F("\nStarting AdvancedWebServer_WiFiMulti on ")); Serial.print(BOARD_NAME);
  Serial.print(F(" with ")); Serial.println(SHIELD_TYPE); 
  Serial.println(WIFIMULTI_GENERIC_VERSION);
  Serial.println(WIFI_WEBSERVER_VERSION);

#if WIFI_USING_ESP_AT

  // initialize serial for ESP module
  EspSerial.begin(115200);
  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println(F("WiFi shield init done"));
  
#endif  

#if ! (ESP32 || ESP8266)
  // check for the presence of the shield
#if USE_WIFI_NINA
  if (WiFi.status() == WL_NO_MODULE)
#else
  if (WiFi.status() == WL_NO_SHIELD)
#endif
  {
    Serial.println(F("WiFi shield not present"));
    // don't continue
    while (true);
  }
#endif

#if USE_WIFI_NINA
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println(F("Please upgrade the firmware"));
  }
#endif

  wifiMulti.addAP(your_ssid1, your_pass1);
  wifiMulti.addAP(your_ssid2, your_pass2);
  //wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");
  //wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  //wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting WiFi...");

  if (wifiMulti.run() == WL_CONNECTED)
  {
    Serial.print("\nWiFi connected, IP address: ");
    Serial.println(WiFi.localIP());
  }

  // you're connected now, so print out the data
  printWifiStatus();

  server.on(F("/"), handleRoot);
  server.on(F("/test.svg"), drawGraph);
  server.on(F("/inline"), []()
  {
    server.send(200, F("text/plain"), F("This works as well"));
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());
}

void heartBeatPrint()
{
  static int num = 1;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  if (WiFiConnected)
#else  
  if (WiFi.status() == WL_CONNECTED)
#endif
    Serial.print(F("H"));        // H means connected to WiFi
  else
    Serial.print(F("F"));        // F means not connected to WiFi

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static uint32_t checkstatus_timeout  = 0;
  static uint32_t checkwifi_timeout    = 0;

  static uint32_t current_millis;

#if ( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
  #define WIFICHECK_INTERVAL    10000L
#else
  #define WIFICHECK_INTERVAL    1000L
#endif

#define HEARTBEAT_INTERVAL    10000L

  current_millis = millis();

  // Check WiFi every WIFICHECK_INTERVAL (1) seconds.
  if ((current_millis > checkwifi_timeout) || (checkwifi_timeout == 0))
  {
    check_WiFi();
    checkwifi_timeout = current_millis + WIFICHECK_INTERVAL;
  }

  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((current_millis > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = current_millis + HEARTBEAT_INTERVAL;
  }

#if defined(CONFIG_PLATFORM_8721D)
  // Important delay() for RTL8720DN
  delay(200);
#endif
}

void loop()
{
  server.handleClient();
  check_status();
}
