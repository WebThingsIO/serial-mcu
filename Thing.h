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
  Thing(const char *name, const char *type, const char *description,
        const Property *properties, size_t numProperties)
    : mName(name),
      mType(type),
      mDescription(description),
      mProperties(properties, numProperties) {}

  JsonObject &Description(JsonBuffer &jsonBuffer) const {
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = mName;
    root["type"] = mType;
    root["description"] = mDescription;
    JsonObject &properties = root.createNestedObject("properties");
    for (size_t i = 0; i < mProperties.size(); ++i) {
      const Property &property = mProperties[i];
      properties[property.Name()] = property.Description(jsonBuffer);
    }
    return root;
  }

private:
  const char *mName;
  const char *mType;
  const char *mDescription;
  StaticArray<Property> mProperties;
};

#endif // Thing_h
