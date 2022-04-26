/****************************************************************************************************************************
  ScanNetworks.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Based on  and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
  
  This example  prints the Wifi shield's MAC address, and
  scans for available Wifi networks using the Wifi shield.
  Every ten seconds, it scans again. It doesn't actually
  connect to any network, so no encryption scheme is specified.
  
  For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
 ***************************************************************************************************************************************/

#include "defines.h"

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

#if !USE_WIFI_PORTENTA_H7
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
#endif

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
  for (int thisNet = 0; thisNet < numSsid; thisNet++) 
  {
    Serial.print(thisNet);
    Serial.print(F(") "));
    Serial.print(WiFi.SSID(thisNet));
    Serial.print(F("\tSignal: "));
    Serial.print(WiFi.RSSI(thisNet));

#if USE_WIFI_NINA
    Serial.print(F(" dBm"));
    Serial.print(F("\tEncryption: "));
    printEncryptionType(WiFi.encryptionType(thisNet));
#else
    Serial.println(F(" dBm"));
#endif    
  }
}

#if USE_WIFI_NINA
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
#endif

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print(F("\nStarting ScanNetworks on ")); Serial.print(BOARD_NAME);
  Serial.print(F(" with ")); Serial.println(SHIELD_TYPE); 
  Serial.println(WIFI_WEBSERVER_VERSION);

#if WIFI_USING_ESP_AT

  // initialize serial for ESP module
  EspSerial.begin(115200);
  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println(F("WiFi shield init done"));
  
#endif

#if !(ESP32 || ESP8266)
  
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
      Serial.println(F("Please upgrade the firmware"));
    }
  #endif
  
#endif

  Serial.print(F("Connecting to SSID: "));
  Serial.println(ssid);
  
  status = WiFi.begin(ssid, pass);

  delay(1000);
   
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    delay(500);
        
    // Connect to WPA/WPA2 network
    status = WiFi.status();
  }

  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(WiFi.localIP());

#if !USE_WIFI_PORTENTA_H7
  // Print WiFi MAC address
  printMacAddress();
#endif  
}

void loop()
{
  // scan for existing networks
  Serial.println();
  Serial.println(F("Scanning available networks..."));
  listNetworks();
  delay(10000);
}
