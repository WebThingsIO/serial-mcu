/*
 * SocketPort.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SocketPort.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>

SocketPort::SocketPort(int inSocket, bool blocking)
  : mSocket(inSocket),
    mBlocking(blocking),
    mEof(false) {}

// virtual
int SocketPort::ReadByte() {
  uint8_t data;
  if (recv(mSocket, &data, 1, mBlocking ? 0 : MSG_DONTWAIT) > 0) {
    return data;
  }
  mEof = mBlocking;
  return -1;
}

// virtual
void SocketPort::Write(const void *data, size_t len) {
  send(mSocket, data, len, 0);
}

//virtual
void SocketPort::Close() {
  shutdown(mSocket, SHUT_WR);
  close(mSocket);
  mSocket = -1;
}

void SocketPort::Shutdown() {
  shutdown(mSocket, SHUT_WR);
}

SocketPort::~SocketPort() {
  if (mSocket >= 0) {
    Close();
  }
}
