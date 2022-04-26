/****************************************************************************************************************************
  SimpleAuthentication.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Based on  and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
 ***************************************************************************************************************************************/

#include "defines.h"    

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiWebServer server(80);

//Check if header is present and correct
bool is_authenticated()
{
  Serial.println(F("Enter is_authenticated"));
  
  if (server.hasHeader(F("Cookie")))
  {
    Serial.print(F("Found cookie: "));
    String cookie = server.header(F("Cookie"));
    Serial.println(cookie);
    if (cookie.indexOf(F("NINASESSIONID=1")) != -1)
    {
      Serial.println(F("Authentication Successful"));
      return true;
    }
  }
  
  Serial.println(F("Authentication Failed"));
  
  return false;
}

//login page, also called for disconnect
void handleLogin()
{
  String msg;

  if (server.hasHeader(F("Cookie")))
  {
    Serial.print(F("Found cookie: "));
    
    String cookie = server.header(F("Cookie"));
    Serial.println(cookie);
  }

  if (server.hasArg("DISCONNECT"))
  {
    Serial.println(F("Disconnection"));
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.sendHeader(F("Set-Cookie"), F("NINASESSIONID=0"));
    server.send(301);
    return;
  }

  if (server.hasArg(F("USERNAME")) && server.hasArg(F("PASSWORD")))
  {
    if (server.arg(F("USERNAME")) == "admin" &&  server.arg(F("PASSWORD")) == "admin")
    {
      server.sendHeader(F("Location"), F("/"));
      server.sendHeader(F("Cache-Control"), F("no-cache"));
      server.sendHeader(F("Set-Cookie"), F("NINASESSIONID=1"));
      server.send(301);
      Serial.println(F("Log in Successful"));
      return;
    }
    msg = F("Wrong username/password! try again.");
    Serial.println(F("Log in Failed"));
  }

  String content = F("<html><body><form action='/login' method='POST'>To log in, please use : admin/admin<br>");
  content += F("User:<input type='text' name='USERNAME' placeholder='user name'><br>");
  content += F("Password:<input type='password' name='PASSWORD' placeholder='password'><br>");
  content += F("<input type='submit' name='SUBMIT' value='Submit'></form>");
  content += msg;
  content += F("<br>");
  content += F("You also can go <a href='/inline'>here</a></body></html>");
  
  server.send(200, F("text/html"), content);
}

#define ORIGINAL_STR_LEN        384

//root page can be accessed only if authentication is ok
void handleRoot()
{
  static String content;
  static uint16_t previousStrLen = ORIGINAL_STR_LEN;
  
  Serial.println(F("Enter handleRoot"));

  if (!is_authenticated())
  {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    
    return;
  }

  content = F("<html><body><H2>Hello, you successfully connected to WiFiNINA on "); 
  content += BOARD_NAME;
  content += F("!</H2><br>");

  if (server.hasHeader(F("User-Agent")))
  {
    content += F("the user agent used is : ");
    content += server.header(F("User-Agent"));
    content += F("<br><br>");
  }

  content += F("You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>");

  if (content.length() > previousStrLen)
  {
    WS_LOGERROR3(F("String Len > "), previousStrLen, F(", extend to"), content.length() + 48);

    previousStrLen = content.length() + 48;
    
    content.reserve(previousStrLen);
  }
  else
  {
    WS_LOGDEBUG1(F("Len ="), content.length());
    server.send(200, F("text/html"), content);
  }
}

//no need authentication
void handleNotFound()
{ 
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
}

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print(F("\nStarting SimpleAuthentication on ")); Serial.print(BOARD_NAME);
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

  server.on(F("/"), handleRoot);
  server.on(F("/login"), handleLogin);

  server.on(F("/inline"), []()
  {
    server.send(200, F("text/plain"), F("This works without need of authentication"));
  });

  server.onNotFound(handleNotFound);

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();

  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();
}
