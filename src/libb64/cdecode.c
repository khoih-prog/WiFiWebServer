/****************************************************************************************************************************
  cdecoder.c - c source to a base64 decoding algorithm implementation

  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.9.5

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
  ...
  1.6.0   K Hoang      13/02/2022 Add support to new ESP32-S3 and ESP32_C3
  1.6.1   K Hoang      13/02/2022 Fix v1.6.0 issue
  1.6.2   K Hoang      22/02/2022 Add support to megaAVR using Arduino megaAVR core
  1.6.3   K Hoang      02/03/2022 Fix decoding error bug
  1.7.0   K Hoang      05/04/2022 Fix issue with Portenta_H7 core v2.7.2+
  1.8.0   K Hoang      26/04/2022 Add WiFiMulti library support and examples
  1.9.0   K Hoang      12/08/2022 Add support to RASPBERRY_PI_PICO_W using CYW4343 WiFi
  1.9.1   K Hoang      13/08/2022 Add WiFiMulti support to RASPBERRY_PI_PICO_W using CYW4343 WiFi
  1.9.2   K Hoang      16/08/2022 Workaround for RP2040W WiFi.status() bug
  1.9.3   K Hoang      16/08/2022 Better workaround for RP2040W WiFi.status() bug using ping() to local gateway
  1.9.4   K Hoang      06/09/2022 Restore support to ESP32 and ESP8266
  1.9.5   K Hoang      10/09/2022 Restore support to Teensy, etc. Fix bug in examples
 *****************************************************************************************************************************/

#if !(ESP32 || ESP8266)

#include "cdecode.h"

int base64_decode_value(int value_in)
{
  static const char decoding[] =
  { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1, -1, 0, 1, 2,
    3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1,
    -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
    47, 48, 49, 50, 51
  };

  static const char decoding_size = sizeof(decoding);
  value_in -= 43;

  if (value_in < 0 || value_in > decoding_size)
    return -1;

  return decoding[(int)value_in];
}

void base64_init_decodestate(base64_decodestate* state_in)
{
  state_in->step = step_a;
  state_in->plainchar = 0;
}

int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in)
{
  const char* codechar = code_in;
  char* plainchar = plaintext_out;
  int fragment;

  *plainchar = state_in->plainchar;

  switch (state_in->step)
  {
      while (1)
      {
      case step_a:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_a;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }

          fragment = base64_decode_value(*codechar++);
        } while (fragment < 0);

        *plainchar    = (fragment & 0x03f) << 2;
        
        // fall through

      case step_b:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_b;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }

          fragment = base64_decode_value(*codechar++);
        } while (fragment < 0);

        *plainchar++ |= (fragment & 0x030) >> 4;
        *plainchar    = (fragment & 0x00f) << 4;
        
        // fall through

      case step_c:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_c;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }

          fragment = base64_decode_value(*codechar++);
        } while (fragment < 0);

        *plainchar++ |= (fragment & 0x03c) >> 2;
        *plainchar    = (fragment & 0x003) << 6;
        
        // fall through

      case step_d:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_d;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }

          fragment = base64_decode_value(*codechar++);
        } while (fragment < 0);

        *plainchar++   |= (fragment & 0x03f);
        
        // fall through
      }
  }

  /* control should not reach here */
  return plainchar - plaintext_out;
}

int base64_decode_chars(const char* code_in, const int length_in, char* plaintext_out) {

  base64_decodestate _state;
  base64_init_decodestate(&_state);
  int len = base64_decode_block(code_in, length_in, plaintext_out, &_state);

  if (len > 0)
    plaintext_out[len] = 0;

  return len;
}

#endif
