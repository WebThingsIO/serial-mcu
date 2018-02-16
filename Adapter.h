/*
 * Adapter.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(Adapter_h)
#define Adapter_h

#include <stddef.h>

#include "StaticArray.h"
#include "Thing.h"

class Adapter {
public:
  Adapter(const char *id, const char *name,
          Thing *things, size_t numThings)
    : mId(id),
      mName(name),
      mThings(things, numThings) {}

  const char *Id() const {
    return mId;
  }

  const char *Name() const {
    return mName;
  }

  const Thing *GetThing(size_t thingIdx) const;
  const Thing *GetThing(const char *id) const;

  size_t ThingCount() const {
    return mThings.size();
  }

  JsonObject &JsonDescription(JsonBuffer &jsonBuffer) const;

  void SetManager(ManagerBase *managerBase);

private:
  const char *mId;
  const char *mName;
  StaticArray<Thing> mThings;
};

#endif // Adapter_h
