signed long tz_offset = 0L;
time_t nextUpdateTime = 0;

/**
 * Custom time provider.
 * @returns A unix timestamp.
 */
time_t getApiTime()
{
  time_t toReturn = 0;
  if (http.begin(client, "http://worldtimeapi.org/api/timezone/America/New_York"))
  {
    int httpCode = http.GET();
    if (httpCode == 200)
    {
      DynamicJsonDocument apiResp(2048);
      String resp = http.getString();
      DeserializationError e = deserializeJson(apiResp, resp);
      if (!e)
      {
        tz_offset = apiResp["raw_offset"].as<signed long>(); // eg for new york, might be -18000 = -5h
        toReturn = apiResp["unixtime"].as<time_t>() + tz_offset;
        nextUpdateTime = toReturn - 1;
        Serial.printf("Setting unix time to %i\n", (int)toReturn);
      }
    }
    http.end();
  }
  return toReturn;
}
