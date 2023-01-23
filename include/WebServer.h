#include <LittleFS.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <uri/UriRegex.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(8080);

#define STATUS_RESPONSE_OBJECTS 2

void corsResponse()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(204, "text/plain", "");
}

void invalidRequestResponse(int status = 400, String msg = "Bad Request")
{
  server.send(status, "text/plain", msg);
}

void notFoundResponse()
{
  invalidRequestResponse(404, "Not Found");
}

void getHomeResponse()
{
  HTTPMethod method = server.method();
  if (!(method == HTTP_GET || method == HTTP_HEAD))
    invalidRequestResponse(405, "Method Not Allowed");
  else
  {
    File index = LittleFS.open("index.html", "r");
    String indexStr;
    if (index.available())
    {
      indexStr = index.readString();
      server.send(200, "text/html", indexStr);
    }
    else
      invalidRequestResponse(503, "Service Unavailable");
  }
}

void setUnixtimeRefreshResponse()
{
  HTTPMethod method = server.method();
  if (method == HTTP_OPTIONS)
    corsResponse();
  else if (method == HTTP_POST) {
    setSyncProvider(&getApiTime);
    time_t toReturn = getApiTime();
    DynamicJsonDocument jsonObject(JSON_OBJECT_SIZE(STATUS_RESPONSE_OBJECTS));
    jsonObject["unixtime"] = toReturn;
    jsonObject["tz_offset"] = tz_offset;

    String jsonObjectString;
    serializeJson(jsonObject, jsonObjectString);

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonObjectString);
  } else {
    invalidRequestResponse(405, "Method Not Allowed");
  }

}

void getUnixtimeResponse()
{
  HTTPMethod method = server.method();
  if (method == HTTP_HEAD)
    server.send(200, "application/json", "");
  else if (method == HTTP_OPTIONS)
    corsResponse();
  else if (method == HTTP_GET)
  {
    DynamicJsonDocument jsonObject(JSON_OBJECT_SIZE(STATUS_RESPONSE_OBJECTS));
    jsonObject["unixtime"] = now();
    jsonObject["tz_offset"] = tz_offset;

    String jsonObjectString;
    serializeJson(jsonObject, jsonObjectString);

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonObjectString);
  }
  else
    invalidRequestResponse(405, "Method Not Allowed");
}

void getStaticAsset()
{
  String filename = server.pathArg(0);
  String ext = filename.substring(filename.lastIndexOf("."));
  String contentType;
  if (ext == ".js")
    contentType = "text/javascript";
  else if (ext == ".css")
    contentType = "text/css";
  else
    contentType = "text/plain";

  String path = "/assets/" + filename + ".gz";
  if (LittleFS.exists(path))
  {
    File index = LittleFS.open(path, "r");
    server.sendHeader("Cache-Control", "max-age: 31556952");
    server.streamFile(index, contentType);
  }
  else
  {
    invalidRequestResponse(404, "Not Found");
  }
}

void wifiServerSetup()
{
  server.enableCORS(true);

  server.on("/unixtime/refresh", setUnixtimeRefreshResponse);
  server.on("/unixtime", getUnixtimeResponse);
  server.on(UriRegex("/assets/(.*)"), getStaticAsset);
  server.on("/", getHomeResponse);

  // Serve any other file requested from flash.
  server.serveStatic("/", LittleFS, "/");

  server.onNotFound(notFoundResponse);
  server.begin();
}