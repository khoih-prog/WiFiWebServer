## WiFiWebServer

[![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiWebServer.svg?)](https://www.ardu-badge.com/WiFiWebServer)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/WiFiWebServer.svg)](https://github.com/khoih-prog/WiFiWebServer/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/WiFiWebServer/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/WiFiWebServer.svg)](http://github.com/khoih-prog/WiFiWebServer/issues)

---

#### New in v1.0.5

1. Fix bug not closing client and releasing socket exposed in NINA Firmware v1.4.0.
2. Enhance examples.

#### New in v1.0.4

1. Add support to boards using ***WiFi101 built-in or shield***. For example MKR1000, Teensy, Mega, etc..
2. Support any future custom WiFi library that meets the no-compiling-error requirements.

#### New in v1.0.3

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc.***

### New Version v1.0.2

1. Add support to ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE***.

### New Version v1.0.1

1. Use new [`WiFiNINA_Generic library`](https://github.com/khoih-prog/WiFiNINA_Generic) to provide support to many more boards running WiFiNINA. 
The original WiFiNINA library only supports ***Nano-33 IoT***, Arduino MKR WiFi 1010, Arduino MKR VIDOR 4000 and Arduino UNO WiFi Rev.2.

### Initial Version v1.0.0

This is simple yet complete WebServer library for `AVR Mega, Teensy, SAMD21, STM32, etc.` boards running WiFi modules/shields (WiFiNINA U-Blox W101, W102, etc.). ***The functions are similar and compatible to ESP8266/ESP32 WebServer libraries*** to make life much easier to port sketches from ESP8266/ESP32.

---

This [WiFiWebServer library](https://github.com/khoih-prog/WiFiWebServer) currently supports these following boards:

1. SAM DUE
2. SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit Adafruit M0: ItsyBitsy M0, etc.)
3. SAMD51 (Adafruit M4 : Metro, Grand Central, ItsyBitsy, Feather Express, Trellis, Metro AirLift lite, MONSTER M4SK Express, Hallowing Express, etc. )
4. Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC)
5. STM32F1, STM32F2, STM32F4, STM32F7 with more than 32KB flash memory.
6. AVR Mega1280, 2560, ADK.

and these following WiFi shields/modules:

1. WiFiNINA
2. WiFi101
3. U-Blox W101, W102
4. ESP8266/ESP32-AT command

The library provides supports to:

1. WiFi Client, STA and AP mode
2. TCP Server and Client
3. UDP Server and Client
4. HTTP Server and Client
5. HTTP GET and POST requests, provides argument parsing, handles one client at a time.

It is based on and modified from:

1. [Ivan Grokhotkov's ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)

The WiFiWebServer class found in `WiFiWebServer.h` header, is a simple web server that knows how to handle HTTP requests such as GET and POST and can only support one simultaneous client.

---

## Prerequisite
 1. [`Arduino IDE 1.8.12+` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. `Arduino AVR core 1.8.2+` for Arduino (Use Arduino Board Manager) for AVR boards
 3. [`Teensy core v1.53+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0) boards.
 4. [`Arduino SAM DUE core v1.6.12+`](https://www.arduino.cc/en/Guide/ArduinoDue) for SAM DUE ARM Cortex-M3 boards.
 5. [`Arduino SAMD core 1.8.6+`](https://www.arduino.cc/en/Guide/ArduinoM0) for SAMD ARM Cortex-M0+ boards  (Nano 33 IoT, etc.).
 6. [`Adafruit SAMD core 1.5.14+`](https://www.adafruit.com/) for SAMD ARM Cortex-M0+ and M4 boards (Itsy-Bitsy M4, etc.)
 7. [`Adafruit nRF52 v0.20.5+`](https://www.adafruit.com/) for nRF52 boards such as AdaFruit Feather nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.
 8. [`Arduino Core for STM32 v1.9.0+`](https://github.com/khoih-prog/Arduino_Core_STM32) for STM32 boards 
 9. [`Functional-VLPP library`](https://github.com/khoih-prog/functional-vlpp) to use server's lambda function. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/Functional-Vlpp.svg?)](https://www.ardu-badge.com/Functional-Vlpp)
10. [`WiFiNINA_Generic library v1.6.0+`](https://github.com/khoih-prog/WiFiNINA_Generic) if using WiFiNINA. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiNINA_Generic.svg?)](https://www.ardu-badge.com/WiFiNINA_Generic).
11. [`ESP_AT_Lib library v1.0.0+`](https://github.com/khoih-prog/ESP_AT_Lib) if using ESP8288/ESP32-AT shields. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP_AT_Lib.svg?)](https://www.ardu-badge.com/ESP_AT_Lib).
12. [`WiFi101 library v0.16.0+`](https://www.arduino.cc/en/Reference/WiFi101) if using WiFi101 shields.
13. [`WiFiEspAT library v1.2.0+`](https://github.com/jandrassy/WiFiEspAT) if using ESP8288/ESP32-AT shields.

## Installation

### Use Arduino Library Manager
The best and easiest way is to use `Arduino Library Manager`. Search for `WiFiWebServer`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiWebServer.svg?)](https://www.ardu-badge.com/WiFiWebServer) for more detailed instructions.

### Manual Install

1. Navigate to [WiFiWebServer](https://github.com/khoih-prog/WiFiWebServer) page.
2. Download the latest release `WiFiWebServer-master.zip`.
3. Extract the zip file to `WiFiWebServer-master` directory 
4. Copy the whole `WiFiWebServer-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:
1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install **WiFiWebServer** library by using [Library Manager](https://docs.platformio.org/en/latest/librarymanager/). Search for WiFiWebServer in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](examples/platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically.

---

### Important notes

1. ***To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840 boards***, you have to copy the whole [nRF52 0.20.5](https://github.com/khoih-prog/WiFiWebServer/tree/master/Packages_Patches/adafruit/hardware/nrf52/0.20.5) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5). 

Supposing the Adafruit nRF52 version is 0.20.5. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B112_ublox/variant.cpp`

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`

2. ***To be able to compile and run on Teensy boards***, you have to copy the file [Teensy boards.txt](https://github.com/khoih-prog/WiFiWebServer/blob/master/Packages_Patches/hardware/teensy/avr/boards.txt) into Teensy hardware directory (./arduino-1.8.12/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.12. This file must be copied into the directory:

- `./arduino-1.8.12/hardware/teensy/avr/boards.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`

3. ***To be able to compile and run on SAM DUE boards***, you have to copy the whole [SAM DUE](https://github.com/khoih-prog/WiFiWebServer/tree/master/Packages_Patches/arduino/hardware/sam/1.6.12) directory into Arduino sam directory (~/.arduino15/packages/arduino/hardware/sam/1.6.12). 

Supposing the Arduino SAM core version is 1.6.12. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/1.6.12/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/x.yy.zz/platform.txt`

4. ***To be able to automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the file [Arduino SAMD platform.txt](https://github.com/khoih-prog/WiFiWebServer/tree/master/Packages_Patches/arduino/hardware/samd/1.8.6) into Arduino samd directory (~/.arduino15/packages/arduino/hardware/samd/1.8.6). 

Supposing the Arduino SAMD core version is 1.8.6. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.6/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.zz/platform.txt`

5. ***To be able to automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the file [Adafruit SAMD platform.txt](https://github.com/khoih-prog/WiFiWebServer/tree/master/Packages_Patches/adafruit/hardware/samd/1.5.14) into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.5.14). 

Supposing the Adafruit SAMD core version is 1.5.14. This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.5.14/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`

---

### How to configure to use different WiFi Libraries

1. Please change the pin-to-pin connection in `~/Arduino/libraries/src/WiFiNINA_Pinout_Generic.h` to match actual connection if using WiFiNINA with [`WiFiNINA_Generic library`](https://github.com/khoih-prog/WiFiNINA_Generic).

For example

```cpp
#elif ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
        defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
        defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) )

  #warning You have to modify pin usage accoring to actual connection for NRF528XX
  // To define pin out for WiFiNINA here

  //#define PINS_COUNT           (60u)
  //NINA
  #define NINA_GPIO0  (26u)                             //26
  #define NINA_RESETN (27u)
  #define NINA_ACK    (28u)

  #define SPIWIFI_SS       24   //PIN_SPI1_SS            //24
  #define SPIWIFI_ACK      28   //NINA_ACK               //28 
  #define SPIWIFI_RESET    27   //NINA_RESETN            //27
```

2. ***How to select which built-in WiFi or shield to use***

- To use W102-based WiFiNINA, define in the sketch:

```cpp
#define USE_WIFI_NINA         true
```

- To use built-in WiFi101 or shield:

```cpp
#define USE_WIFI_NINA         false
#define USE_WIFI101           true
```

- To use MKR1000 with built-in WiFi101:

```cpp
// Don't care false ot true
#define USE_WIFI_NINA         false
```

- For boards other than MKR1000, to use another WiFi library with the standard ***WiFi.h***, such as WiFiEspAT library

```cpp
#define USE_WIFI_NINA         false
```
- To use another WiFi library without the standard ***WiFi.h***
For example, WiFi_XYZ library uses ***WiFi_XYZ.h***

```cpp
#define USE_WIFI_NINA         false
#define USE_WIFI_CUSTOM       true

...
//Must be placed before #include <WiFiWebServer.h>
#include <WiFi_XYZ.h>
#include <WiFiWebServer.h>
```

#### Important:

- The ***WiFiEsp, WiFi_Link libraries are not supported***. Don't use unless you know how to modify those libraries.
- Requests to support for any custom WiFi library will be ignored. ***Use at your own risk***.

---

#### Usage

#### Class Constructor

```cpp
  WiFiWebServer server(80);
```

Creates the WiFiWebServer class object.

*Parameters:* 
 
host port number: ``int port`` (default is the standard HTTP port 80)

#### Basic Operations

***Starting the server***

```cpp
  void begin();
```

***Handling incoming client requests***

```cpp
  void handleClient();
```

***Disabling the server***

```cpp
  void close();
  void stop();
```

Both methods function the same

***Client request handlers***

```cpp
  void on();
  void addHandler();
  void onNotFound();
  void onFileUpload();	
```

Example:*

```cpp
  server.on("/", handlerFunction);
  server.onNotFound(handlerFunction);   // called when handler is not assigned
  server.onFileUpload(handlerFunction); // handle file uploads
```

***Sending responses to the client***

```cpp
  void send();
  void send_P();
```

`Parameters:`

`code` - HTTP response code, can be `200` or `404`, etc.

`content_type` - HTTP content type, like `"text/plain"` or `"image/png"`, etc.

`content` - actual content body

#### Advanced Options

***Getting information about request arguments***

```cpp
  const String & arg();
  const String & argName();
  int args();
  bool hasArg();
```

`Function usage:`

`arg` - get request argument value, use `arg("plain")` to get POST body
	
`argName` - get request argument name
	
`args` - get arguments count
	
`hasArg` - check if argument exist

***Getting information about request headers***

```cpp
  const String & header();
  const String & headerName();
  const String & hostHeader();
  int headers();
  bool hasHeader();
``` 
`Function usage:`

`header` - get request header value

`headerName` - get request header name

`hostHeader` - get request host header if available, else empty string
  
`headers` - get header count
	
`hasHeader` - check if header exist

***Authentication***

```cpp
  bool authenticate();
  void requestAuthentication();
```

`Function usage:`

`authenticate` - server authentication, returns true if client is authenticated else false

`requestAuthentication` - sends authentication failure response to the client

`Example Usage:`

```cpp

  if(!server.authenticate(username, password)){
    server.requestAuthentication();
  }
```

#### Other Function Calls

```cpp
  const String & uri(); // get the current uri
  HTTPMethod  method(); // get the current method 
  WiFiClient client(); // get the current client
  HTTPUpload & upload(); // get the current upload
  void setContentLength(); // set content length
  void sendHeader(); // send HTTP header
  void sendContent(); // send content
  void sendContent_P(); 
  void collectHeaders(); // set the request headers to collect
  void serveStatic();
  size_t streamFile();
```

---

Also see examples: 
 1. [HelloServer](examples/HelloServer)
 2. [HelloServer2](examples/HelloServer2)
 3. [AdvancedWebServer](examples/AdvancedWebServer)
 4. [HttpBasicAuth](examples/HttpBasicAuth)
 5. [PostServer](examples/PostServer)
 6. [SimpleAuthentication](examples/SimpleAuthentication)
 7. [ConnectWPA](examples/ConnectWPA)
 8. [ScanNetworks](examples/ScanNetworks)
 9. [UdpNTPClient](examples/UdpNTPClient)
10. [WiFiUdpNtpClient](examples/WiFiUdpNtpClient) 
11. [UdpSendReceive](examples/UdpSendReceive)
12. [WebClient](examples/WebClient)
13. [WebClientRepeating](examples/WebClientRepeating)
14. [WebServer](examples/WebServer)
15. [AP_SimpleWebServer](examples/AP_SimpleWebServer)


## Example [AdvancedWebServer](examples/AdvancedWebServer)

Please take a look at other examples, as well.

```cpp
#define DEBUG_WIFI_WEBSERVER_PORT   Serial

// Debug Level from 0 to 4
#define _WIFI_LOGLEVEL_             1
#define _WIFININA_LOGLEVEL_         1

#define USE_WIFI_NINA         true

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

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
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
#define BUFFER_SIZE     400
  
  digitalWrite(led, 1);
  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, BUFFER_SIZE - 1,
           "<html>\
<head>\
<meta http-equiv='refresh' content='5'/>\
<title>WiFiNINA %s</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>Hello from WiFiNINA</h1>\
<h2>on %s</h2>\
<p>Uptime: %02d:%02d:%02d</p>\
<img src=\"/test.svg\" />\
</body>\
</html>", BOARD_NAME, BOARD_NAME, hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void handleNotFound()
{
  digitalWrite(led, 1);
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

String out;

void drawGraph()
{
  //String out;
  out.reserve(3000);
  char temp[70];
  out = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"310\" height=\"150\">\n";
  out += "<rect width=\"310\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;

  for (int x = 10; x < 300; x += 10)
  {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}

void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting AdvancedServer on " + String(BOARD_NAME));

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

  server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []()
  {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());
}

void loop(void)
{
  server.handleClient();
}
```

The following are debug terminal output and screen shot when running example [AdvancedWebServer](examples/AdvancedWebServer) on Arduino Nano 33 IoT board

<p align="center">
    <img src="https://github.com/khoih-prog/WiFiWebServer/blob/master/pics/AdvancedWebServer.png">
</p>

```

Starting AdvancedServer on SAMD_NANO_33_IOT
[NN] ===============================
[NN] 
Used/default SPI pinout: 
[NN] MOSI:  11
[NN] MISO:  12
[NN] SCK:  13
[NN] SS:  10
[NN] ===============================
[NN] 
Used/default NINA pinout: 
[NN] NINA_GPIO0:  26
[NN] NINA_RESETN/SPIWIFI_RESET:  27
[NN] NINA_ACK:  28
[NN] SS:  10
[NN] ===============================
[NN] 
Actual final pinout to used: 
[NN] SPIWIFI_SS:  24
[NN] SLAVESELECT/SPIWIFI_SS:  24
[NN] SLAVEREADY/SPIWIFI_ACK/NINA_ACK:  28
[NN] SLAVERESET/SPIWIFI_RESET/NINA_RESETN:  27
[NN] ===============================

Connecting to WPA SSID: HueNet1
HTTP server started @ 192.168.2.118

Starting AdvancedServer on SAMD_NANO_33_IOT
[NN] ===============================
[NN] 
Used/default SPI pinout: 
[NN] MOSI:  11
[NN] MISO:  12
[NN] SCK:  13
[NN] SS:  10
[NN] ===============================
[NN] 
Used/default NINA pinout: 
[NN] NINA_GPIO0:  26
[NN] NINA_RESETN/SPIWIFI_RESET:  27
[NN] NINA_ACK:  28
[NN] SS:  10
[NN] ===============================
[NN] 
Actual final pinout to used: 
[NN] SPIWIFI_SS:  24
[NN] SLAVESELECT/SPIWIFI_SS:  24
[NN] SLAVEREADY/SPIWIFI_ACK/NINA_ACK:  28
[NN] SLAVERESET/SPIWIFI_RESET/NINA_RESETN:  27
[NN] ===============================

Connecting to WPA SSID: HueNet1
HTTP server started @ 192.168.2.118
WiFiWebServer::handleClient: New Client
method:  GET
url:  /
search:
headerName: Host
headerValue: 192.168.2.118
headerName: Connection
headerValue: keep-alive
headerName: Cache-Control
headerValue: max-age=0
headerName: DNT
headerValue: 1
headerName: Upgrade-Insecure-Requests
headerValue: 1
headerName: User-Agent
headerValue: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36
headerName: Accept
headerValue: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
headerName: Accept-Encoding
headerValue: gzip, deflate
headerName: Accept-Language
headerValue: en-GB,en-US;q=0.9,en;q=0.8
headerName: Cookie
headerValue: NINASESSIONID=0
args:
args count:  0
args:
args count:  0
Request: /
Arguments: 
Final list of key/value pairs:
WiFiWebServer::_handleRequest handle
WiFiWebServer::send1: len =  330
content =  <html><head><meta http-equiv='refresh' content='5'/><title>WiFiNINA SAMD_NANO_33_IOT</title><style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1>Hello from WiFiNINA</h1><h2>on SAMD_NANO_33_IOT</h2><p>Uptime: 00:00:20</p><img src="/test.svg" /></body></html>
WiFiWebServer::_prepareHeader sendHeader Conn close
WiFiWebServer::send1: write header =  HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 330
Connection: close


WiFiWebServer::sendContent: Client.write content:  <html><head><meta http-equiv='refresh' content='5'/><title>WiFiNINA SAMD_NANO_33_IOT</title><style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1>Hello from WiFiNINA</h1><h2>on SAMD_NANO_33_IOT</h2><p>Uptime: 00:00:20</p><img src="/test.svg" /></body></html>
WiFiWebServer::_handleRequest OK
WiFiWebServer::handleClient: Client disconnected
WiFiWebServer::handleClient: Don't keepCurrentClient
WiFiWebServer::handleClient: Client disconnected
WiFiWebServer::handleClient: New Client
method:  GET
url:  /test.svg
search:
headerName: Host
headerValue: 192.168.2.118
headerName: Connection
headerValue: keep-alive
headerName: User-Agent
headerValue: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36
headerName: DNT
headerValue: 1
headerName: Accept
headerValue: image/webp,image/apng,image/*,*/*;q=0.8
headerName: Referer
headerValue: http://192.168.2.118/
headerName: Accept-Encoding
headerValue: gzip, deflate
headerName: Accept-Language
headerValue: en-GB,en-US;q=0.9,en;q=0.8
headerName: Cookie
headerValue: NINASESSIONID=0
args:
args count:  0
args:
args count:  0
Request: /test.svg
Arguments: 
Final list of key/value pairs:
WiFiWebServer::_handleRequest handle
WiFiWebServer::send1: len =  1946
content =  <svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="310" height="150">
<rect width="310" height="150" fill="rgb(250, 230, 210)" stroke-width="1" stroke="rgb(0, 0, 0)" />
<g stroke="black">
<line x1="10" y1="77" x2="20" y2="67" stroke-width="1" />
<line x1="20" y1="67" x2="30" y2="98" stroke-width="1" />
<line x1="30" y1="98" x2="40" y2="111" stroke-width="1" />
<line x1="40" y1="111" x2="50" y2="90" stroke-width="1" />
<line x1="50" y1="90" x2="60" y2="22" stroke-width="1" />
<line x1="60" y1="22" x2="70" y2="98" stroke-width="1" />
<line x1="70" y1="98" x2="80" y2="64" stroke-width="1" />
<line x1="80" y1="64" x2="90" y2="104" stroke-width="1" />
<line x1="90" y1="104" x2="100" y2="31" stroke-width="1" />
<line x1="100" y1="31" x2="110" y2="59" stroke-width="1" />
<line x1="110" y1="59" x2="120" y2="139" stroke-width="1" />
<line x1="120" y1="139" x2="130" y2="117" stroke-width="1" />
<line x1="130" y1="117" x2="140" y2="75" stroke-width="1" />
<line x1="140" y1="75" x2="150" y2="72" stroke-width="1" />
<line x1="150" y1="72" x2="160" y2="137" stroke-width="1" />
<line x1="160" y1="137" x2="170" y2="20" stroke-width="1" />
<line x1="170" y1="20" x2="180" y2="94" stroke-width="1" />
<line x1="180" y1="94" x2="190" y2="81" stroke-width="1" />
<line x1="190" y1="81" x2="200" y2="38" stroke-width="1" />
<line x1="200" y1="38" x2="210" y2="33" stroke-width="1" />
<line x1="210" y1="33" x2="220" y2="53" stroke-width="1" />
<line x1="220" y1="53" x2="230" y2="88" stroke-width="1" />
<line x1="230" y1="88" x2="240" y2="32" stroke-width="1" />
<line x1="240" y1="32" x2="250" y2="110" stroke-width="1" />
<line x1="250" y1="110" x2="260" y2="87" stroke-width="1" />
<line x1="260" y1="87" x2="270" y2="11" stroke-width="1" />
<line x1="270" y1="11" x2="280" y2="98" stroke-width="1" />
<line x1="280" y1="98" x2="290" y2="76" stroke-width="1" />
<line x1="290" y1="76" x2="300" y2="121" stroke-width="1" />
</g>
</svg>

WiFiWebServer::_prepareHeader sendHeader Conn close
WiFiWebServer::send1: write header =  HTTP/1.1 200 OK
Content-Type: image/svg+xml
Content-Length: 1946
Connection: close
```
---

### Debug

Debug is enabled by default on Serial. Debug Level from 0 to 4. To disable, change the _WIFI_LOGLEVEL_ and _WIFININA_LOGLEVEL_ to 0

```cpp
// Use this to output debug msgs to Serial
#define DEBUG_WIFI_WEBSERVER_PORT   Serial

// Debug Level from 0 to 4
#define _WIFI_LOGLEVEL_             1
#define _WIFININA_LOGLEVEL_         1
```

---

#### New in v1.0.5

1. Fix bug not closing client and releasing socket exposed in NINA Firmware v1.4.0.
2. Enhance examples.

#### New in v1.0.4

1. Add support to boards using ***WiFi101 built-in or shield***. For example MKR1000, Teensy, Mega, etc..
2. Support any future custom WiFi library that meets the no-compiling-error requirements.

#### New in v1.0.3

1. Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, etc.

### New Version v1.0.2

1. Add support to ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE***.

### New Version v1.0.1

1. Use new [`WiFiNINA_Generic library`](https://github.com/khoih-prog/WiFiNINA_Generic) to provide support to many more boards running WiFiNINA. 
The original WiFiNINA library only supports ***Nano-33 IoT***, Arduino MKR WiFi 1010, Arduino MKR VIDOR 4000 and Arduino UNO WiFi Rev.2.

### Initial Version v1.0.0

This is simple yet complete WebServer library for `AVR, Teensy, SAMD21, STM32, etc.` boards running WiFi modules/shields (WiFiNINA U-Blox W101, W102, etc.). ***The functions are similar and compatible to ESP8266/ESP32 WebServer libraries*** to make life much easier to port sketches from ESP8266/ESP32.

The library currently supports these boards
1. SAM DUE
2. SAMD21 (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)
3. SAMD51 (Adafruit M4 : Metro, Grand Central, ItsyBitsy, Feather Express, Trellis, Metro AirLift lite, MONSTER M4SK Express, Hallowing Express, etc. )
4. Teensy (4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC)
5. STM32F1, STM32F2, STM32F4, STM32F7 with more than 32KB flash memory.
6. AVR Mega1280, 2560, ADK.

The library supports 
1. WiFi Client, STA and AP mode
2. TCP Server and Client
3. UDP Server and Client
4. HTTP Server and Client
5. HTTP GET and POST requests, provides argument parsing, handles one client at a time.

---

## TO DO
1. Bug Searching and Killing
2. Add SSL/TLS Client and Server support
3. Support more types of boards using WiFiNINA and other WiFi shields.

### DONE

 1. Add support to Arduino SAMD21, Adafruit SAMD21/SAMD51.
 2. Add support to nRF52.
 3. Add support to SAM DUE.
 4. Add support to STM32F.
 5. Add support to WiFiNINA using [`WiFiNINA_Generic library`](https://github.com/khoih-prog/WiFiNINA_Generic). 
 6. Add support to [`ESP_AT_Lib library`](https://github.com/khoih-prog/ESP_AT_Lib).
 7. Add support to [`WiFi101 library`](https://www.arduino.cc/en/Reference/WiFi101).
 8. Add support to [`WiFiEspAT library`](https://github.com/jandrassy/WiFiEspAT).
 
---

### Contributions and thanks

1. Based on and modified from [Ivan Grokhotkov's ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
2. Thanks to good work of [Miguel Alexandre Wisintainer](https://github.com/tcpipchip) for initiating, inspriring, working with, developing, debugging and testing. Without that, support to nRF52, especially ***U-Box B302 running as nRF52840 and U-Box B112 running as nRF52832***, has never been started and finished. See [u-blox nina b](https://github.com/khoih-prog/WiFiNINA_Generic/issues/1)

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

## Copyright

Copyright 2020- Khoi Hoang


