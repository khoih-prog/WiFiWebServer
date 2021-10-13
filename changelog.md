## WiFiWebServer Changelog

[![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiWebServer.svg?)](https://www.ardu-badge.com/WiFiWebServer)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/WiFiWebServer.svg)](https://github.com/khoih-prog/WiFiWebServer/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/WiFiWebServer/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/WiFiWebServer.svg)](http://github.com/khoih-prog/WiFiWebServer/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.4.2](#releases-v142)
  * [Releases v1.4.1](#releases-v141)
  * [Major Releases v1.4.0](#major-releases-v140)
  * [Releases v1.3.1](#releases-v131)
  * [Releases v1.3.0](#releases-v130)
  * [Major Releases v1.2.0](#major-releases-v120)
  * [Releases v1.1.1](#releases-v111)
  * [Major Releases v1.1.0](#major-releases-v110)
  * [Releases v1.0.7](#releases-v107)
  * [Releases v1.0.6](#releases-v106)
  * [Releases v1.0.5](#releases-v105)
  * [Releases v1.0.4](#releases-v104)
  * [Releases v1.0.3](#releases-v103)
  * [Releases v1.0.2](#releases-v102)
  * [Releases v1.0.1](#releases-v101)
  * [Initial Releases v1.0.0](#initial-releases-v100)

---
---

## Changelog

### Releases v1.4.2

1. Update `platform.ini` and `library.json` to use original `khoih-prog` instead of `khoih.prog` after PIO fix
2. Update `Packages' Patches`

### Releases v1.4.1

1. Change option for PIO `lib_compat_mode` from default `soft` to `strict` to minimize compile error in crosss-platform
2. Update `Packages' Patches` for many boards

### Major Releases v1.4.0

1. Add support to **Portenta_H7** using [**Arduino mbed_portenta core**](https://github.com/arduino/ArduinoCore-mbed).
2. Update `Packages' Patches` for **Portenta_H7**

### Releases v1.3.1

1. Add support to ESP32/ESP8266 to use in some rare use-cases
2. Update `Packages' Patches`
3. Split `changelog.md` from `README.md`

### Releases v1.3.0

1. Add support to Adafruit nRF52 core v0.22.0+
2. Add support to Raytac MDBT50Q_RX Dongle
3. Update `Packages' Patches`

### Major Releases v1.2.0

1. Add support to **Arduino Nano RP2040 Connect** using [**Arduino mbed OS for Nano boards**](https://github.com/arduino/ArduinoCore-mbed).
2. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Earle Philhower's arduino-pico** v1.5.1+ core](https://github.com/earlephilhower/arduino-pico).
3. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** v2.1.0+ core](https://github.com/arduino/ArduinoCore-mbed).
4. Add to examples the support to ESP32-AT/ESP8266-AT WiFi, using [`WiFiEspAT library`](https://github.com/jandrassy/WiFiEspAT)
5. Fix bugs
6. Update `Packages' Patches`

### Releases v1.1.1

1. Clean-up all compiler warnings possible.
2. Add MQTT examples
3. Add Version String 

### Major Releases v1.1.0

1. Add high-level **HTTP and WebSockets Client** by merging [ArduinoHttpClient Library](https://github.com/arduino-libraries/ArduinoHttpClient)
2. Add many more examples for HTTP and WebSockets Client.

### Releases v1.0.7

1. Add support to **PROGMEM-related commands, such as sendContent_P() and send_P()**
2. Update Platform.ini to support **PlatformIO 5.x owner-based dependency declaration.**
3. Clean up code.
4. Update examples.

#### Releases v1.0.6

1. Add support to all **STM32F/L/H/G/WB/MP1** boards.
2. Add support to **Seeeduino SAMD21/SAMD51** boards.
3. Restructure examples. Clean-up code.

#### Releases v1.0.5

1. Fix bug not closing client and releasing socket exposed in NINA Firmware v1.4.0.
2. Enhance examples.

#### Releases v1.0.4

1. Add support to boards using **WiFi101 built-in or shield**. For example MKR1000, Teensy, Mega, etc..
2. Support any future custom WiFi library that meets the no-compiling-error requirements.

#### Releases v1.0.3

1. Add support to **nRF52** boards, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, etc.**

#### Releases v1.0.2

1. Add support to **SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE**.

#### Releases v1.0.1

1. Use new [`WiFiNINA_Generic library`](https://github.com/khoih-prog/WiFiNINA_Generic) to provide support to many more boards running WiFiNINA.

The original WiFiNINA library only supports **Nano-33 IoT**, Arduino MKR WiFi 1010, Arduino MKR VIDOR 4000 and Arduino UNO WiFi Rev.2.

#### Initial Releases v1.0.0

This is simple yet complete WebServer library for `AVR Mega, Teensy, SAMD21, STM32, etc.` boards running WiFi modules/shields (WiFiNINA U-Blox W101, W102, etc.). **The functions are similar and compatible to ESP8266/ESP32 WebServer libraries** to make life much easier to port sketches from ESP8266/ESP32.


