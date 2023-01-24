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
uint8_t currentBrightness = INITIAL_PIXEL_BRIGHTNESS;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIXEL_PIN);

HTTPClient http;
WiFiClient client;

#include "TimeHelpers.h"
#include "WebServer.h"

uint8_t secondMax = 59;
uint8_t minuteMax = 59;
uint8_t hourMax = 23;

uint16_t hueMin = 0;
uint16_t hueMax = 360;

uint8_t digitCount = 6; // The maximum number of binary bits we care about.

/**
 * Convert a counter to a hue value.
 * @param counter The counter value to convert.
 * @param counterMax The max value of that counter.
 * @returns The packed color value for that hue.
 */
uint32_t convertToHSV(uint8_t counter, uint8_t counterMax)
{
  uint16_t hue = map(map(counter, 0, counterMax, hueMin, hueMax), 360, 0, 0, 65535);
  return pixels.gamma32(pixels.ColorHSV(hue, 255, 255));
}

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
  Serial.printf(" complete. IP Address: %s\n", WiFi.localIP().toString().c_str());

  setSyncProvider(&getApiTime); // Sync is every 5m by default.

  LittleFS.begin();
  wifiServerSetup();
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
      if (secondCounter & mask)
        pixels.setPixelColor(bit, convertToHSV(secondCounter, secondMax));

      if (minuteCounter & mask)
        pixels.setPixelColor(bit + digitCount, convertToHSV(minuteCounter, minuteMax));

      if (hourCounter & mask)
        pixels.setPixelColor(bit + (digitCount * 2), convertToHSV(hourCounter, hourMax));

      mask = mask << 1;
    }

    pixels.show();
  }
  server.handleClient();
}
