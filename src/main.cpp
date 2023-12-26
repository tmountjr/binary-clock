#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

#include "secrets.h"

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "Please define WIFI_PASSWORD in include/secrets.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "Please define WIFI_SSID in include/secrets.h"
#endif

#define NUM_LEDS 18
#define PIXEL_PIN 13
#define INITIAL_PIXEL_BRIGHTNESS 16
uint8_t inverted = 0;
uint8_t currentBrightness = INITIAL_PIXEL_BRIGHTNESS;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIXEL_PIN);

HTTPClient http;
WiFiClient client;

#include "TimeHelpers.h"
#include "WebServer.h"

uint8_t digitCount = 6; // The maximum number of binary bits we care about.

uint32_t hours[24];
uint32_t seconds_minutes[60];

void setup()
{
  pinMode(PIXEL_PIN, OUTPUT);
  pixels.setBrightness(currentBrightness);
  pixels.begin();

  Serial.begin(115200);

  Serial.print("\nStarting wifi");
  WiFi.hostname("binary-clock");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  String localIp = WiFi.localIP().toString();
  Serial.printf(" complete. IP Address: %s\n", localIp.c_str());

  setSyncProvider(&getApiTime); // Sync is every 5m by default.

  LittleFS.begin();
  wifiServerSetup();

  // Read in the hours and seconds_minutes files for the color maps.
  File hours_bin = LittleFS.open("/hours.bin", "r");
  if (!hours_bin) {
    Serial.println("Cannot open hours.bin; using red for all hours.");
    // TODO: for debugging purposes, just do a solid fill
    for (int i = 0; i < 24; i++) {
      hours[i] = 16711680; // (255, 0, 0)
    }
  } else {
    Serial.println("hours.bin found, using color wheel information.");
    for (int i = 0; i < 24; i++) {
      hours_bin.readBytes((char*)&hours[i], sizeof(hours[i]));
    }
  }
  hours_bin.close();

  File ms_bin = LittleFS.open("/seconds_minutes.bin", "r");
  if (!ms_bin) {
    Serial.println("Cannot open seconds_minutes.bin; using green for all hours.");
    // TODO: for debugging purposes, just do a solid fill
    for (int i = 0; i < 60; i++) {
      seconds_minutes[i] = 255; // (0, 255, 0)
    }
  } else {
    Serial.println("seconds_minutes.bin found, using color wheel information.");
    for (int i = 0; i < 60; i++) {
      ms_bin.readBytes((char*)&seconds_minutes[i], sizeof(seconds_minutes[i]));
    }
  }
  ms_bin.close();

  #ifdef IP_TRACKER
  char url[128];
  strcpy(url, IP_TRACKER);
  strcat(url, localIp.c_str());
  Serial.printf("\nPinging url %s...\n", url);
  if (http.begin(client, url))
  {
    int httpCode = http.GET();
    Serial.printf("Pinged IP tracker. Status code = %i\n", httpCode);
  }
  #endif
}

void loop()
{
  time_t rightNow = now();
  if (rightNow >= nextUpdateTime)
  {
    nextUpdateTime = rightNow + 1;
    pixels.clear();
    uint8_t mask = 1;

    uint8_t secondCounter = second();
    uint8_t minuteCounter = minute();
    uint8_t hourCounter = hour();

    for (uint8_t bit = 0; bit < digitCount; bit++)
    {
      uint8_t pixelList[3];
      pixelList[0] = bit;                     // Seconds
      pixelList[1] = bit + digitCount;        // Minutes
      pixelList[2] = bit + (digitCount * 2);  // Hours
      if (inverted) {
        pixelList[0] = (digitCount * 3) + (1 - bit) - 2;
        pixelList[1] = (digitCount * 2) + (1 - bit) - 2;
        pixelList[2] = digitCount + (1 - bit) - 2;
      }

      if (secondCounter & mask)
        pixels.setPixelColor(pixelList[0], seconds_minutes[secondCounter % 60]);

      if (minuteCounter & mask)
        pixels.setPixelColor(pixelList[1], seconds_minutes[minuteCounter % 60]);

      if (hourCounter & mask)
        pixels.setPixelColor(pixelList[2], hours[hourCounter % 24]);

      mask = mask << 1;
    }

    pixels.show();
  }
  server.handleClient();
}
