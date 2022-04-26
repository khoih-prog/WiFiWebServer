/*********************************************************************************************************************************
  WiFiWebServer-impl.h - Dead simple web-server.
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc

  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.8.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for SAMD21, Nano 33 IoT, etc running WiFiNINA
  ...
  1.6.0   K Hoang      13/02/2022 Add support to new ESP32-S3 and ESP32_C3
  1.6.1   K Hoang      13/02/2022 Fix v1.6.0 issue
  1.6.2   K Hoang      22/02/2022 Add support to megaAVR using Arduino megaAVR core
  1.6.3   K Hoang      02/03/2022 Fix decoding error bug
  1.7.0   K Hoang      05/04/2022 Fix issue with Portenta_H7 core v2.7.2+
  1.8.0   K Hoang      26/04/2022 Add WiFiMulti library support and examples.
 **********************************************************************************************************************************/

#pragma once

#ifndef WiFiWebServer_Impl_H
#define WiFiWebServer_Impl_H

#include <Arduino.h>
#include <libb64/cencode.h>

#include "WiFiWebServer.hpp"
#include "utility/RequestHandlersImpl.h"
#include "utility/WiFiDebug.h"
#include "utility/mimetable.h"

const char * AUTHORIZATION_HEADER = "Authorization";

// New to use WWString

/////////////////////////////////////////////////////////////////////////

WWString fromString(const String& str)
{
  return str.c_str();
}

/////////////////////////////////////////////////////////////////////////

WWString fromString(const String&& str)
{
  return str.c_str();
}

/////////////////////////////////////////////////////////////////////////

String fromWWString(const WWString& str)
{
  return str.c_str();
}

/////////////////////////////////////////////////////////////////////////

String fromWWString(const WWString&& str)
{
  return str.c_str();
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

WiFiWebServer::WiFiWebServer(int port)
  : _server(port)
  , _currentMethod(HTTP_ANY)
  , _currentVersion(0)
  , _currentHandler(nullptr)
  , _firstHandler(nullptr)
  , _lastHandler(nullptr)
  , _currentArgCount(0)
  , _currentArgs(nullptr)

#if USE_NEW_WEBSERVER_VERSION
  ,_currentUpload(nullptr)
  ,_postArgs(nullptr)
#endif
  , _headerKeysCount(0)
  , _currentHeaders(nullptr)
  , _contentLength(0)
  , _chunked(false)
{
}

/////////////////////////////////////////////////////////////////////////

WiFiWebServer::~WiFiWebServer() 
{
  if (_currentHeaders)
    delete[]_currentHeaders;
    
  _headerKeysCount = 0;
  RequestHandler* handler = _firstHandler;
  
  while (handler) 
  {
    RequestHandler* next = handler->next();
    delete handler;
    handler = next;
  }
  
  close();
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::begin() 
{
  _currentStatus = HC_NONE;
  _server.begin();
  
  if (!_headerKeysCount)
    collectHeaders(0, 0);
}

/////////////////////////////////////////////////////////////////////////

bool WiFiWebServer::authenticate(const char * username, const char * password) 
{
  if (hasHeader(AUTHORIZATION_HEADER)) 
  {
    String authReq = header(AUTHORIZATION_HEADER);
    
    if (authReq.startsWith("Basic")) 
    {
      authReq = authReq.substring(6);
      authReq.trim();
      char toencodeLen = strlen(username) + strlen(password) + 1;
      char *toencode = new char[toencodeLen + 1];
      
      if (toencode == NULL) 
      {
        authReq = String();
        return false;
      }
      
      char *encoded = new char[base64_encode_expected_len(toencodeLen) + 1];
      
      if (encoded == NULL) 
      {
        authReq = String();
        delete[] toencode;
        return false;
      }
      
      sprintf(toencode, "%s:%s", username, password);
      
      if (base64_encode_chars(toencode, toencodeLen, encoded) > 0 && authReq.equals(encoded)) 
      {
        authReq = String();
        delete[] toencode;
        delete[] encoded;
        return true;
      }
      
      delete[] toencode;
      delete[] encoded;
    }
    
    authReq = String();
  }
  
  return false;
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::requestAuthentication() 
{
  sendHeader("WWW-Authenticate", "Basic realm=\"Login Required\"");
  send(401);
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::on(const String &uri, WiFiWebServer::THandlerFunction handler) 
{
  on(uri, HTTP_ANY, handler);
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::on(const String &uri, HTTPMethod method, WiFiWebServer::THandlerFunction fn) 
{
  on(uri, method, fn, _fileUploadHandler);
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::on(const String &uri, HTTPMethod method, WiFiWebServer::THandlerFunction fn, WiFiWebServer::THandlerFunction ufn) 
{
  _addRequestHandler(new FunctionRequestHandler(fn, ufn, uri, method));
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::addHandler(RequestHandler* handler) 
{
  _addRequestHandler(handler);
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::_addRequestHandler(RequestHandler* handler) 
{
  if (!_lastHandler) 
  {
    _firstHandler = handler;
    _lastHandler = handler;
  }
  else 
  {
    _lastHandler->next(handler);
    _lastHandler = handler;
  }
}

/////////////////////////////////////////////////////////////////////////

//KH
#if USE_NEW_WEBSERVER_VERSION

void WiFiWebServer::handleClient() 
{
  if (_currentStatus == HC_NONE) 
  {
    WiFiClient client = _server.available();
    
    if (!client) 
    {
      //WS_LOGDEBUG(F("handleClient: No Client"));
      return;
    }

    WS_LOGDEBUG(F("handleClient: New Client"));

    _currentClient = client;
    _currentStatus = HC_WAIT_READ;
    _statusChange = millis();
  }

  bool keepCurrentClient = false;
  bool callYield = false;

  if (_currentClient.connected() || _currentClient.available()) 
  {
    switch (_currentStatus) 
    {
    case HC_NONE:
      // No-op to avoid C++ compiler warning
      break;
    case HC_WAIT_READ:
      // Wait for data from client to become available
      if (_currentClient.available()) 
      {
        if (_parseRequest(_currentClient)) 
        {
          _currentClient.setTimeout(HTTP_MAX_SEND_WAIT);
          _contentLength = CONTENT_LENGTH_NOT_SET;
          _handleRequest();

          if (_currentClient.connected()) 
          {
            _currentStatus = HC_WAIT_CLOSE;
            _statusChange = millis();
            keepCurrentClient = true;
          }
        }
      } 
      else 
      { 
        // !_currentClient.available()
        if (millis() - _statusChange <= HTTP_MAX_DATA_WAIT) 
        {
          keepCurrentClient = true;
        }
        
        callYield = true;
      }
      break;
    case HC_WAIT_CLOSE:
      // Wait for client to close the connection
      if (millis() - _statusChange <= HTTP_MAX_CLOSE_WAIT) 
      {
        keepCurrentClient = true;
        callYield = true;
      }
    }
  }

  if (!keepCurrentClient) 
  {
    WS_LOGDEBUG(F("handleClient: Don't keepCurrentClient"));
    _currentClient = WiFiClient();
    _currentStatus = HC_NONE;
    // KH
    //_currentUpload.reset();
  }

  if (callYield) 
  {
    yield();
  }

#if (USE_WIFI_NINA || WIFI_USE_PORTENTA_H7)
  // KH, fix bug relating to New NINA FW 1.4.0. Have to close the connection
  _currentClient.stop();
  WS_LOGDEBUG(F("handleClient: Client disconnected"));
#endif  
}

/////////////////////////////////////////////////////////////////////////
 
#else

/////////////////////////////////////////////////////////////////////////

// KH, rewritten for Portenta H7 from v1.4.0
void WiFiWebServer::handleClient() 
{
  if (_currentStatus == HC_NONE) 
  {
    WiFiClient client = _server.available();
    
    if (!client) 
    {
      return;
    }

    WS_LOGDEBUG(F("handleClient: New Client"));

    _currentClient = client;
    _currentStatus = HC_WAIT_READ;
    _statusChange = millis();
  }
  
  if (!_currentClient.connected()) 
  {
    _currentStatus = HC_NONE;
    
    goto stopClient;
  }

  // Wait for data from client to become available
  if (_currentStatus == HC_WAIT_READ) 
  {
    //WS_LOGDEBUG(F("handleClient: _currentStatus = HC_WAIT_READ"));
    
    if (!_currentClient.available()) 
    {
      //WS_LOGDEBUG(F("handleClient: Client not available"));
            
      if (millis() - _statusChange > HTTP_MAX_DATA_WAIT) 
      {
        WS_LOGDEBUG(F("handleClient: HTTP_MAX_DATA_WAIT Timeout"));

        _currentStatus = HC_NONE;
        
        goto stopClient;
      }
      
      yield();
      return;
    }

    WS_LOGDEBUG(F("handleClient: Parsing Request"));

    if (!_parseRequest(_currentClient)) 
    {
      WS_LOGDEBUG(F("handleClient: Can't parse request"));

      _currentStatus = HC_NONE;
      
      goto stopClient;
    }

    _currentClient.setTimeout(HTTP_MAX_SEND_WAIT);
    _contentLength = CONTENT_LENGTH_NOT_SET;

    //WS_LOGDEBUG(F("handleClient _handleRequest"));
    _handleRequest();

    if (!_currentClient.connected()) 
    {
      WS_LOGINFO(F("handleClient: Connection closed"));
     
      _currentStatus = HC_NONE;
            
      goto stopClient;
    } 
    else 
    {
      _currentStatus = HC_WAIT_CLOSE;
      _statusChange = millis();
      return;
    }
  }

  if (_currentStatus == HC_WAIT_CLOSE) 
  {
    if (millis() - _statusChange > HTTP_MAX_CLOSE_WAIT) 
    {
      _currentStatus = HC_NONE;

      WS_LOGDEBUG(F("handleClient: HTTP_MAX_CLOSE_WAIT Timeout"));

      yield();
    } 
    else 
    {
      yield();
      return;
    }
  }
  
stopClient:
  
#if (USE_WIFI_NINA || WIFI_USE_PORTENTA_H7)  
	// To be used with New NINA FW 1.4.0 and Portenta_H7 WiFi. Have to close the connection
  _currentClient.stop();
  WS_LOGDEBUG(F("handleClient: Client disconnected"));
#endif  
}

#endif

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::close() 
{
  // TODO: Write close method for WIFI library and uncomment this
  //_server.close();
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::stop() 
{
  close();
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::sendHeader(const String& name, const String& value, bool first) 
{
  WWString headerLine = fromString(name);

  headerLine += ": ";
  headerLine += fromString(value);
  headerLine += RETURN_NEWLINE;

  if (first) 
  {
    _responseHeaders = fromWWString(headerLine + fromString(_responseHeaders));
  }
  else 
  {
    _responseHeaders = fromWWString(fromString(_responseHeaders) + headerLine);
  }
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::setContentLength(size_t contentLength) 
{
  _contentLength = contentLength;
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::_prepareHeader(String& response, int code, const char* content_type, size_t contentLength) 
{
  WWString aResponse = fromString(response);
  
  aResponse = "HTTP/1." + fromString(String(_currentVersion)) + " ";
  aResponse += fromString(String(code));
  aResponse += " ";
  aResponse += fromString(_responseCodeToString(code));
  aResponse += RETURN_NEWLINE;

 using namespace mime;
 
  if (!content_type)
      content_type = mimeTable[html].mimeType;

  sendHeader("Content-Type", content_type, true);
  
  if (_contentLength == CONTENT_LENGTH_NOT_SET) 
  {
    sendHeader("Content-Length", String(contentLength));
  } 
  else if (_contentLength != CONTENT_LENGTH_UNKNOWN) 
  {
    sendHeader("Content-Length", String(_contentLength));
  } 
  else if (_contentLength == CONTENT_LENGTH_UNKNOWN && _currentVersion) 
  { 
    //HTTP/1.1 or above client
    //let's do chunked
    _chunked = true;
    sendHeader("Accept-Ranges", "none");
    sendHeader("Transfer-Encoding", "chunked");
  }
  
  WS_LOGDEBUG(F("_prepareHeader sendHeader Conn close"));
  
  sendHeader("Connection", "close");

  aResponse += fromString(_responseHeaders);
  aResponse += RETURN_NEWLINE;
  
  response = fromWWString(aResponse);
  
  _responseHeaders = String("");
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::_prepareHeader(WWString& response, int code, const char* content_type, size_t contentLength) 
{
  response = "HTTP/1." + fromString(String(_currentVersion)) + " ";
  response += fromString(String(code));
  response += " ";
  response += fromString(_responseCodeToString(code));
  response += RETURN_NEWLINE;

 using namespace mime;
 
  if (!content_type)
      content_type = mimeTable[html].mimeType;

  sendHeader("Content-Type", content_type, true);
  
  if (_contentLength == CONTENT_LENGTH_NOT_SET) 
  {
    sendHeader("Content-Length", String(contentLength));
  } 
  else if (_contentLength != CONTENT_LENGTH_UNKNOWN) 
  {
    sendHeader("Content-Length", String(_contentLength));
  } 
  else if (_contentLength == CONTENT_LENGTH_UNKNOWN && _currentVersion) 
  { 
    //HTTP/1.1 or above client
    //let's do chunked
    _chunked = true;
    sendHeader("Accept-Ranges", "none");
    sendHeader("Transfer-Encoding", "chunked");
  }
  
  WS_LOGDEBUG(F("_prepareHeader sendHeader Conn close"));
  
  sendHeader("Connection", "close");

  response += fromString(_responseHeaders);
  response += RETURN_NEWLINE;
  
  _responseHeaders = String("");
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::send(int code, const char* content_type, const String& content) 
{
  WWString header;
  
  // Can we asume the following?
  //if(code == 200 && content.length() == 0 && _contentLength == CONTENT_LENGTH_NOT_SET)
  //  _contentLength = CONTENT_LENGTH_UNKNOWN;

  WS_LOGDEBUG1(F("send1: len = "), content.length());
  WS_LOGDEBUG1(F("content = "), content);

  _prepareHeader(header, code, content_type, content.length());

  _currentClient.write((const uint8_t *)header.c_str(), header.length());

  if (content.length())
  {
    WS_LOGDEBUG1(F("send1: write header = "), fromWWString(header));
    sendContent(content, content.length());
  }
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::send(int code, char* content_type, const String& content, size_t contentLength)
{
  WWString header;

  WS_LOGDEBUG1(F("send2: len = "), contentLength);
  WS_LOGDEBUG1(F("content = "), content);

  char type[64];
  
  memccpy((void*)type, content_type, 0, sizeof(type));
  _prepareHeader(header, code, (const char* )type, contentLength);

  WS_LOGDEBUG1(F("send2: hdrlen = "), header.length());
  WS_LOGDEBUG1(F("header = "), fromWWString(header));

  _currentClient.write((const uint8_t *) header.c_str(), header.length());
  
  if (contentLength)
  {
    sendContent(content, contentLength);
  }
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::send(int code, char* content_type, const String& content) 
{
  send(code, (const char*)content_type, content);
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::send(int code, const String& content_type, const String& content) 
{
  send(code, (const char*)content_type.c_str(), content);
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::sendContent(const String& content) 
{
  const char * footer = RETURN_NEWLINE;
  size_t len = content.length();
  
  if (_chunked) 
  {
    char * chunkSize = (char *) malloc(11);
    
    if (chunkSize) 
    {
      sprintf(chunkSize, "%x%s", len, footer);
      _currentClient.write(chunkSize, strlen(chunkSize));
      free(chunkSize);
    }
  }
  
  _currentClient.write(content.c_str(), len);
  
  if (_chunked) 
  {
    _currentClient.write(footer, 2);
  }
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::sendContent(const String& content, size_t size)
{
  const char * footer = RETURN_NEWLINE;
  
  if (_chunked) 
  {
    char * chunkSize = (char *) malloc(11);
    
    if (chunkSize) 
    {
      WS_LOGDEBUG(F("sendContent: _chunked"));

      sprintf(chunkSize, "%x%s", size, footer);
      _currentClient.write(chunkSize, strlen(chunkSize));
      free(chunkSize);
    }
  }

  WS_LOGDEBUG1(F("sendContent: Client.write content: "), content);
  _currentClient.write(content.c_str(), size);
  
  if (_chunked) {
    _currentClient.write(footer, 2);
  }
}

/////////////////////////////////////////////////////////////////////////

// KH, Restore PROGMEM commands
void WiFiWebServer::send_P(int code, PGM_P content_type, PGM_P content) 
{
  size_t contentLength = 0;

  if (content != NULL) 
  {
    contentLength = strlen_P(content);
  }

  String header;
  char type[64];
  
  memccpy_P((void*)type, (PGM_VOID_P)content_type, 0, sizeof(type));
  _prepareHeader(header, code, (const char* )type, contentLength);

  WS_LOGDEBUG1(F("send_P: len = "), contentLength);
  WS_LOGDEBUG1(F("content = "), content);
  WS_LOGDEBUG1(F("send_P: hdrlen = "), header.length());
  WS_LOGDEBUG1(F("header = "), header);

  _currentClient.write((const uint8_t *) header.c_str(), header.length());
  
  if (contentLength)
  {
    sendContent_P(content);
  }
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::send_P(int code, PGM_P content_type, PGM_P content, size_t contentLength) 
{
  WWString header;
  
  char type[64];
  
  memccpy_P((void*)type, (PGM_VOID_P)content_type, 0, sizeof(type));
  _prepareHeader(header, code, (const char* )type, contentLength);

  WS_LOGDEBUG1(F("send_P: len = "), contentLength);
  WS_LOGDEBUG1(F("content = "), content);
  WS_LOGDEBUG1(F("send_P: hdrlen = "), header.length());
  WS_LOGDEBUG1(F("header = "), fromWWString(header));

  _currentClient.write((const uint8_t *) header.c_str(), header.length());
  
  if (contentLength)
  {
    sendContent_P(content, contentLength);
  }
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::sendContent_P(PGM_P content) 
{
  sendContent_P(content, strlen_P(content));
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::sendContent_P(PGM_P content, size_t size) 
{
  const char * footer = RETURN_NEWLINE;
  
  if (_chunked) 
  {
    char * chunkSize = (char *) malloc(11);
    
    if (chunkSize) 
    {
      sprintf(chunkSize, "%x%s", size, footer);
      _currentClient.write(chunkSize, strlen(chunkSize));
      free(chunkSize);
    }
  }
  
  uint8_t* buffer = new uint8_t[SENDCONTENT_P_BUFFER_SZ];
  
  if (buffer)
  {
    uint16_t count = size / SENDCONTENT_P_BUFFER_SZ;
    uint16_t remainder = size % SENDCONTENT_P_BUFFER_SZ;
    uint16_t i = 0;

    for (i = 0; i < count; i++) 
    {
      /* code */
      memcpy_P(buffer, &content[i * SENDCONTENT_P_BUFFER_SZ], SENDCONTENT_P_BUFFER_SZ);
      _currentClient.write(buffer, SENDCONTENT_P_BUFFER_SZ);
    }
    
    memcpy_P(buffer, &content[i * SENDCONTENT_P_BUFFER_SZ], remainder);
    _currentClient.write(buffer, remainder);
    
    delete [] buffer;
  }
  else
  {
    WS_LOGERROR1(F("sendContent_P: Error, can't allocate buffer, Sz ="), SENDCONTENT_P_BUFFER_SZ);
    return;
  }
  
  if (_chunked) 
  {
    _currentClient.write(footer, 2);
  }
}

/////////////////////////////////////////////////////////////////////////

#if (ESP32 || ESP8266)

#include "FS.h"

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::serveStatic(const char* uri, FS& fs, const char* path, const char* cache_header) 
{
  _addRequestHandler(new StaticFileRequestHandler(fs, path, uri, cache_header));
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::_streamFileCore(const size_t fileSize, const String &fileName, const String &contentType)
{
  using namespace mime;
  
  setContentLength(fileSize);
  
  if (fileName.endsWith(String(FPSTR(mimeTable[gz].endsWith))) &&
      contentType != String(FPSTR(mimeTable[gz].mimeType)) &&
      contentType != String(FPSTR(mimeTable[none].mimeType))) 
  {
    sendHeader(F("Content-Encoding"), F("gzip"));
  }
  
  send(200, contentType, emptyString);
}
#endif

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::arg(const String& name) 
{
  for (int i = 0; i < _currentArgCount; ++i) 
  {
    if ( _currentArgs[i].key == name )
      return _currentArgs[i].value;
  }
  
  return String();
}

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::arg(int i) 
{
  if (i < _currentArgCount)
    return _currentArgs[i].value;
    
  return String();
}

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::argName(int i) 
{
  if (i < _currentArgCount)
    return _currentArgs[i].key;
    
  return String();
}

/////////////////////////////////////////////////////////////////////////

int WiFiWebServer::args() 
{
  return _currentArgCount;
}

/////////////////////////////////////////////////////////////////////////

bool WiFiWebServer::hasArg(const String& name) 
{
  for (int i = 0; i < _currentArgCount; ++i) 
  {
    if (_currentArgs[i].key == name)
      return true;
  }
  
  return false;
}

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::header(const String& name) 
{
  for (int i = 0; i < _headerKeysCount; ++i) 
  {
    if (_currentHeaders[i].key == name)
      return _currentHeaders[i].value;
  }
  
  return String();
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::collectHeaders(const char* headerKeys[], const size_t headerKeysCount) 
{
  _headerKeysCount = headerKeysCount + 1;
  
  if (_currentHeaders)
    delete[]_currentHeaders;
    
  _currentHeaders = new RequestArgument[_headerKeysCount];
  _currentHeaders[0].key = AUTHORIZATION_HEADER;
  
  for (int i = 1; i < _headerKeysCount; i++) 
  {
    _currentHeaders[i].key = headerKeys[i - 1];
  }
}

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::header(int i) 
{
  if (i < _headerKeysCount)
    return _currentHeaders[i].value;
    
  return String();
}

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::headerName(int i) 
{
  if (i < _headerKeysCount)
    return _currentHeaders[i].key;
    
  return String();
}

/////////////////////////////////////////////////////////////////////////

int WiFiWebServer::headers() 
{
  return _headerKeysCount;
}

/////////////////////////////////////////////////////////////////////////

bool WiFiWebServer::hasHeader(const String& name) 
{
  for (int i = 0; i < _headerKeysCount; ++i) 
  {
    if ((_currentHeaders[i].key == name) &&  (_currentHeaders[i].value.length() > 0))
      return true;
  }
  
  return false;
}

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::hostHeader() 
{
  return _hostHeader;
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::onFileUpload(THandlerFunction fn) 
{
  _fileUploadHandler = fn;
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::onNotFound(THandlerFunction fn) 
{
  _notFoundHandler = fn;
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::_handleRequest()
{
  bool handled = false;
  
  if (!_currentHandler)
  {
    WS_LOGDEBUG(F("_handleRequest: request handler not found"));
  }
  else
  {
    WS_LOGDEBUG(F("_handleRequest handle"));
    
    handled = _currentHandler->handle(*this, _currentMethod, _currentUri);
    
    if (!handled)
    {
      WS_LOGDEBUG(F("_handleRequest: _handleRequest failed"));
    }
    else
    {
      WS_LOGDEBUG(F("_handleRequest OK"));
    }
  }

  if (!handled && _notFoundHandler) 
  {
    WS_LOGDEBUG(F("_handleRequest: Call _notFoundHandler"));
    
    _notFoundHandler();
    handled = true;
  }
  
  if (!handled) 
  {
    using namespace mime;
    
    WS_LOGDEBUG(F("_handleRequest: Send Not found"));
    
    send(404, mimeTable[html].mimeType, String("Not found: ") + _currentUri);
    handled = true;
  }
  
  if (handled) 
  {
    WS_LOGDEBUG(F("_handleRequest: _finalizeResponse"));
    
    _finalizeResponse();
  }

/////////////////////////////////////////////////////////////////////////

#if WIFI_USE_PORTENTA_H7
  WS_LOGDEBUG(F("_handleRequest: Clear _currentUri"));
  //_currentUri = String();
  WS_LOGDEBUG(F("_handleRequest: Done Clear _currentUri"));
#else
  _responseHeaders = String("");
#endif  
}

/////////////////////////////////////////////////////////////////////////

void WiFiWebServer::_finalizeResponse() 
{
  if (_chunked) 
  {
    sendContent(String());
  }
}

/////////////////////////////////////////////////////////////////////////

String WiFiWebServer::_responseCodeToString(int code) 
{
  switch (code) 
  {
    case 100: return F("Continue");
    case 101: return F("Switching Protocols");
    case 200: return F("OK");
    case 201: return F("Created");
    case 202: return F("Accepted");
    case 203: return F("Non-Authoritative Information");
    case 204: return F("No Content");
    case 205: return F("Reset Content");
    case 206: return F("Partial Content");
    case 300: return F("Multiple Choices");
    case 301: return F("Moved Permanently");
    case 302: return F("Found");
    case 303: return F("See Other");
    case 304: return F("Not Modified");
    case 305: return F("Use Proxy");
    case 307: return F("Temporary Redirect");
    case 400: return F("Bad Request");
    case 401: return F("Unauthorized");
    case 402: return F("Payment Required");
    case 403: return F("Forbidden");
    case 404: return F("Not Found");
    case 405: return F("Method Not Allowed");
    case 406: return F("Not Acceptable");
    case 407: return F("Proxy Authentication Required");
    case 408: return F("Request Time-out");
    case 409: return F("Conflict");
    case 410: return F("Gone");
    case 411: return F("Length Required");
    case 412: return F("Precondition Failed");
    case 413: return F("Request Entity Too Large");
    case 414: return F("Request-URI Too Large");
    case 415: return F("Unsupported Media Type");
    case 416: return F("Requested range not satisfiable");
    case 417: return F("Expectation Failed");
    case 500: return F("Internal Server Error");
    case 501: return F("Not Implemented");
    case 502: return F("Bad Gateway");
    case 503: return F("Service Unavailable");
    case 504: return F("Gateway Time-out");
    case 505: return F("HTTP Version not supported");
    default:  return "";
  }
}

/////////////////////////////////////////////////////////////////////////

#endif    // WiFiWebServer_Impl_H
