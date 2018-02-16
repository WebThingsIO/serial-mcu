/*
 * Packet.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Packet.h"

#include <stdio.h>
#include <stddef.h>

#include "Log.h"

// Packet layout:
//
//  <SOH><LenLow><LenHigh><STX><PAYLOAD><ETX><LRC><EOT>

#define SOH 0x01
#define STX 0x02
#define ETX 0x03
#define EOT 0x04

static uint8_t LRC(const void *voidBuf, size_t len) {
  const uint8_t *buf = static_cast<const uint8_t *>(voidBuf);
  uint8_t sum = 0;
  while (len > 0) {
    sum += *buf++;
    len--;
  }
  return (sum ^ 0xff) + 1;
}

const char *Packet::mStateStr[] = {
  "SOH", "LEN_0", "LEN_1", "STX", "PAYLOAD", "ETX", "LRC", "EOT"
};

Packet::Packet(Buffer &buffer)
  : mBuffer(buffer),
    mShowPackets(false),
    mShowBytes(false),
    mState(STATE_SOH),
    mLength(0) {}

Packet::~Packet() {}

bool Packet::ProcessByte(uint8_t byte) {
  #if USE_LOG
  if (mShowBytes) {
    char ch = (byte >= ' ' && byte <= '~') ? byte : '.';
    LOG("State: %7s Rcvd 0x%02x '%c'\n", mStateStr[mState], byte, ch);
  }
  #endif

  switch (mState) {
    case STATE_SOH:
      if (byte == SOH)
        mState = STATE_LEN_0;
      break;
    case STATE_LEN_0:
      mBuffer.Reset();
      mLength = byte;
      mState = STATE_LEN_1;
      break;
    case STATE_LEN_1:
      mLength += ((size_t)byte << 8);
      // We add 1 to allow us to store a terminating null character
      // at the end. This is useful for dealing with packets containing
      // character strings.
      mBuffer.SetCapacity(mLength + 1);
      mState = STATE_STX;
      break;
    case STATE_STX:
      if (byte == STX) {
        if (mLength > 0) {
          mState = STATE_PAYLOAD;
        } else {
          mState = STATE_ETX;
        }
      } else {
        mState = STATE_SOH;
      }
      break;
    case STATE_PAYLOAD:
      if (mBuffer.AddByte(byte)) {
        if (mBuffer.Length() >= mLength) {
          // Add a terminating null.
          mBuffer.AddByte(0);
          mState = STATE_ETX;
        }
      } else {
        // packet wasn't big enough
        mState = STATE_SOH;
      }
      break;
    case STATE_ETX:
      if (byte == ETX) {
        mState = STATE_LRC;
      } else {
        mState = STATE_SOH;
      }
      break;
    case STATE_LRC: {
      uint8_t lrc = mBuffer.LRC();
      if (byte == lrc) {
        mState = STATE_EOT;
      } else {
        LOG("Got LRC: 0x%02x, expected: 0x%02x\n", byte, lrc);
        mState = STATE_SOH;
      }
      break;
    }
    case STATE_EOT:
      mState = STATE_SOH;
      if (byte == EOT) {
        // We've received a packet
        return true;
      }
      break;
    default:
      mState = STATE_SOH;
      break;
  }
  return false;
}

bool Packet::FillHeaderAndTrailer(size_t len) {
  if (len + OverheadSize > mBuffer.MaxCapacity()) {
    return false;
  }
  mBuffer.SetCapacity(OverheadSize + len);
  mBuffer.SetLength(OverheadSize + len);

  uint8_t *header = mBuffer.Bytes();

  header[0] = SOH;
  header[1] = len & 0xff;
  header[2] = (len >> 8) & 0xff;
  header[3] = STX;

  uint8_t *trailer = &header[HeaderSize] + len;
  trailer[0] = ETX;
  trailer[1] = LRC(&header[HeaderSize], len);
  trailer[2] = EOT;

  return true;
}
