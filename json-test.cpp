#include <stdio.h>
#include <ArduinoJson.h>

#include "Log.h"

int main(int argc, char ** argv) {
  StaticJsonBuffer<400> jsonBuffer;

  const char *input = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  LOG("    input = '%s'\n", input);

  JsonObject &root = jsonBuffer.parseObject(input);
  if (!root.success()) {
    LOG("Parsing failed\n");
    return 1;
  }

  JsonObject &xxx = root["xxx"];
  if (xxx == JsonObject::invalid()) {
    LOG("No xxx found\n");
  }
  if (!xxx.success()) {
    LOG("No xxx success\n");
  }

  #if USE_LOG
  const char* sensor = root["sensor"];
  long time          = root["time"];
  double latitude    = root["data"][0];
  double longitude   = root["data"][1];

  LOG("    input = '%s'\n", input);
  LOG("   sensor = '%s'\n", sensor);
  LOG("     time = %ld\n", time);
  LOG(" latitude = %10.6f\n", latitude);
  LOG("longitude = %10.6f\n", longitude);
  #endif

  return 0;
}
