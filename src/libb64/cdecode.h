/****************************************************************************************************************************
   cdecoder.h - c source to a base64 decoding algorithm implementation
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
                                    Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc. 
    1.0.4   K Hoang      23/04/2020 Add support to MKR1000 boards using WiFi101 and custom WiFi libraries.
 *****************************************************************************************************************************/
#ifndef BASE64_CDECODE_H
#define BASE64_CDECODE_H

#define base64_decode_expected_len(n) ((n * 3) / 4)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  step_a, step_b, step_c, step_d
} base64_decodestep;

typedef struct {
  base64_decodestep step;
  char plainchar;
} base64_decodestate;

void base64_init_decodestate(base64_decodestate* state_in);

int base64_decode_value(char value_in);

int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in);

int base64_decode_chars(const char* code_in, const int length_in, char* plaintext_out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* BASE64_CDECODE_H */
