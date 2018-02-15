/*
 * ClientSocketPort.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(ClientSocketPort_h)
#define ClientSocketPort_h

#include "SocketPort.h"

#include <netinet/in.h>

class ClientSocketPort : public SocketPort {
public:
  ClientSocketPort();

  bool Connect();

private:
  struct sockaddr_in mServerAddr;
};

#endif // ClientSocketPort_h
