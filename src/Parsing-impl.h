/**************************************************************************************************************************************
  Parsing-impl.h - Dead simple web-server.
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

#pragma once

#include <Arduino.h>

#include "WiFiWebServer.h"

#ifndef WEBSERVER_MAX_POST_ARGS
#define WEBSERVER_MAX_POST_ARGS 32
#endif

// KH
#if USE_NEW_WEBSERVER_VERSION

static bool readBytesWithTimeout(WiFiClient& client, size_t maxLength, String& data, int timeout_ms)
{
  if (!data.reserve(maxLength + 1))
    return false;

  data[0] = 0;  // data.clear()??

  while (data.length() < maxLength)
  {
    int tries = timeout_ms;
    size_t avail;

    while (!(avail = client.available()) && tries--)
      delay(1);

    if (!avail)
      break;

    if (data.length() + avail > maxLength)
      avail = maxLength - data.length();

    while (avail--)
      data += (char)client.read();
  }

  return data.length() == maxLength;
}

#else

#if !WIFI_USE_PORTENTA_H7

static char* readBytesWithTimeout(WiFiClient& client, size_t maxLength, size_t& dataLength, int timeout_ms)
{
  char *buf = nullptr;
  dataLength = 0;

  while (dataLength < maxLength)
  {
    int tries = timeout_ms;
    size_t newLength;

    while (!(newLength = client.available()) && tries--)
      delay(1);

    if (!newLength)
    {
      break;
    }

    if (!buf)
    {
      buf = (char *) malloc(newLength + 1);

      if (!buf)
      {
        return nullptr;
      }
    }
    else
    {
      char* newBuf = (char *) realloc(buf, dataLength + newLength + 1);

      if (!newBuf)
      {
        free(buf);
        return nullptr;
      }

      buf = newBuf;
    }

    client.readBytes(buf + dataLength, newLength);
    dataLength += newLength;
    buf[dataLength] = '\0';
  }

  return buf;
}
#endif    // #if !WIFI_USE_PORTENTA_H7

#endif    // #if USE_NEW_WEBSERVER_VERSION

bool WiFiWebServer::_parseRequest(WiFiClient& client)
{
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  client.readStringUntil('\n');

  //reset header value
  for (int i = 0; i < _headerKeysCount; ++i)
  {
    _currentHeaders[i].value = String();
  }

  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start  = req.indexOf(' ');
  int addr_end    = req.indexOf(' ', addr_start + 1);

  if (addr_start == -1 || addr_end == -1)
  {
    WS_LOGDEBUG1(F("_parseRequest: Invalid request: "), req);
    return false;
  }

  String methodStr  = req.substring(0, addr_start);
  String url        = req.substring(addr_start + 1, addr_end);
  String versionEnd = req.substring(addr_end + 8);
  _currentVersion   = atoi(versionEnd.c_str());
  String searchStr  = "";
  int hasSearch     = url.indexOf('?');

  if (hasSearch != -1)
  {
    searchStr = urlDecode(url.substring(hasSearch + 1));
    url = url.substring(0, hasSearch);
  }

  _currentUri = url;
  _chunked = false;

  HTTPMethod method = HTTP_GET;

  // KH
#if USE_NEW_WEBSERVER_VERSION
  if (methodStr == "HEAD")
  {
    method = HTTP_HEAD;
  }
  else if (methodStr == "POST")
  {
    method = HTTP_POST;
  }
  else if (methodStr == "DELETE")
  {
    method = HTTP_DELETE;
  }
  else if (methodStr == "OPTIONS")
  {
    method = HTTP_OPTIONS;
  }
  else if (methodStr == "PUT")
  {
    method = HTTP_PUT;
  }
  else if (methodStr == "PATCH")
  {
    method = HTTP_PATCH;
  }
#else
  if (methodStr == "POST")
  {
    method = HTTP_POST;
  }
  else if (methodStr == "DELETE") {

    method = HTTP_DELETE;
  }
  else if (methodStr == "OPTIONS")
  {
    method = HTTP_OPTIONS;
  }
  else if (methodStr == "PUT")
  {
    method = HTTP_PUT;
  }
  else if (methodStr == "PATCH")
  {
    method = HTTP_PATCH;
  }
#endif

  _currentMethod = method;


  WS_LOGDEBUG1(F("method: "), methodStr);
  WS_LOGDEBUG1(F("url: "), url);
  WS_LOGDEBUG1(F("search: "), searchStr);

  //attach handler
  RequestHandler* handler = nullptr;

  for (handler = _firstHandler; handler; handler = handler->next())
  {
    if (handler->canHandle(_currentMethod, _currentUri))
      break;
  }

  _currentHandler = handler;

  String formData;

  // below is needed only when POST type request
  if (method == HTTP_POST || method == HTTP_PUT || method == HTTP_PATCH || method == HTTP_DELETE)
  {
    String boundaryStr;
    String headerName;
    String headerValue;
    
    bool isForm     = false;
    bool isEncoded  = false;
    uint32_t contentLength  = 0;

    //parse headers
    while (1)
    {
      req = client.readStringUntil('\r');
      client.readStringUntil('\n');

      if (req == "")
        break;//no more headers

      int headerDiv = req.indexOf(':');

      if (headerDiv == -1)
      {
        break;
      }

      headerName  = req.substring(0, headerDiv);
      headerValue = req.substring(headerDiv + 1);
      
      headerValue.trim();
      _collectHeader(headerName.c_str(), headerValue.c_str());

      WS_LOGDEBUG1(F("headerName: "), headerName);
      WS_LOGDEBUG1(F("headerValue: "), headerValue);

      //KH
      if (headerName.equalsIgnoreCase("Content-Type"))
      {
        using namespace mime;
        
        if (headerValue.startsWith(mimeTable[txt].mimeType))
        {
          isForm = false;
        }
        else if (headerValue.startsWith("application/x-www-form-urlencoded"))
        {
          isForm = false;
          isEncoded = true;
        }
        else if (headerValue.startsWith("multipart/"))
        {
          boundaryStr = headerValue.substring(headerValue.indexOf('=') + 1);
          // KH
          boundaryStr.replace("\"", "");
          //
          isForm = true;
        }
      }
      //KH
      else if (headerName.equalsIgnoreCase("Content-Length"))
      {
        contentLength = headerValue.toInt();
      }
      //KH
      else if (headerName.equalsIgnoreCase("Host"))
      {
        _hostHeader = headerValue;
      }
    }

    //KH
#if USE_NEW_WEBSERVER_VERSION
    String plainBuf;

    if (   !isForm
           && // read content into plainBuf
           (   !readBytesWithTimeout(client, contentLength, plainBuf, HTTP_MAX_POST_WAIT)
               || (plainBuf.length() < contentLength)
           )
       )
    {
      return false;
    }

    if (isEncoded)
    {
      // isEncoded => !isForm => plainBuf is not empty
      // add plainBuf in search str
      if (searchStr.length())
        searchStr += '&';

      searchStr += plainBuf;
    }

    // parse searchStr for key/value pairs
    _parseArguments(searchStr);

    if (!isForm)
    {
      if (contentLength)
      {
        // add key=value: plain={body} (post json or other data)
        RequestArgument& arg = _currentArgs[_currentArgCount++];
        arg.key   = F("plain");
        arg.value = plainBuf;
      }
    }
    else
    {
      // isForm is true
      // here: content is not yet read (plainBuf is still empty)
      if (!_parseForm(client, boundaryStr, contentLength))
      {
        return false;
      }
    }
  }
  else
  {
    String headerName;
    String headerValue;

    //parse headers
    while (1)
    {
      req = client.readStringUntil('\r');
      client.readStringUntil('\n');

      if (req == "")
        break;//no more headers

      int headerDiv = req.indexOf(':');

      if (headerDiv == -1)
      {
        break;
      }

      headerName  = req.substring(0, headerDiv);
      headerValue = req.substring(headerDiv + 2);
      _collectHeader(headerName.c_str(), headerValue.c_str());

      WS_LOGDEBUG1(F("headerName:"), headerName);
      WS_LOGDEBUG1(F("headerValue:"), headerValue);

      if (headerName.equalsIgnoreCase(F("Host")))
      {
        _hostHeader = headerValue;
      }
    }

    _parseArguments(searchStr);
  }

  client.flush();

  WS_LOGDEBUG1(F("Request:"), url);
  WS_LOGDEBUG1(F("Arguments:"), searchStr);
  WS_LOGDEBUG (F("Final list of key/value pairs:"));

  for (int i = 0; i < _currentArgCount; i++)
  {
    WS_LOGDEBUG1("key:",   _currentArgs[i].key.c_str());
    WS_LOGDEBUG1("value:", _currentArgs[i].value.c_str());
  }

  return true;

#else

    if (isForm)
    {
      _parseArguments(searchStr);

      if (!_parseForm(client, boundaryStr, contentLength))
      {
        return false;
      }
    }
  }
  else
  {
    String headerName;
    String headerValue;

    //parse headers
    while (1)
    {
      req = client.readStringUntil('\r');
      client.readStringUntil('\n');

      if (req == "")
        break;//no more headers

      int headerDiv = req.indexOf(':');

      if (headerDiv == -1)
      {
        break;
      }

      headerName  = req.substring(0, headerDiv);
      headerValue = req.substring(headerDiv + 2);
      _collectHeader(headerName.c_str(), headerValue.c_str());

      WS_LOGDEBUG1(F("headerName: "), headerName);
      WS_LOGDEBUG1(F("headerValue: "), headerValue);

      if (headerName == "Host")
      {
        _hostHeader = headerValue;
      }
    }

    _parseArguments(searchStr);
  }

  client.flush();

  WS_LOGDEBUG1(F("Request: "), url);
  WS_LOGDEBUG1(F("Arguments: "), searchStr);

  return true;

#endif
}

bool WiFiWebServer::_collectHeader(const char* headerName, const char* headerValue)
{
  for (int i = 0; i < _headerKeysCount; i++)
  {
    //KH
    if (_currentHeaders[i].key.equalsIgnoreCase(headerName))
    {
      _currentHeaders[i].value = headerValue;
      return true;
    }
  }

  return false;
}


#if USE_NEW_WEBSERVER_VERSION

struct storeArgHandler
{
  void operator() (String& key, String& value, const String& data, int equal_index, int pos, int key_end_pos, int next_index)
  {
    key = WiFiWebServer::urlDecode(data.substring(pos, key_end_pos));

    if ((equal_index != -1) && ((equal_index < next_index - 1) || (next_index == -1)))
      value = WiFiWebServer::urlDecode(data.substring(equal_index + 1, next_index));
  }
};

struct nullArgHandler
{
  void operator() (String& key, String& value, const String& data, int equal_index, int pos, int key_end_pos, int next_index)
  {
    (void)key; (void)value; (void)data; (void)equal_index; (void)pos; (void)key_end_pos; (void)next_index;
    // do nothing
  }
};

void WiFiWebServer::_parseArguments(const String& data)
{
  if (_currentArgs)
    delete[] _currentArgs;

  _currentArgCount = _parseArgumentsPrivate(data, nullArgHandler());

  // allocate one more, this is needed because {"plain": plainBuf} is always added
  _currentArgs = new RequestArgument[_currentArgCount + 1];

  (void)_parseArgumentsPrivate(data, storeArgHandler());
}

int WiFiWebServer::_parseArgumentsPrivate(const String& data, vl::Func<void(String&, String&, const String&, int, int, int, int)> handler)
{

  WS_LOGDEBUG1(F("args: "), data);

  size_t pos    = 0;
  int arg_total = 0;

  while (true)
  {
    // skip empty expression
    while (data[pos] == '&' || data[pos] == ';')
      if (++pos >= data.length())
        break;

    // locate separators
    int equal_index = data.indexOf('=', pos);
    int key_end_pos = equal_index;
    int next_index  = data.indexOf('&', pos);
    int next_index2 = data.indexOf(';', pos);

    if ((next_index == -1) || (next_index2 != -1 && next_index2 < next_index))
      next_index = next_index2;

    if ((key_end_pos == -1) || ((key_end_pos > next_index) && (next_index != -1)))
      key_end_pos = next_index;

    if (key_end_pos == -1)
      key_end_pos = data.length();

    // handle key/value
    if ((int)pos < key_end_pos)
    {
      RequestArgument& arg = _currentArgs[arg_total];
      handler(arg.key, arg.value, data, equal_index, pos, key_end_pos, next_index);

      ++arg_total;
      pos = next_index + 1;
    }

    if (next_index == -1)
      break;
  }

  WS_LOGDEBUG1(F("args count: "), arg_total);

  return arg_total;
}

void WiFiWebServer::_uploadWriteByte(uint8_t b)
{
  if (_currentUpload->currentSize == HTTP_UPLOAD_BUFLEN)
  {
    if (_currentHandler && _currentHandler->canUpload(_currentUri))
      _currentHandler->upload(*this, _currentUri, *_currentUpload);

    _currentUpload->totalSize += _currentUpload->currentSize;
    _currentUpload->currentSize = 0;
  }

  _currentUpload->buf[_currentUpload->currentSize++] = b;
}

uint8_t WiFiWebServer::_uploadReadByte(WiFiClient& client)
{
  int res = client.read();

  if (res == -1)
  {
    while (!client.available() && client.connected())
      yield();

    res = client.read();
  }

  return (uint8_t)res;
}

#else

void WiFiWebServer::_parseArguments(String data)
{

  WS_LOGDEBUG1(F("args: "), data);

  if (_currentArgs)
    delete[] _currentArgs;

  _currentArgs = 0;

  if (data.length() == 0)
  {
    _currentArgCount = 0;
    _currentArgs = new RequestArgument[1];

    return;
  }

  _currentArgCount = 1;

  for (int i = 0; i < (int)data.length(); )
  {
    i = data.indexOf('&', i);

    if (i == -1)
      break;

    ++i;
    ++_currentArgCount;
  }

  WS_LOGDEBUG1(F("args count: "), _currentArgCount);

  _currentArgs = new RequestArgument[_currentArgCount + 1];
  
  int pos = 0;
  int iarg;

  for (iarg = 0; iarg < _currentArgCount;)
  {
    int equal_sign_index  = data.indexOf('=', pos);
    int next_arg_index    = data.indexOf('&', pos);

    WS_LOGDEBUG1(F("pos: "), pos);
    WS_LOGDEBUG1(F("=@ "), equal_sign_index);
    WS_LOGDEBUG1(F(" &@ "), next_arg_index);


    if ((equal_sign_index == -1) || ((equal_sign_index > next_arg_index) && (next_arg_index != -1)))
    {
      WS_LOGDEBUG1(F("arg missing value: "), iarg);

      if (next_arg_index == -1)
        break;

      pos = next_arg_index + 1;

      continue;
    }

    RequestArgument& arg = _currentArgs[iarg];
    arg.key   = data.substring(pos, equal_sign_index);
    arg.value = data.substring(equal_sign_index + 1, next_arg_index);

    WS_LOGDEBUG1(F("arg: "), iarg);
    WS_LOGDEBUG1(F("key: "), arg.key);
    WS_LOGDEBUG1(F("value: "), arg.value);

    ++iarg;

    if (next_arg_index == -1)
      break;

    pos = next_arg_index + 1;
  }

  _currentArgCount = iarg;

  WS_LOGDEBUG1(F("args count: "), _currentArgCount);
}

void WiFiWebServer::_uploadWriteByte(uint8_t b)
{
  if (_currentUpload.currentSize == HTTP_UPLOAD_BUFLEN)
  {
    if (_currentHandler && _currentHandler->canUpload(_currentUri))
      _currentHandler->upload(*this, _currentUri, _currentUpload);

    _currentUpload.totalSize += _currentUpload.currentSize;
    _currentUpload.currentSize = 0;
  }

  _currentUpload.buf[_currentUpload.currentSize++] = b;
}

uint8_t WiFiWebServer::_uploadReadByte(WiFiClient& client)
{
  int res = client.read();

  if (res == -1)
  {
    while (!client.available() && client.connected())
      yield();

    res = client.read();
  }

  return (uint8_t)res;
}

#endif

#if USE_NEW_WEBSERVER_VERSION

bool WiFiWebServer::_parseForm(WiFiClient& client, const String& boundary, uint32_t len)
{
  (void) len;

  WS_LOGDEBUG1(F("Parse Form: Boundary: "), boundary);
  WS_LOGDEBUG1(F("Length: "), len);

  String line;
  int retry = 0;

  do
  {
    line = client.readStringUntil('\r');
    ++retry;
  } while (line.length() == 0 && retry < 3);

  client.readStringUntil('\n');
  
  //start reading the form
  if (line == ("--" + boundary)) 
  {
    if (_postArgs)
      delete[] _postArgs;

    _postArgs = new RequestArgument[WEBSERVER_MAX_POST_ARGS];
    _postArgsLen = 0;

    while (1)
    {
      String argName;
      String argValue;
      String argType;
      String argFilename;
      
      bool argIsFile = false;

      line = client.readStringUntil('\r');
      client.readStringUntil('\n');

      if (line.length() > 19 && line.substring(0, 19).equalsIgnoreCase(F("Content-Disposition")))
      {
        int nameStart = line.indexOf('=');

        if (nameStart != -1)
        {
          argName = line.substring(nameStart + 2);
          nameStart = argName.indexOf('=');

          if (nameStart == -1)
          {
            argName = argName.substring(0, argName.length() - 1);
          }
          else
          {
            argFilename = argName.substring(nameStart + 2, argName.length() - 1);
            argName = argName.substring(0, argName.indexOf('"'));
            argIsFile = true;

            WS_LOGDEBUG1(F("PostArg FileName: "), argFilename);

            //use GET to set the filename if uploading using blob
            if (argFilename == F("blob") && hasArg("filename"))
              argFilename = arg("filename");
          }

          WS_LOGDEBUG1(F("PostArg Name: "), argName);

          using namespace mime;
          
          argType = mimeTable[txt].mimeType;
          line    = client.readStringUntil('\r');
          client.readStringUntil('\n');

          if (line.length() > 12 && line.substring(0, 12).equalsIgnoreCase("Content-Type"))
          {
            argType = line.substring(line.indexOf(':') + 2);
            //skip next line
            client.readStringUntil('\r');
            client.readStringUntil('\n');
          }

          WS_LOGDEBUG1(F("PostArg Type: "), argType);

          if (!argIsFile)
          {
            while (1)
            {
              line = client.readStringUntil('\r');
              client.readStringUntil('\n');

              if (line.startsWith("--" + boundary))
                break;

              if (argValue.length() > 0)
                argValue += "\n";

              argValue += line;
            }

            WS_LOGDEBUG1(F("PostArg Value: "), argValue);

            RequestArgument& arg = _postArgs[_postArgsLen++];
            arg.key = argName;
            arg.value = argValue;

            if (line == ("--" + boundary + "--"))
            {
              WS_LOGDEBUG(F("Done Parsing POST"));

              break;
            }
          }
          else
          {
            //_currentUpload.reset(new HTTPUpload());
            if (!_currentUpload)
              _currentUpload = new HTTPUpload();

            _currentUpload->status = UPLOAD_FILE_START;
            _currentUpload->name = argName;
            _currentUpload->filename = argFilename;
            _currentUpload->type = argType;
            _currentUpload->totalSize = 0;
            _currentUpload->currentSize = 0;
            _currentUpload->contentLength = len;

            WS_LOGDEBUG1(F("Start File: "), _currentUpload->filename);
            WS_LOGDEBUG1(F("Type: "), _currentUpload->type);

            if (_currentHandler && _currentHandler->canUpload(_currentUri))
              _currentHandler->upload(*this, _currentUri, *_currentUpload);

            _currentUpload->status = UPLOAD_FILE_WRITE;
            uint8_t argByte = _uploadReadByte(client);
            
readfile:
            while (argByte != 0x0D)
            {
              if (!client.connected())
                return _parseFormUploadAborted();

              _uploadWriteByte(argByte);
              argByte = _uploadReadByte(client);
            }

            argByte = _uploadReadByte(client);

            if (!client.connected())
              return _parseFormUploadAborted();

            if (argByte == 0x0A)
            {
              argByte = _uploadReadByte(client);

              if (!client.connected())
                return _parseFormUploadAborted();

              if ((char)argByte != '-')
              {
                //continue reading the file
                _uploadWriteByte(0x0D);
                _uploadWriteByte(0x0A);
                goto readfile;
              }
              else
              {
                argByte = _uploadReadByte(client);

                if (!client.connected())
                  return _parseFormUploadAborted();

                if ((char)argByte != '-')
                {
                  //continue reading the file
                  _uploadWriteByte(0x0D);
                  _uploadWriteByte(0x0A);
                  _uploadWriteByte((uint8_t)('-'));
                  goto readfile;
                }
              }

              uint8_t endBuf[boundary.length()];
              client.readBytes(endBuf, boundary.length());

              if (strstr((const char*)endBuf, boundary.c_str()) != NULL)
              {
                if (_currentHandler && _currentHandler->canUpload(_currentUri))
                  _currentHandler->upload(*this, _currentUri, *_currentUpload);

                _currentUpload->totalSize += _currentUpload->currentSize;
                _currentUpload->status = UPLOAD_FILE_END;

                if (_currentHandler && _currentHandler->canUpload(_currentUri))
                  _currentHandler->upload(*this, _currentUri, *_currentUpload);

                WS_LOGDEBUG1(F("End File: "), _currentUpload->filename);
                WS_LOGDEBUG1(F("Type: "), _currentUpload->type);
                WS_LOGDEBUG1(F("Size: "), _currentUpload->totalSize);

                line = client.readStringUntil(0x0D);
                client.readStringUntil(0x0A);

                if (line == "--")
                {
                  WS_LOGDEBUG(F("Done Parsing POST"));
                  break;
                }
                
                continue;
              }
              else
              {
                _uploadWriteByte(0x0D);
                _uploadWriteByte(0x0A);
                _uploadWriteByte((uint8_t)('-'));
                _uploadWriteByte((uint8_t)('-'));
                
                uint32_t i = 0;

                while (i < boundary.length())
                {
                  _uploadWriteByte(endBuf[i++]);
                }

                argByte = _uploadReadByte(client);
                goto readfile;
              }
            }
            else
            {
              _uploadWriteByte(0x0D);
              goto readfile;
            }

            break;
          }
        }
      }
    }

    int iarg;
    int totalArgs = ((WEBSERVER_MAX_POST_ARGS - _postArgsLen) < _currentArgCount) ? (WEBSERVER_MAX_POST_ARGS - _postArgsLen) : _currentArgCount;

    for (iarg = 0; iarg < totalArgs; iarg++)
    {
      RequestArgument& arg = _postArgs[_postArgsLen++];
      arg.key = _currentArgs[iarg].key;
      arg.value = _currentArgs[iarg].value;
    }

    if (_currentArgs)
      delete[] _currentArgs;

    _currentArgs = new RequestArgument[_postArgsLen];

    for (iarg = 0; iarg < _postArgsLen; iarg++)
    {
      RequestArgument& arg = _currentArgs[iarg];
      arg.key = _postArgs[iarg].key;
      arg.value = _postArgs[iarg].value;
    }

    _currentArgCount = iarg;

    if (_postArgs)
    {
      delete[] _postArgs;
      _postArgs = nullptr;
      _postArgsLen = 0;
    }
    
    return true;
  }

  WS_LOGDEBUG1(F("Error: line: "), line);

  return false;
}

bool WiFiWebServer::_parseFormUploadAborted()
{
  _currentUpload->status = UPLOAD_FILE_ABORTED;

  if (_currentHandler && _currentHandler->canUpload(_currentUri))
    _currentHandler->upload(*this, _currentUri, *_currentUpload);

  return false;
}

#else


bool WiFiWebServer::_parseForm(WiFiClient& client, String boundary, uint32_t len) {

  WS_LOGDEBUG1(F("Parse Form: Boundary: "), boundary);
  WS_LOGDEBUG1(F("Length: "), len);

  String line;
  int retry = 0;

  do
  {
    line = client.readStringUntil('\r');
    ++retry;
  } while (line.length() == 0 && retry < 3);

  client.readStringUntil('\n');

  //start reading the form
  if (line == ("--" + boundary))
  {
    RequestArgument* postArgs = new RequestArgument[32];
    int postArgsLen = 0;

    while (1)
    {
      String argName;
      String argValue;
      String argType;
      String argFilename;
      
      bool argIsFile = false;

      line = client.readStringUntil('\r');
      client.readStringUntil('\n');

      if (line.startsWith("Content-Disposition"))
      {
        int nameStart = line.indexOf('=');

        if (nameStart != -1)
        {
          argName   = line.substring(nameStart + 2);
          nameStart = argName.indexOf('=');

          if (nameStart == -1)
          {
            argName = argName.substring(0, argName.length() - 1);
          }
          else
          {
            argFilename = argName.substring(nameStart + 2, argName.length() - 1);
            argName     = argName.substring(0, argName.indexOf('"'));
            argIsFile   = true;

            WS_LOGDEBUG1(F("PostArg FileName: "), argFilename);

            //use GET to set the filename if uploading using blob
            if (argFilename == "blob" && hasArg("filename"))
              argFilename = arg("filename");
          }

          WS_LOGDEBUG1(F("PostArg Name: "), argName);

          argType = "text/plain";
          line    = client.readStringUntil('\r');
          client.readStringUntil('\n');

          if (line.startsWith("Content-Type"))
          {
            argType = line.substring(line.indexOf(':') + 2);
            //skip next line
            client.readStringUntil('\r');
            client.readStringUntil('\n');
          }

          WS_LOGDEBUG1(F("PostArg Type: "), argType);

          if (!argIsFile)
          {
            while (1)
            {
              line = client.readStringUntil('\r');
              client.readStringUntil('\n');

              if (line.startsWith("--" + boundary))
                break;

              if (argValue.length() > 0)
                argValue += "\n";

              argValue += line;
            }

            WS_LOGDEBUG1(F("PostArg Value: "), argValue);

            RequestArgument& arg = postArgs[postArgsLen++];
            arg.key   = argName;
            arg.value = argValue;

            if (line == ("--" + boundary + "--"))
            {
              WS_LOGDEBUG(F("Done Parsing POST"));

              break;
            }
          }
          else
          {
            _currentUpload.status       = UPLOAD_FILE_START;
            _currentUpload.name         = argName;
            _currentUpload.filename     = argFilename;
            _currentUpload.type         = argType;
            _currentUpload.totalSize    = 0;
            _currentUpload.currentSize  = 0;

            WS_LOGDEBUG1(F("Start File: "), _currentUpload.filename);
            WS_LOGDEBUG1(F("Type: "), _currentUpload.type);

            if (_currentHandler && _currentHandler->canUpload(_currentUri))
              _currentHandler->upload(*this, _currentUri, _currentUpload);

            _currentUpload.status = UPLOAD_FILE_WRITE;
            uint8_t argByte       = _uploadReadByte(client);

readfile:
            while (argByte != 0x0D)
            {
              if (!client.connected())
                return _parseFormUploadAborted();

              _uploadWriteByte(argByte);
              argByte = _uploadReadByte(client);
            }

            argByte = _uploadReadByte(client);

            if (!client.connected())
              return _parseFormUploadAborted();

            if (argByte == 0x0A)
            {
              argByte = _uploadReadByte(client);

              if (!client.connected())
                return _parseFormUploadAborted();

              if ((char)argByte != '-')
              {
                //continue reading the file
                _uploadWriteByte(0x0D);
                _uploadWriteByte(0x0A);
                goto readfile;
              }
              else
              {
                argByte = _uploadReadByte(client);

                if (!client.connected())
                  return _parseFormUploadAborted();

                if ((char)argByte != '-')
                {
                  //continue reading the file
                  _uploadWriteByte(0x0D);
                  _uploadWriteByte(0x0A);
                  _uploadWriteByte((uint8_t)('-'));
                  goto readfile;
                }
              }

              uint8_t endBuf[boundary.length()];
              client.readBytes(endBuf, boundary.length());

              if (strstr((const char*)endBuf, boundary.c_str()) != NULL)
              {
                if (_currentHandler && _currentHandler->canUpload(_currentUri))
                  _currentHandler->upload(*this, _currentUri, _currentUpload);

                _currentUpload.totalSize += _currentUpload.currentSize;
                _currentUpload.status = UPLOAD_FILE_END;

                if (_currentHandler && _currentHandler->canUpload(_currentUri))
                  _currentHandler->upload(*this, _currentUri, _currentUpload);

                WS_LOGDEBUG1(F("End File: "), _currentUpload.filename);
                WS_LOGDEBUG1(F("Type: "), _currentUpload.type);
                WS_LOGDEBUG1(F("Size: "), _currentUpload.totalSize);

                line = client.readStringUntil(0x0D);
                client.readStringUntil(0x0A);

                if (line == "--")
                {
                  WS_LOGDEBUG(F("Done Parsing POST"));

                  break;
                }
                continue;
              }
              else
              {
                _uploadWriteByte(0x0D);
                _uploadWriteByte(0x0A);
                _uploadWriteByte((uint8_t)('-'));
                _uploadWriteByte((uint8_t)('-'));
                
                uint32_t i = 0;

                while (i < boundary.length())
                {
                  _uploadWriteByte(endBuf[i++]);
                }

                argByte = _uploadReadByte(client);
                goto readfile;
              }
            }
            else
            {
              _uploadWriteByte(0x0D);
              goto readfile;
            }

            break;
          }
        }
      }
    }

    int iarg;
    int totalArgs = ((32 - postArgsLen) < _currentArgCount) ? (32 - postArgsLen) : _currentArgCount;

    for (iarg = 0; iarg < totalArgs; iarg++)
    {
      RequestArgument& arg = postArgs[postArgsLen++];
      arg.key = _currentArgs[iarg].key;
      arg.value = _currentArgs[iarg].value;
    }

    if (_currentArgs) delete[] _currentArgs;
    _currentArgs = new RequestArgument[postArgsLen];

    for (iarg = 0; iarg < postArgsLen; iarg++)
    {
      RequestArgument& arg = _currentArgs[iarg];
      arg.key = postArgs[iarg].key;
      arg.value = postArgs[iarg].value;
    }

    _currentArgCount = iarg;

    if (postArgs)
      delete[] postArgs;

    return true;
  }

  WS_LOGDEBUG1(F("Error: line: "), line);

  return false;
}

bool WiFiWebServer::_parseFormUploadAborted()
{
  _currentUpload.status = UPLOAD_FILE_ABORTED;

  if (_currentHandler && _currentHandler->canUpload(_currentUri))
    _currentHandler->upload(*this, _currentUri, _currentUpload);

  return false;
}

#endif

String WiFiWebServer::urlDecode(const String& text)
{
  String decoded    = "";
  char temp[]       = "0x00";
  unsigned int len  = text.length();
  unsigned int i    = 0;

  while (i < len)
  {
    char decodedChar;
    char encodedChar = text.charAt(i++);

    if ((encodedChar == '%') && (i + 1 < len))
    {
      temp[2] = text.charAt(i++);
      temp[3] = text.charAt(i++);

      decodedChar = strtol(temp, NULL, 16);
    }
    else
    {
      if (encodedChar == '+')
      {
        decodedChar = ' ';
      }
      else
      {
        decodedChar = encodedChar;  // normal ascii char
      }
    }

    decoded += decodedChar;
  }

  return decoded;
}

