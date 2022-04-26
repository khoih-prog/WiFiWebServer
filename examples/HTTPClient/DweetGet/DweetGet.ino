/****************************************************************************************************************************
  DweetGet.ino - Simple Arduino web server sample for WiFi shield

  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Based on  and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Connects to dweet.io once every ten seconds, sends a GET request and a request body. Uses SSL

  Shows how to use Strings to assemble path and parse content from response. dweet.io expects:
  https://dweet.io/get/latest/dweet/for/thingName

  For more on dweet.io, see https://dweet.io/play/

  created 15 Feb 2016
  updated 22 Jan 2019
  by Tom Igoe
 *****************************************************************************************************************************/

#include "defines.h"

const char serverAddress[] = "dweet.io";  // server address
int port = 80;

WiFiClient      client;
WiFiHttpClient  httpClient(client, serverAddress, port);

int status = WL_IDLE_STATUS;     // the Wifi radio's status

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
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print(F("\nStarting DweetGet on ")); Serial.print(BOARD_NAME);
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

  // you're connected now, so print out the data
  printWifiStatus();
}

// assemble the path for the GET message:
String dweetName  = "Hello-from-" + String(BOARD_NAME);
String path       = "/get/latest/dweet/for/" + dweetName;

void loop()
{
  // send the GET request
  Serial.println("Making GET request");
  httpClient.get(path);

  // read the status code and body of the response
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  /*
    Typical response is:
    {"this":"succeeded",
    "by":"getting",
    "the":"dweets",
    "with":[{"thing":"my-thing-name",
      "created":"2016-02-16T05:10:36.589Z",
      "content":{"sensorValue":456}}]}

    You want "content": numberValue
  */
  // now parse the response looking for "content":
  int labelStart = response.indexOf("content\":");
  
  // find the first { after "content":
  int contentStart = response.indexOf("{", labelStart);
  
  // find the following } and get what's between the braces:
  int contentEnd = response.indexOf("}", labelStart);
  
  String content = response.substring(contentStart + 1, contentEnd);
  Serial.println(content);

  // now get the value after the colon, and convert to an int:
  int valueStart      = content.indexOf(":");
  String valueString  = content.substring(valueStart + 1);
  int number          = valueString.toInt();
  
  Serial.print("Value string: ");
  Serial.println(valueString);
  Serial.print("Actual value: ");
  Serial.println(number);

  Serial.println("Wait ten seconds\n");
  delay(10000);
}
