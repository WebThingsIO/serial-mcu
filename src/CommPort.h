/*
 * CommPort.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(CommPort_h)
#define CommPort_h

#include <stddef.h>

class CommPort {
public:
  CommPort() {}
  virtual ~CommPort() {}

  virtual bool Eof() { return false; }

  // Reads a byte from the communications device. Returns the character
  // read or -1 if no character was available.
  virtual int ReadByte() = 0;

  // Writes a block of data to the communications device.
  virtual void Write(const void *data, size_t len) = 0;

  virtual void Close() {}
};

#endif // CommPort_h
