/****************************************************************************************************************************
  WebClientRepeating_WiFiMulti.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Based on  and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  A simple web server that shows the value of the analog input pins via a web page using an ESP8266 module.
  This sketch will start an access point and print the IP address of your ESP8266 module to the Serial monitor.
  From there, you can open that address in a web browser to display the web page.
  The web page will be automatically refreshed each 20 seconds.

  For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
 ***************************************************************************************************************************************/
#include "defines.h"

int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "arduino.tips";

//unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
//const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the Web client object
WiFiClient client;

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

void check_status()
{
  static uint32_t checkstatus_timeout  = 0;
  static uint32_t checkwifi_timeout    = 0;
  static uint32_t checkHTTP_timeout    = 0;

  static uint32_t current_millis;

#if ( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
#define WIFICHECK_INTERVAL    10000L
#else
#define WIFICHECK_INTERVAL    1000L
#endif

#define HEARTBEAT_INTERVAL    10000L
#define HTTP_INTERVAL         20000L

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

  // Check HTTP every HTTP_INTERVAL (20) seconds.
  if ((current_millis > checkHTTP_timeout) || (checkHTTP_timeout == 0))
  {
    httpRequest();
    checkHTTP_timeout = current_millis + HTTP_INTERVAL;
  }

  // Important delay() for RTL8720DN
  delay(200);
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

// this method makes a HTTP connection to the server
void httpRequest()
{
  Serial.println();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80))
  {
    Serial.println(F("Connecting..."));

    // send the HTTP PUT request
    client.println(F("GET /asciilogo.txt HTTP/1.1"));
    client.println(F("Host: arduino.tips"));
    client.println(F("Connection: close"));
    client.println();
  }
  else
  {
    // if you couldn't make a connection
    Serial.println(F("Connection failed"));
  }
}

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  Serial.print(F("\nStarting WebClientRepeating_WiFiMulti on "));
  Serial.print(BOARD_NAME);
  Serial.print(F(" with "));
  Serial.println(SHIELD_TYPE);
  Serial.println(WIFIMULTI_GENERIC_VERSION);
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
}

void printoutData()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }
}

void loop()
{
  printoutData();
  check_status();
}
