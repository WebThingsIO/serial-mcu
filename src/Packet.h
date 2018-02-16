/*
 * Packet.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(Packet_h)
#define Packet_h

#include <stdint.h>
#include "Buffer.h"

class Packet {
public:
  Packet(Buffer &buffer);
  ~Packet();

  enum {
    HeaderSize = 4,
    TrailerSize = 3,
    OverheadSize = HeaderSize + TrailerSize
  };

  uint8_t *Bytes() {
    return mBuffer.Bytes();
  }

  size_t Length() const {
    return mBuffer.Length();
  }

  // Returns true when a packet has been parsed successfully. The
  // packet data will be in the buffer object passed into the
  // constructor.
  bool ProcessByte(uint8_t byte);

  void ShowBytes(bool showBytes) {
    mShowBytes = showBytes;
  }

  void ShowPackets(bool showPackets) {
    mShowPackets = showPackets;
  }

  // Returns a pointer to the associated buffer with space left at the
  // beginning for the packet headers.
  uint8_t *WriteBytes() {
    return mBuffer.Bytes() + HeaderSize;
  }

  size_t WriteSize() {
    return mBuffer.MaxCapacity() - OverheadSize;
  }

  bool FillHeaderAndTrailer(size_t len);

private:
  enum State {
    STATE_SOH,
    STATE_LEN_0,
    STATE_LEN_1,
    STATE_STX,
    STATE_PAYLOAD,
    STATE_ETX,
    STATE_LRC,
    STATE_EOT
  };

  Buffer &mBuffer;
  bool mShowPackets;
  bool mShowBytes;
  State mState;
  size_t mLength;
  static const char *mStateStr[];
};

#endif // Packet_h
