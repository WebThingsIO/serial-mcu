/*
 * Buffer.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(Buffer_h)
#define Buffer_h

#include <stddef.h>
#include <stdint.h>

class Buffer {
public:
  Buffer() : mLrc(0), mCapacity(0), mIndex(0) {}

  size_t Capacity() {
    return mCapacity;
  }

  size_t Length() {
    return mIndex;
  }

  uint8_t LRC() {
    return (mLrc ^ 0xff) + 1;
  }

  void Reset() {
    mIndex = 0;
    mLrc = 0;
  }

  virtual bool AddByte(uint8_t byte) {
    if (mIndex >= mCapacity) {
      return false;
    }

    mLrc += byte;
    mIndex++;
    return true;
  }

  virtual bool SetCapacity(size_t capacity) {
    mCapacity = capacity;
    return true;
  }

  void SetLength(size_t length) {
    if (length < mCapacity) {
      mIndex = length;
    } else {
      mIndex = mCapacity;
    }
  }

  virtual uint8_t *Bytes() = 0;
  virtual size_t MaxCapacity() = 0;

protected:
  // Caution: No virtual destructor.
  // If we add a virtual destructor then the Arduino compiler will add
  // malloc() and free() to the binary adding about 700 useless bytes.
  ~Buffer() {}

private:
  // copy constructor and copy operator are private - therefore Buffer
  // object is non-copyable.
  Buffer(const Buffer &);
  Buffer &operator =(const Buffer &);
  uint8_t mLrc;
  size_t mCapacity;
  size_t mIndex;
};

#endif // Buffer_h
