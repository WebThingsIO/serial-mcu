#include <stdio.h>
#include <ArduinoJson.h>

int main(int argc, char ** argv) {
  StaticJsonBuffer<400> jsonBuffer;

  const char *input = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  printf("    input = '%s'\n", input);

  JsonObject &root = jsonBuffer.parseObject(input);
  if (!root.success()) {
    printf("Parsing failed\n");
    return 1;
  }

  JsonObject &xxx = root["xxx"];
  if (xxx == JsonObject::invalid()) {
    printf("No xxx found\n");
  }
  if (!xxx.success()) {
    printf("No xxx success\n");
  }

  const char* sensor = root["sensor"];
  long time          = root["time"];
  double latitude    = root["data"][0];
  double longitude   = root["data"][1];

  printf("    input = '%s'\n", input);
  printf("   sensor = '%s'\n", sensor);
  printf("     time = %ld\n", time);
  printf(" latitude = %10.6f\n", latitude);
  printf("longitude = %10.6f\n", longitude);

  return 0;
}
