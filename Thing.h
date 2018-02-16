/*
 * Thing.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(Thing_h)
#define Thing_h

#include <ArduinoJson.h>

#include "Property.h"
#include "StaticArray.h"

class Thing {
public:
  Thing(const char *id, const char *name,
        const char *type, const char *description,
        Property *properties, size_t numProperties)
    : mId(id),
      mName(name),
      mType(type),
      mDescription(description),
      mProperties(properties, numProperties) {}

  const char *Id() const {
    return mId;
  }

  const char *Name() const {
    return mName;
  }

  const char *Type() const {
    return mType;
  }

  const char *Description() const {
    return mDescription;
  }

  const Property *GetProperty(size_t propertyIdx) const;
  const Property *GetProperty(const char *name) const;

  const size_t PropertyCount() const {
    return mProperties.size();
  }

  JsonObject &JsonDescription(JsonBuffer &jsonBuffer) const;

  void SetManager(ManagerBase *managerBase);

private:
  const char *mId;
  const char *mName;
  const char *mType;
  const char *mDescription;
  StaticArray<Property> mProperties;
};

#endif // Thing_h
