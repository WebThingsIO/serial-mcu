/*
 * echo-client.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <unistd.h>
#include <ArduinoJson.h>

#include "ClientSocketPort.h"
#include "Packet.h"
#include "StaticBuffer.h"

#define NUM_TESTS 4
const char *TEST[NUM_TESTS] = {
    "{\"a\":1, \"b\":2, \"c\":3}",
    "{\"a\": 11, \"b\": 22, \"c\":33}",
    "{\"d\": \"This is a 55 test\"}",
    "{\"d\": \"This is a test\"}",
};

StaticBuffer<128> rxBuffer;
StaticJsonBuffer<512> jsonBuffer;

int main(int argc, char **argv)
{
  bool debug = false;
  if (argc == 2 && strcmp(argv[1], "-d") == 0) {
    debug = true;
  }

  ClientSocketPort server;

  if (!server.Connect()) {
    return 0;
  }

  Packet packet(rxBuffer);
  packet.ShowPackets(debug);
  packet.ShowBytes(debug);

  for (int test = 0; test < NUM_TESTS; test++) {
    const char *buf = TEST[test];
    size_t bufLen = strlen(buf);

    printf("Sending: '%s'\n", buf);
    packet.Write(server, buf, bufLen);

    // A bit of a hack, but we just add a delay here to allow the
    // echo'd response to come in.
    sleep(1);

    while (true) {
      int byte = server.ReadByte();
      if (byte >= 0) {
        if (packet.ProcessByte(byte)) {
          printf("Got packet: '%s'\n", rxBuffer.Bytes());
          JsonObject &root = jsonBuffer.parseObject(rxBuffer.Bytes());
          if (!root.success()) {
            printf("Doesn't look like a JSON packet\n");
            break;
          }
          printf("Looks like a JSON packet\n");
          for (JsonObject::iterator kv = root.begin(); kv != root.end(); ++kv) {
            printf("  Got: %s : %s\n", kv->key, kv->value.as<char *>());
          }
          break;
        }
      } else {
        printf("No more bytes available\n");
        break;
      }
    }
  }
  server.Close();

  return 0;
}
