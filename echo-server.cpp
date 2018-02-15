/*
 * echo-server.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <string.h>

#include "Packet.h"
#include "ServerSocketPort.h"
#include "StaticBuffer.h"

StaticBuffer<128> rxBuffer;

int main(int argc, char **argv)
{
  bool debug = false;
  if (argc == 2 && strcmp(argv[1], "-d") == 0) {
    debug = true;
  }

  ServerSocketPort client;
  while (true) {
    if (client.Connect()) {
      Packet packet(rxBuffer);
      packet.ShowPackets(debug);
      packet.ShowBytes(debug);
      while (true) {
        int byte = client.ReadByte();
        if (packet.ProcessByte(byte)) {
            printf("Got packet: '%s'\n", rxBuffer.Bytes());
        }
        if (byte >= 0) {
          uint8_t buf = byte;
          client.Write(&buf, sizeof(buf));
        } else {
          break;
        }
      }
      client.Close();
    }
  }
  return 0;
}
