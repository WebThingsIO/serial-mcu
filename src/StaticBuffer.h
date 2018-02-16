/*
 * StaticBuffer.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(StaticBuffer_h)
#define StaticBuffer_h

#include "Buffer.h"

template <size_t CAPACITY>
class StaticBuffer : public Buffer {
public:
  explicit StaticBuffer() : Buffer() {}

  virtual bool AddByte(uint8_t byte) {
    size_t index = Length();
    if (Buffer::AddByte(byte)) {
      mBuffer[index] = byte;
      return true;
    }
    return false;
  }

  virtual uint8_t *Bytes() {
    return mBuffer;
  }

  virtual bool SetCapacity(size_t capacity) {
    if (capacity > CAPACITY) {
      return false;
    }
    return Buffer::SetCapacity(capacity);
  }

  virtual size_t MaxCapacity() {
    return CAPACITY;
  }

  // Caution: No virtual destructor.
  // If we add a virtual destructor then the Arduino compiler will add
  // malloc() and free() to the binary adding about 700 useless bytes.
  ~StaticBuffer() {}

private:
  uint8_t mBuffer[CAPACITY];
};

#endif // StaticBuffer_h
