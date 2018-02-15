/*
 * SocketPort.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(SocketPort_h)
#define SocketPort_h

#include "CommPort.h"

class SocketPort : public CommPort {
public:
  SocketPort(int socket, bool blocking=false);
  virtual ~SocketPort();

  void SetSocket(int socket) {
    mSocket = socket;
  }

  virtual bool Eof() {
    return mEof;
  }

  // Reads a byte from the communications device. Returns the character
  // read or -1 if no character was available.
  virtual int ReadByte();

  // Writes a block of data to the communications device.
  virtual void Write(const void *data, size_t len);

  virtual void Close();

  void Shutdown();

private:
  int mSocket;
  bool mBlocking;
  bool mEof;
};

#endif // SocketPort_h
