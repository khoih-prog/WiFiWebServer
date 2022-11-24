/****************************************************************************************************************************
  defines.h
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Based on  and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
 ***************************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#define DEBUG_WIFI_WEBSERVER_PORT   Serial

// Debug Level from 0 to 4
#define _WIFI_LOGLEVEL_             1
#define _WIFININA_LOGLEVEL_         1
#define _WIFIMULTI_LOGLEVEL_        1

#if ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )

#if defined(BOARD_NAME)
  #undef BOARD_NAME
#endif

#if defined(CORE_CM7)
  #warning Using Portenta H7 M7 core
  #define BOARD_NAME            "PORTENTA_H7_M7"
#else
  #warning Using Portenta H7 M4 core
  #define BOARD_NAME            "PORTENTA_H7_M4"
#endif

#define USE_WIFI_PORTENTA_H7  true

#define USE_WIFI_NINA         false

// To use the default WiFi library here
#define USE_WIFI_CUSTOM       false

#elif (ESP32)

#define USE_WIFI_NINA         false

// To use the default WiFi library here
#define USE_WIFI_CUSTOM       false

#elif (ESP8266)

#define USE_WIFI_NINA         false

// To use the default WiFi library here
#define USE_WIFI_CUSTOM       true

#elif ( defined(ARDUINO_SAMD_MKR1000)  || defined(ARDUINO_SAMD_MKRWIFI1010) )

#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#define USE_WIFI_CUSTOM       false

#elif ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_SAMD_NANO_33_IOT) )

#define USE_WIFI_NINA         true
#define USE_WIFI101           false
#define USE_WIFI_CUSTOM       false

#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)

#define USE_WIFI_NINA         false
#define USE_WIFI101           false
#define USE_WIFI_CUSTOM       false

#elif ( defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) )

#if defined(ARDUINO_AVR_UNO_WIFI_REV2)

#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#define USE_WIFI_CUSTOM       false

#elif ( defined(__AVR_ATmega4809__) || defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) || \
      defined(ARDUINO_AVR_ATmega4809) || defined(ARDUINO_AVR_ATmega4808) || defined(ARDUINO_AVR_ATmega3209) || \
      defined(ARDUINO_AVR_ATmega3208) || defined(ARDUINO_AVR_ATmega1609) || defined(ARDUINO_AVR_ATmega1608) || \
      defined(ARDUINO_AVR_ATmega809) || defined(ARDUINO_AVR_ATmega808) )

#define USE_WIFI_NINA         false
#define USE_WIFI101           false
#define USE_WIFI_CUSTOM       true

#else

#define USE_WIFI_NINA         false
#define USE_WIFI101           false
#define USE_WIFI_CUSTOM       true

#endif

#elif defined(CONFIG_PLATFORM_8721D)

//#error Ameba Realtek RTL8720DN, RTL8722DM and RTM8722CSM not supported yet
#define USE_WIFI_NINA         false
#define USE_WIFI101           false
#define USE_WIFI_CUSTOM       true

#else

#define USE_WIFI_NINA         false
#define USE_WIFI101           false

// If not USE_WIFI_NINA, you can USE_WIFI_CUSTOM, then include the custom WiFi library here
#define USE_WIFI_CUSTOM       true

#endif

#define WIFI_USING_ESP8266_AT_WEBSERVER         false

#if (!USE_WIFI_NINA && USE_WIFI_CUSTOM)
  #if (defined(ESP8266))
    #include "ESP8266WiFi.h"
  #elif WIFI_USING_ESP8266_AT_WEBSERVER
    #warning WIFI_USING_ESP8266_AT_WEBSERVER
    #error WIFI_USING_ESP8266_AT_WEBSERVER not ready yet
    #include "ESP8266_AT_WebServer.h"
    #define WIFI_USING_ESP_AT     true
  #elif defined(CONFIG_PLATFORM_8721D)
    #include "WiFi.h"
    #define WIFI_USING_ESP_AT     false
  #else
    #warning WIFI_USING_WIFIESPAT
    //#include "WiFi_XYZ.h"
    #include "WiFiEspAT.h"
    #define WIFI_USING_ESP_AT     true
  #endif
#endif

#if WIFI_USING_ESP_AT
  #if defined(Serial1)
    #define EspSerial       Serial1
  #else
    #define EspSerial       Serial
  #endif
#endif

#if USE_WIFI_PORTENTA_H7
  #warning Using Portenta H7 WiFi
  #define SHIELD_TYPE           "Portenta_H7 WiFi"
#elif USE_WIFI_NINA
  #warning Using WiFiNINA using WiFiNINA_Generic Library
  #define SHIELD_TYPE           "WiFiNINA using WiFiNINA_Generic Library"
#elif USE_WIFI101
  #warning Using WiFi101 using WiFi101 Library
  #define SHIELD_TYPE           "WiFi101 using WiFi101 Library"
#elif (ESP32 || ESP8266)
  #warning Using ESP WiFi with WiFi Library
  #define SHIELD_TYPE           "ESP WiFi using WiFi Library"
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  #warning Using RP2040W CYW43439 WiFi
  #define SHIELD_TYPE           "RP2040W CYW43439 WiFi"
#elif USE_WIFI_CUSTOM
  #warning Using Custom WiFi using Custom WiFi Library
  #define SHIELD_TYPE           "Custom WiFi using Custom WiFi Library"
#else
  #define SHIELD_TYPE           "Unknown WiFi shield/Library"
#endif

#if ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
        defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
        defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#if defined(WIFI_USE_NRF528XX)
  #undef WIFI_USE_NRF528XX
#endif
#define WIFI_USE_NRF528XX          true
#endif

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )
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

#if ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) ) && ! ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
#if defined(WIFI_USE_STM32)
  #undef WIFI_USE_STM32
#endif
#define WIFI_USE_STM32      true
#endif

#if ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || \
      defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) )

#if defined(WIFI_USE_RP2040)
  #undef WIFI_USE_RP2040
#endif
#define WIFI_USE_RP2040      true

#endif

#if ( defined(ARDUINO_AVR_ADK) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560) )
  #if defined(WIFI_USE_AVR)
    #undef WIFI_USE_AVR
  #endif
  #define WIFI_USE_AVR      true
  #warning Use AVR Mega architecture from WiFiMulti_Generic
#endif

#ifdef CORE_TEENSY
#if defined(__IMXRT1062__)
  // For Teensy 4.1/4.0
  #define BOARD_TYPE      "TEENSY 4.1/4.0"
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
  #define BOARD_TYPE      "NRF52840_FEATHER_EXPRESS"
#elif defined(NRF52832_FEATHER)
  #define BOARD_TYPE      "NRF52832_FEATHER"
#elif defined(NRF52840_FEATHER_SENSE)
  #define BOARD_TYPE      "NRF52840_FEATHER_SENSE"
#elif defined(NRF52840_ITSYBITSY)
  #define BOARD_TYPE      "NRF52840_ITSYBITSY_EXPRESS"
#elif defined(NRF52840_CIRCUITPLAY)
  #define BOARD_TYPE      "NRF52840_CIRCUIT_PLAYGROUND"
#elif defined(NRF52840_CLUE)
  #define BOARD_TYPE      "NRF52840_CLUE"
#elif defined(NRF52840_METRO)
  #define BOARD_TYPE      "NRF52840_METRO_EXPRESS"
#elif defined(NRF52840_PCA10056)
  #define BOARD_TYPE      "NORDIC_NRF52840DK"
#elif defined(NINA_B302_ublox)
  #define BOARD_TYPE      "NINA_B302_ublox"
#elif defined(NINA_B112_ublox)
  #define BOARD_TYPE      "NINA_B112_ublox"
#elif defined(PARTICLE_XENON)
  #define BOARD_TYPE      "PARTICLE_XENON"
#elif defined(MDBT50Q_RX)
  #define BOARD_TYPE      "RAYTAC_MDBT50Q_RX"
#elif defined(ARDUINO_NRF52_ADAFRUIT)
  #define BOARD_TYPE      "ARDUINO_NRF52_ADAFRUIT"
#else
  #define BOARD_TYPE      "nRF52 Unknown"
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
#elif defined(ADAFRUIT_FEATHER_M0_EXPRESS)
  #define BOARD_TYPE      "SAMD21 ADAFRUIT_FEATHER_M0_EXPRESS"
#elif defined(ADAFRUIT_METRO_M0_EXPRESS)
  #define BOARD_TYPE      "SAMD21 ADAFRUIT_METRO_M0_EXPRESS"
#elif defined(ADAFRUIT_CIRCUITPLAYGROUND_M0)
  #define BOARD_TYPE      "SAMD21 ADAFRUIT_CIRCUITPLAYGROUND_M0"
#elif defined(ADAFRUIT_GEMMA_M0)
  #define BOARD_TYPE      "SAMD21 ADAFRUIT_GEMMA_M0"
#elif defined(ADAFRUIT_TRINKET_M0)
  #define BOARD_TYPE      "SAMD21 ADAFRUIT_TRINKET_M0"
#elif defined(ADAFRUIT_ITSYBITSY_M0)
  #define BOARD_TYPE      "SAMD21 ADAFRUIT_ITSYBITSY_M0"
#elif defined(ARDUINO_SAMD_HALLOWING_M0)
  #define BOARD_TYPE      "SAMD21 ARDUINO_SAMD_HALLOWING_M0"
#elif defined(ADAFRUIT_METRO_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_METRO_M4_EXPRESS"
#elif defined(ADAFRUIT_GRAND_CENTRAL_M4)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_GRAND_CENTRAL_M4"
#elif defined(ADAFRUIT_FEATHER_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_FEATHER_M4_EXPRESS"
#elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_ITSYBITSY_M4_EXPRESS"
#elif defined(ADAFRUIT_TRELLIS_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_TRELLIS_M4_EXPRESS"
#elif defined(ADAFRUIT_PYPORTAL)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYPORTAL"
#elif defined(ADAFRUIT_PYPORTAL_M4_TITANO)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYPORTAL_M4_TITANO"
#elif defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYBADGE_M4_EXPRESS"
#elif defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_METRO_M4_AIRLIFT_LITE"
#elif defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYGAMER_M4_EXPRESS"
#elif defined(ADAFRUIT_PYGAMER_ADVANCE_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYGAMER_ADVANCE_M4_EXPRESS"
#elif defined(ADAFRUIT_PYBADGE_AIRLIFT_M4)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYBADGE_AIRLIFT_M4"
#elif defined(ADAFRUIT_MONSTER_M4SK_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_MONSTER_M4SK_EXPRESS"
#elif defined(ADAFRUIT_HALLOWING_M4_EXPRESS)
  #define BOARD_TYPE      "SAMD51 ADAFRUIT_HALLOWING_M4_EXPRESS"
#elif defined(SEEED_WIO_TERMINAL)
  #define BOARD_TYPE      "SAMD SEEED_WIO_TERMINAL"
#elif defined(SEEED_FEMTO_M0)
  #define BOARD_TYPE      "SAMD SEEED_FEMTO_M0"
#elif defined(SEEED_XIAO_M0)
  #define BOARD_TYPE      "SAMD SEEED_XIAO_M0"
#elif defined(Wio_Lite_MG126)
  #define BOARD_TYPE      "SAMD SEEED Wio_Lite_MG126"
#elif defined(WIO_GPS_BOARD)
  #define BOARD_TYPE      "SAMD SEEED WIO_GPS_BOARD"
#elif defined(SEEEDUINO_ZERO)
  #define BOARD_TYPE      "SAMD SEEEDUINO_ZERO"
#elif defined(SEEEDUINO_LORAWAN)
  #define BOARD_TYPE      "SAMD SEEEDUINO_LORAWAN"
#elif defined(SEEED_GROVE_UI_WIRELESS)
  #define BOARD_TYPE      "SAMD SEEED_GROVE_UI_WIRELESS"
#elif defined(__SAMD21E18A__)
  #define BOARD_TYPE      "SAMD21E18A"
#elif defined(__SAMD21G18A__)
  #define BOARD_TYPE      "SAMD21G18A"
#elif defined(__SAMD51G19A__)
  #define BOARD_TYPE      "SAMD51G19A"
#elif defined(__SAMD51J19A__)
  #define BOARD_TYPE      "SAMD51J19A"
#elif defined(__SAMD51P19A__)
  #define BOARD_TYPE      "__SAMD51P19A__"
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

// For STM32
#warning EspSerial using SERIAL_PORT_HARDWARE, can be Serial or Serial1. See your board variant.h
#define EspSerial     SERIAL_PORT_HARDWARE    //Serial1

#if defined(STM32F0)
  #warning STM32F0 board selected
  #define BOARD_TYPE  "STM32F0"
#elif defined(STM32F1)
  #warning STM32F1 board selected
  #define BOARD_TYPE  "STM32F1"
#elif defined(STM32F2)
  #warning STM32F2 board selected
  #define BOARD_TYPE  "STM32F2"
#elif defined(STM32F3)
  #warning STM32F3 board selected
  #define BOARD_TYPE  "STM32F3"
#elif defined(STM32F4)
  #warning STM32F4 board selected
  #define BOARD_TYPE  "STM32F4"
#elif defined(STM32F7)

  #if defined(ARDUINO_NUCLEO_F767ZI)
    #warning Nucleo-144 NUCLEO_F767ZI board selected, using HardwareSerial Serial1 @ pin D0/RX and D1/TX
    // RX TX
    HardwareSerial Serial1(D0, D1);
  #else

    #warning STM32F7 board selected
    #define BOARD_TYPE  "STM32F7"

  #endif

#elif defined(STM32L0)
  #if defined(ARDUINO_NUCLEO_L053R8)
    #warning Nucleo-64 NUCLEO_L053R8 board selected, using HardwareSerial Serial1 @ pin D0/RX and D1/TX
    // RX TX
    HardwareSerial Serial1(D0, D1);   // (PA3, PA2);
  #else

    #warning STM32L0 board selected
    #define BOARD_TYPE  "STM32L0"

  #endif

#elif defined(STM32L1)
  #warning STM32L1 board selected
  #define BOARD_TYPE  "STM32L1"
#elif defined(STM32L4)
  #warning STM32L4 board selected
  #define BOARD_TYPE  "STM32L4"
#elif defined(STM32H7)
  #warning STM32H7 board selected
  #define BOARD_TYPE  "STM32H7"
#elif defined(STM32G0)
  #warning STM32G0 board selected
  #define BOARD_TYPE  "STM32G0"
#elif defined(STM32G4)
  #warning STM32G4 board selected
  #define BOARD_TYPE  "STM32G4"
#elif defined(STM32WB)
  #warning STM32WB board selected
  #define BOARD_TYPE  "STM32WB"
#elif defined(STM32MP1)
  #warning STM32MP1 board selected
  #define BOARD_TYPE  "STM32MP1"
#else
  #warning STM32 unknown board selected
  #define BOARD_TYPE  "STM32 Unknown"
#endif

#elif defined(BOARD_SIPEED_MAIX_DUINO)

#warning SIPEED_MAIX_DUINO board selected
#define BOARD_TYPE  "BOARD_SIPEED_MAIX_DUINO"

#define EspSerial       Serial1

#elif defined(ESP32)
#warning ESP32 board selected
#define BOARD_TYPE  "ESP32"

#elif defined(ESP8266)
#warning ESP8266 board selected
#define BOARD_TYPE  "ESP8266"

#elif defined(WIFI_USE_RP2040) && WIFI_USE_RP2040

#warning RP2040 board selected

#elif defined(WIFI_USE_SAM_DUE) && WIFI_USE_SAM_DUE

#warning SAM DUE board selected

#elif defined(CONFIG_PLATFORM_8721D)

#warning RTL8720DN board using AmebaD core selected
#define BOARD_TYPE  "RTL8720DN"

#elif ( defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) )

#warning megaAVR board selected

#elif ( defined(__AVR_ATmega4809__) || \
      defined(ARDUINO_AVR_ATmega4809) || defined(ARDUINO_AVR_ATmega4808) || defined(ARDUINO_AVR_ATmega3209) || \
      defined(ARDUINO_AVR_ATmega3208) || defined(ARDUINO_AVR_ATmega1609) || defined(ARDUINO_AVR_ATmega1608) || \
      defined(ARDUINO_AVR_ATmega809) || defined(ARDUINO_AVR_ATmega808) )

#error MegaCoreX megaAVR board not supported

#elif (WIFI_USE_AVR)

#if defined(ARDUINO_AVR_MEGA2560)
  #define BOARD_TYPE      "AVR Mega2560"
#elif defined(ARDUINO_AVR_MEGA)
  #define BOARD_TYPE      "AVR Mega"
#else
  #define BOARD_TYPE      "AVR ADK"
#endif

// For Mega, use Serial1 or Serial3
#define EspSerial Serial3

#elif USE_WIFI_PORTENTA_H7
#warning Portenta_H7 WiFi selected
#else

//#define BOARD_TYPE      "AVR Mega"
#warning Unknown or unsupported board

#endif

#ifndef BOARD_NAME
  #if defined(ARDUINO_BOARD)
    #define BOARD_NAME    ARDUINO_BOARD
  #elif defined(BOARD_TYPE)
    #define BOARD_NAME    BOARD_TYPE
  #else
    #define BOARD_NAME    "Unknown Board"
  #endif
#endif

#include <WiFiMulti_Generic.h>
#include <WiFiWebServer.h>

char your_ssid1[] = "YOUR_SSID";         // your network SSID (name)
char your_pass1[] = "12345678";          // your network password
char your_ssid2[] = "YOUR_SSID";         // your network SSID (name)
char your_pass2[] = "12345678";          // your network password

#endif    //defines_h
