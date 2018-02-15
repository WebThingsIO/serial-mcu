/*
 * StaticBuffer.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(StaticArray_h)
#define StaticArray_h

#include <stddef.h>

template<class T>
struct StaticArray
{
  typedef size_t  size_type;

  StaticArray(const T *elems, size_type numElems)
    : mElem(elems),
      mNumElems(numElems) {}

  size_type size() const {
    return mNumElems;
  }

  const T& operator[](size_type index) const {
    return mElem[index];
  }
private:
  const T *mElem;
  size_type mNumElems;
};

#define STATIC_ARRAY(T, var, elems) StaticArray<T> var(elems, sizeof(elems)/sizeof(elems[0]))

#endif // StaticArray_h
