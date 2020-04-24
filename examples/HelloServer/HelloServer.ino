/****************************************************************************************************************************
   HelloServer.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
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
                                    Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc. 
    1.0.4   K Hoang      23/04/2020 Add support to MKR1000 boards using WiFi101 and custom WiFi libraries.
 *****************************************************************************************************************************/
#define DEBUG_WIFI_WEBSERVER_PORT Serial

#define USE_WIFI_NINA           true

#if defined(ARDUINO_SAMD_MKR1000)
  #if defined(USE_WIFI_NINA)
    #undef USE_WIFI_NINA
  #endif
  #define USE_WIFI_NINA         false
  #define USE_WIFI101           true
#endif

#if    ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) )
  #if defined(WIFI_USE_NRF528XX)
    #undef WIFI_USE_NRF528XX
  #endif
  #define WIFI_USE_NRF528XX          true
#endif

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD21G18A__) )
#if defined(WIFI_USE_SAMD)
#undef WIFI_USE_SAMD
#endif
#define WIFI_USE_SAMD      true
#endif

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
#if defined(WIFI_USE_SAM_DUE)
#undef WIFI_USE_SAM_DUE
#endif
#define WIFI_USE_SAM_DUE      true
#warning Use SAM_DUE architecture
#endif

#if ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) )
#if defined(WIFI_USE_STM32)
#undef WIFI_USE_STM32
#endif
#define WIFI_USE_STM32      true
#endif

#ifdef CORE_TEENSY
#if defined(__IMXRT1062__)
// For Teensy 4.0
#define BOARD_TYPE      "TEENSY 4.0"
#elif defined(__MK66FX1M0__)
#define BOARD_TYPE "Teensy 3.6"
#elif defined(__MK64FX512__)
#define BOARD_TYPE "Teensy 3.5"
#elif defined(__MKL26Z64__)
#define BOARD_TYPE "Teensy LC"
#elif defined(__MK20DX256__)
#define BOARD_TYPE "Teensy 3.2" // and Teensy 3.1 (obsolete)
#elif defined(__MK20DX128__)
#define BOARD_TYPE "Teensy 3.0"
#elif defined(__AVR_AT90USB1286__)
#error Teensy 2.0++ not supported yet
#elif defined(__AVR_ATmega32U4__)
#error Teensy 2.0 not supported yet
#else
// For Other Boards
#define BOARD_TYPE      "Unknown Teensy Board"
#endif

#elif defined(WIFI_USE_NRF528XX)
#if defined(NRF52840_FEATHER)
#define BOARD_TYPE      "NRF52840_FEATHER"
#elif defined(NRF52832_FEATHER)
#define BOARD_TYPE      "NRF52832_FEATHER"
#else
#define BOARD_TYPE      "NRF52 Unknown"
#endif

#elif defined(WIFI_USE_SAMD)
#if defined(ARDUINO_SAMD_ZERO)
#define BOARD_TYPE      "SAMD Zero"
#elif defined(ARDUINO_SAMD_MKR1000)
#define BOARD_TYPE      "SAMD MKR1000"
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
#define BOARD_TYPE      "SAMD MKRWIFI1010"
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
#define BOARD_TYPE      "SAMD NANO_33_IOT"
#elif defined(ARDUINO_SAMD_MKRFox1200)
#define BOARD_TYPE      "SAMD MKRFox1200"
#elif ( defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) )
#define BOARD_TYPE      "SAMD MKRWAN13X0"
#elif defined(ARDUINO_SAMD_MKRGSM1400)
#define BOARD_TYPE      "SAMD MKRGSM1400"
#elif defined(ARDUINO_SAMD_MKRNB1500)
#define BOARD_TYPE      "SAMD MKRNB1500"
#elif defined(ARDUINO_SAMD_MKRVIDOR4000)
#define BOARD_TYPE      "SAMD MKRVIDOR4000"
#elif defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS)
#define BOARD_TYPE      "SAMD ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS"
#elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
#define BOARD_TYPE      "SAMD ADAFRUIT_ITSYBITSY_M4_EXPRESS"
#elif defined(__SAMD21E18A__)
#define BOARD_TYPE      "SAMD21E18A"
#elif defined(__SAMD21G18A__)
#define BOARD_TYPE      "SAMD21G18A"
#elif defined(__SAMD51G19A__)
#define BOARD_TYPE      "SAMD51G19A"
#elif defined(__SAMD51J19A__)
#define BOARD_TYPE      "SAMD51J19A"
#elif defined(__SAMD51J20A__)
#define BOARD_TYPE      "SAMD51J20A"
#elif defined(__SAM3X8E__)
#define BOARD_TYPE      "SAM3X8E"
#elif defined(__CPU_ARC__)
#define BOARD_TYPE      "CPU_ARC"
#elif defined(__SAMD51__)
#define BOARD_TYPE      "SAMD51"
#else
#define BOARD_TYPE      "SAMD Unknown"
#endif

#elif defined(WIFI_USE_SAM_DUE)

#if defined(ARDUINO_SAM_DUE)
#define BOARD_TYPE      "SAM DUE"
#elif defined(__SAM3X8E__)
#define BOARD_TYPE      "SAM SAM3X8E"
#else
#define BOARD_TYPE      "SAM Unknown"
#endif

#elif defined(WIFI_USE_STM32)
#if defined(STM32F0)
#define BOARD_TYPE  "STM32F0"
#elif defined(STM32F1)
#define BOARD_TYPE  "STM32F1"
#elif defined(STM32F2)
#define BOARD_TYPE  "STM32F2"
#elif defined(STM32F3)
#define BOARD_TYPE  "STM32F3"
#elif defined(STM32F4)
#define BOARD_TYPE  "STM32F4"
#elif defined(STM32F7)
#define BOARD_TYPE  "STM32F7"
#else
#warning STM32 unknown board selected
#define BOARD_TYPE  "STM32 Unknown"
#endif
#else
#define BOARD_TYPE      "AVR Mega"
#endif

#include <WiFiWebServer.h>

char ssid[] = "****";        // your network SSID (name)
char pass[] = "****";        // your network password

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiWebServer server(80);

const int led = 13;

void handleRoot()
{
  server.send(200, "text/plain", "Hello from WiFiNINA_WebServer!");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void)
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting HelloServer on " + String(BOARD_TYPE));

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

#if USE_WIFI_NINA
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
  {
    Serial.println("Please upgrade the firmware");
  }
#endif

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    Serial.print(F("Connecting to WPA SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  server.begin();

  server.on("/", handleRoot);

  server.on("/inline", []()
  {
    server.send(200, "text/plain", "This works as well");
  });

  server.onNotFound(handleNotFound);

  //server.begin();

  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());
}

void loop(void)
{
  server.handleClient();
}
