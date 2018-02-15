/*
 * socket-client.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(SOCKET_CLIENT_H)
#define SOCKET_CLIENT_H

#include <stddef.h>

class SocketClient
{
public:
  SocketClient();

  char readByte();
  void write(void *data, size_t len);

private:
  int socket;

};

#endif // SOCKET_CLIENT_H
