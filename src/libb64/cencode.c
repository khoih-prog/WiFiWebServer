/****************************************************************************************************************************
   cencoder.c - c source to a base64 decoding algorithm implementation
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

#include "cencode.h"

const int CHARS_PER_LINE = 72;

void base64_init_encodestate(base64_encodestate* state_in) {
  state_in->step = step_A;
  state_in->result = 0;
  state_in->stepcount = 0;
}

char base64_encode_value(char value_in) {
  static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  if (value_in > 63) return '=';
  return encoding[(int)value_in];
}

int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in) {
  const char* plainchar = plaintext_in;
  const char* const plaintextend = plaintext_in + length_in;
  char* codechar = code_out;
  char result;
  char fragment;

  result = state_in->result;

  switch (state_in->step) {
      while (1) {
      case step_A:
        if (plainchar == plaintextend) {
          state_in->result = result;
          state_in->step = step_A;
          return codechar - code_out;
        }
        fragment = *plainchar++;
        result = (fragment & 0x0fc) >> 2;
        *codechar++ = base64_encode_value(result);
        result = (fragment & 0x003) << 4;
      case step_B:
        if (plainchar == plaintextend) {
          state_in->result = result;
          state_in->step = step_B;
          return codechar - code_out;
        }
        fragment = *plainchar++;
        result |= (fragment & 0x0f0) >> 4;
        *codechar++ = base64_encode_value(result);
        result = (fragment & 0x00f) << 2;
      case step_C:
        if (plainchar == plaintextend) {
          state_in->result = result;
          state_in->step = step_C;
          return codechar - code_out;
        }
        fragment = *plainchar++;
        result |= (fragment & 0x0c0) >> 6;
        *codechar++ = base64_encode_value(result);
        result  = (fragment & 0x03f) >> 0;
        *codechar++ = base64_encode_value(result);

        ++(state_in->stepcount);
        if (state_in->stepcount == CHARS_PER_LINE / 4) {
          *codechar++ = '\n';
          state_in->stepcount = 0;
        }
      }
  }
  /* control should not reach here */
  return codechar - code_out;
}

int base64_encode_blockend(char* code_out, base64_encodestate* state_in) {
  char* codechar = code_out;

  switch (state_in->step) {
    case step_B:
      *codechar++ = base64_encode_value(state_in->result);
      *codechar++ = '=';
      *codechar++ = '=';
      break;
    case step_C:
      *codechar++ = base64_encode_value(state_in->result);
      *codechar++ = '=';
      break;
    case step_A:
      break;
  }
  *codechar = 0x00;

  return codechar - code_out;
}

int base64_encode_chars(const char* plaintext_in, int length_in, char* code_out) {
  base64_encodestate _state;
  base64_init_encodestate(&_state);
  int len = base64_encode_block(plaintext_in, length_in, code_out, &_state);
  return len + base64_encode_blockend((code_out + len), &_state);
}
