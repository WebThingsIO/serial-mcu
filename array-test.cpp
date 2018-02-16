#include <stdio.h>
#include <stddef.h>

#include "Log.h"

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

const int elems[] = {3, 4, 5};
STATIC_ARRAY(int, arr, elems);

int main(int argc, char **argv) {
  for (size_t i = 0; i < arr.size(); i++) {
    LOG("arr[%zu] = %d\n", i, arr[i]);
  }
  return 0;
}