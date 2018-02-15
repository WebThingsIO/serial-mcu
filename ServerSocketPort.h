/*
 * ServerSocketPort.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(ServerSocketPort_h)
#define ServerSocketPort_h

#include "SocketPort.h"

#include <netinet/in.h>

class ServerSocketPort : public SocketPort {
public:
  ServerSocketPort();

  bool Bind();
  bool Connect();

private:
  int mServer;
  struct sockaddr_in mClientAddr;
  struct sockaddr_in mServerAddr;
};

#endif // ServerSocketPort_h
