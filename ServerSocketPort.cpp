/*
 * ServerSocketPort.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ServerSocketPort.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 7788

ServerSocketPort::ServerSocketPort()
    : SocketPort(-1, true),
      mServer(-1)
{
  memset(&mServerAddr, 0, sizeof(mServerAddr));
  mServerAddr.sin_family = AF_INET;
  mServerAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  mServerAddr.sin_port = htons(PORT);
}

bool ServerSocketPort::Bind()
{
  mServer = socket(AF_INET, SOCK_STREAM, 0);
  if (mServer < 0)
  {
    fprintf(stderr, "Unable to open socket: %s\n", strerror(errno));
    return false;
  }

  int reuseAddr = 1;
  if (setsockopt(mServer, SOL_SOCKET, SO_REUSEADDR,
                 &reuseAddr, sizeof(reuseAddr)) < 0) {
    fprintf(stderr, "SO_REUSEADDR failed: %s\n", strerror(errno));
    return false;
  }
  #if defined(SO_REUSEPORT)
  int reusePort = 1;
  if (setsockopt(mServer, SOL_SOCKET, SO_REUSEPORT,
                 &reusePort, sizeof(reusePort)) < 0) {
    fprintf(stderr, "SO_REUSEPORT failed: %s\n", strerror(errno));
    return false;
  }
  #endif

  if (bind(mServer, (struct sockaddr *)&mServerAddr, sizeof(mServerAddr)) < 0) {
    fprintf(stderr, "bind failed: %s\n", strerror(errno));
    return false;
  }  

  return true;
}

bool ServerSocketPort::Connect() {
  if (mServer < 0) {
    if (!Bind()) {
      return false;
    }
  }

  printf("Waiting for client to connect...\n");
  if (listen(mServer, 1) < 0) {
    fprintf(stderr, "listen failed: %s\n", strerror(errno));
    return false;
  }

  struct sockaddr_in mClientAddr;
  socklen_t clientAddrLen = sizeof(mClientAddr);
  memset(&mClientAddr, 0, sizeof(mClientAddr));
  int client = accept(mServer, (struct sockaddr *)&mClientAddr, &clientAddrLen);
  if (client < 0) {
    fprintf(stderr, "accept failed: %s\n", strerror(errno));
    return false;
  }
  SetSocket(client);

  char addrBuf[INET_ADDRSTRLEN];
  printf("Connection from %s:%d\n",
          inet_ntop(AF_INET, &mClientAddr.sin_addr.s_addr,
                    addrBuf, sizeof(addrBuf)),
          ntohs(mClientAddr.sin_port));
  return true;
}
