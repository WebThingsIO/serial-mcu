/*
 * ClientSocketPort.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ClientSocketPort.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT  7788

ClientSocketPort::ClientSocketPort()
  : SocketPort(-1, true)
{
  memset(&mServerAddr, 0, sizeof(mServerAddr));
  mServerAddr.sin_family = AF_INET;
  mServerAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  mServerAddr.sin_port = htons(PORT);
}

bool ClientSocketPort::Connect() {
  int client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0) {
    fprintf(stderr, "Unable to open socket: %s\n", strerror(errno));
    return false;
  }
  SetSocket(client);
  if (connect(client, (struct sockaddr *)&mServerAddr, sizeof(mServerAddr)) < 0) {
    fprintf(stderr, "connect failed: %s\n", strerror(errno));
    Close();
    return false;
  }

  char addrBuf[INET_ADDRSTRLEN];
  printf("Connection to %s:%d\n",
          inet_ntop(AF_INET, &mServerAddr.sin_addr.s_addr,
                    addrBuf, sizeof(addrBuf)),
          ntohs(mServerAddr.sin_port));
  return true;
}
