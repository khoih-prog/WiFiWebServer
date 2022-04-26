/****************************************************************************************************************************
  AP_SimpleWebServer.ino - Simple Arduino WiFi Web Server LED Blink sample for SAMD21 running WiFiNINA shield
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Based on  and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
  
  A simple web server that lets you blink an LED via the web.
  This sketch will create a new access point (with no password).
  It will then launch a new server and print out the IP address
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 13.
  
  If the IP address of your board is yourAddress:
  http://yourAddress/H turns the LED on
  http://yourAddress/L turns it off
  
  created 25 Nov 2012
  by Tom Igoe
  adapted to WiFi AP by Adafruit
 ***************************************************************************************************************************************/

#include "defines.h"

int keyIndex = 0;                // your network key Index number (needed only for WEP)

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

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void printWiFiStatus()
{
  // print your WiFi shield's IP address:

#if (ESP32 || ESP8266)
  IPAddress ip = WiFi.softAPIP();
#else  

  // print the SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  
  Serial.println(WiFi.SSID());
  
  IPAddress ip = WiFi.localIP();
#endif
  
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print(F("To see this page in action, open a browser to http://"));
  Serial.println(ip);
}

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  delay(200);

  Serial.print(F("\nStarting AP_SimpleWebServer on ")); Serial.print(BOARD_NAME);
  Serial.print(F(" with ")); Serial.println(SHIELD_TYPE); 
  Serial.println(WIFI_WEBSERVER_VERSION);

  pinMode(led, OUTPUT);      // set the LED pin mode

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

  // by default the local IP address of will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print(F("Creating access point named: "));
  Serial.print(ssid);
  Serial.print(F(" and password: "));
  Serial.println(pass);

#if (ESP32 || ESP8266)

  WiFi.softAP(ssid, pass);
  
#else

  // Create open network. Change this line if you want to create an WEP network:
  // default AP channel = 1
  uint8_t ap_channel = 2;

  status = WiFi.beginAP(ssid, pass, ap_channel);
  
  //status = WiFi.beginAP(ssid, pass);

  if (status != WL_AP_LISTENING)
  {
    Serial.println(F("Creating access point failed"));
    // don't continue
    while (true);
  }
#endif

  // wait 10 seconds for connection:
  //delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}

void loop()
{
#if !(ESP32 || ESP8266)
  // compare the previous status to the current status
  if (status != WiFi.status())
  {
    // it has changed update the variable
    status = WiFi.status();


    if (status == WL_AP_CONNECTED)
    {
      // a device has connected to the AP
      Serial.println(F("Device connected to AP"));
    } 
    else
    {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println(F("Device disconnected from AP"));
    }  
  }
#endif

  WiFiClient client = server.available();   // listen for incoming clients

  if (client)
  {
    // if you get a client,
    Serial.println(F("New client"));           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    
    while (client.connected())
    {
      // loop while the client's connected
      if (client.available())
      {
        // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor

        if (c == '\n')
        {
          // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-type:text/html"));
            client.println();

            // the content of the HTTP response follows the header:
            client.print(F("Click <a href=\"/H\">here</a> turn the LED on<br>"));
            client.print(F("Click <a href=\"/L\">here</a> turn the LED off<br>"));

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else
          {
            // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith(F("GET /H")))
        {
          digitalWrite(led, HIGH);               // GET /H turns the LED on
        }
        
        if (currentLine.endsWith(F("GET /L")))
        {
          digitalWrite(led, LOW);                // GET /L turns the LED off
        }
      }
    }
    
    // close the connection:
    client.stop();
    Serial.println(F("Client disconnected"));
  }
}
