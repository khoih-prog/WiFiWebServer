/****************************************************************************************************************************
    ScanNetworks.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
   For ESP32-based WiFi shields, such as WiFiNINA W101, W102, W13x, etc

   WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
   Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
   Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
   Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
   Licensed under MIT license
   Version: 1.0.1

    This example  prints the Wifi shield's MAC address, and
    scans for available Wifi networks using the Wifi shield.
    Every ten seconds, it scans again. It doesn't actually
    connect to any network, so no encryption scheme is specified.

    For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
    
    Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
    1.0.1   K Hoang      28/03/2020 Change to use new WiFiNINA_Generic library to support many more boards running WiFiNINA
 *****************************************************************************************************************************/

#define DEBUG_WIFI_WEBSERVER_PORT Serial

#define USE_WIFI_NINA         true

#if    ( defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAM3X8E__) || defined(__CPU_ARC__) )
#if defined(WIFI_USE_SAMD)
#undef WIFI_USE_SAMD
#endif
#define WIFI_USE_SAMD      true
#endif

#if ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) )
#if defined(WIFI_USE_STM32)
#undef WIFI_USE_STM32
#endif
#define WIFI_USE_STM32      true
#endif

#ifdef CORE_TEENSY
#if defined(__IMXRT1062__)
#define BOARD_TYPE      "TEENSY 4.0"
#elif ( defined(__MKL26Z64__) || defined(ARDUINO_ARCH_AVR) )
#define BOARD_TYPE      "TEENSY LC or 2.0"
#else
#define BOARD_TYPE      "TEENSY 3.X"
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
#elif ( defined(__SAM3X8E__) || (__SAM3X8E__) || (__CPU_ARC__) )
#define BOARD_TYPE      "SAMD Board"
#else
#define BOARD_TYPE      "SAMD Unknown"
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

void printMacAddress()
{
  // get your MAC address
  byte mac[6];
  WiFi.macAddress(mac);

  // print MAC address
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

void listNetworks()
{
  // scan for nearby networks
  int numSsid = WiFi.scanNetworks();

  if (numSsid == -1)
  {
    Serial.println(F("Couldn't get a wifi connection"));
    while (true);
  }

  // print the list of networks seen
  Serial.print(F("Number of available networks:"));
  Serial.println(numSsid);

  // print the network number and name for each network found
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(F(") "));
    Serial.print(WiFi.SSID(thisNet));
    Serial.print(F("\tSignal: "));
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(F(" dBm"));
    Serial.print(F("\tEncryption: "));
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType)
{
  // read the encryption type and print out the name
  switch (thisType)
  {
    case AUTH_MODE_WPA_PSK:
      Serial.print(F("WPA_PSK"));
      break;
    case AUTH_MODE_WPA2_PSK:
      Serial.print(F("WPA2_PSK"));
      break;
    case AUTH_MODE_WPA:
      Serial.print(F("WPA"));
      break;
    case AUTH_MODE_WPA2:
      Serial.print(F("WPA2"));
      break;           
    case AUTH_MODE_AUTO:
      Serial.print(F("Auto"));
      break;
    case AUTH_MODE_SHARED_KEY:
      Serial.print(F("Shared Key"));
      break;      
    case AUTH_MODE_OPEN_SYSTEM:
      Serial.print(F("Open"));
      break;
    case AUTH_MODE_INVALID:
      Serial.print(F("Invalid"));
      break;      
  }
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting ScanNetworks on " + String(BOARD_TYPE));

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println(F("WiFi shield not present"));
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    Serial.print(F("Connecting to WPA SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(WiFi.localIP());

  // Print WiFi MAC address
  printMacAddress();
}

void loop()
{
  // scan for existing networks
  Serial.println();
  Serial.println(F("Scanning available networks..."));
  listNetworks();
  delay(10000);
}
