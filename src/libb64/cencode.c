/**************************************************************************************************************************************
  cencoder.c - c source to a base64 decoding algorithm implementation
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.4.2

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
  1.0.1   K Hoang      28/03/2020 Change to use new WiFiNINA_Generic library to support many more boards running WiFiNINA
  1.0.2   K Hoang      28/03/2020 Add support to SAMD51 and SAM DUE boards
  1.0.3   K Hoang      22/04/2020 Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, 
                                  Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc. 
  1.0.4   K Hoang      23/04/2020 Add support to MKR1000 boards using WiFi101 and custom WiFi libraries.
  1.0.5   K Hoang      21/07/2020 Fix bug not closing client and releasing socket.    
  1.0.6   K Hoang      24/07/2020 Add support to all STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 boards. Restructure examples 
  1.0.7   K Hoang      25/09/2020 Restore support to PROGMEM-related commands, such as sendContent_P() and send_P()
  1.1.0   K Hoang      17/11/2020 Add basic HTTP and WebSockets Client by merging ArduinoHttpClient
  1.1.1   K Hoang      27/12/2020 Suppress all possible compiler warnings
  1.2.0   K Hoang      26/05/2021 Add support to RP2040-based boards using Arduino-pico and Arduino mbed_rp2040 core
  1.3.0   K Hoang      14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
  1.3.1   K Hoang      06/09/2021 Add support to ESP32/ESP8266 to use in some rare use-cases
  1.4.0   K Hoang      07/09/2021 Add support to Portenta H7
  1.4.1   K Hoang      04/10/2021 Change option for PIO `lib_compat_mode` from default `soft` to `strict`. Update Packages Patches
  1.4.2   K Hoang      12/10/2021 Update `platform.ini` and `library.json`
 ***************************************************************************************************************************************/

#include "cencode.h"

void base64_init_encodestate(base64_encodestate* state_in)
{
  state_in->step = step_A;
  state_in->result = 0;
  state_in->stepcount = 0;
  state_in->stepsnewline = BASE64_CHARS_PER_LINE;
}


void base64_init_encodestate_nonewlines(base64_encodestate* state_in)
{
  base64_init_encodestate(state_in);
  state_in->stepsnewline = -1;
}

char base64_encode_value(const char n)
{
  char r;

  if (n < 26)
    r = n + 'A';
  else if (n < 26 + 26)
    r = n - 26 + 'a';
  else if (n < 26 + 26 + 10 )
    r = n - 26 - 26 + '0';
  else if (n == 62 )
    r = '+';
  else
    r = '/';
  return r;
}

int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in)
{
  const char* plainchar = plaintext_in;
  const char* const plaintextend = plaintext_in + length_in;
  char* codechar = code_out;
  char result;
  char fragment;

  result = state_in->result;

  switch (state_in->step)
  {
      while (1)
      {
      case step_A:
        if (plainchar == plaintextend)
        {
          state_in->result = result;
          state_in->step = step_A;
          return codechar - code_out;
        }

        fragment = *plainchar++;
        result = (fragment & 0x0fc) >> 2;
        *codechar++ = base64_encode_value(result);
        result = (fragment & 0x003) << 4;
      // falls through
      case step_B:
        if (plainchar == plaintextend)
        {
          state_in->result = result;
          state_in->step = step_B;
          return codechar - code_out;
        }

        fragment = *plainchar++;
        result |= (fragment & 0x0f0) >> 4;
        *codechar++ = base64_encode_value(result);
        result = (fragment & 0x00f) << 2;
      // falls through
      case step_C:
        if (plainchar == plaintextend)
        {
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

        if ((state_in->stepcount == BASE64_CHARS_PER_LINE / 4) && (state_in->stepsnewline > 0))
        {
          *codechar++ = '\n';
          state_in->stepcount = 0;
        }
      }
  }

  /* control should not reach here */
  return codechar - code_out;
}

int base64_encode_blockend(char* code_out, base64_encodestate* state_in)
{
  char* codechar = code_out;

  switch (state_in->step)
  {
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

int base64_encode_chars(const char* plaintext_in, int length_in, char* code_out)
{
  base64_encodestate _state;
  base64_init_encodestate(&_state);

  int len = base64_encode_block(plaintext_in, length_in, code_out, &_state);

  return len + base64_encode_blockend((code_out + len), &_state);
}

