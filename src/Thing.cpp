/*
 * Thing.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Thing.h"

const Property *Thing::GetProperty(size_t propertyIdx) const {
  if (propertyIdx < mProperties.size()) {
    return &mProperties[propertyIdx];
  }
  return NULL;
}

const Property *Thing::GetProperty(const char *name) const {
  for (size_t propertyIdx = 0; propertyIdx < mProperties.size(); ++propertyIdx) {
    const Property &property = mProperties[propertyIdx];
    if (strcmp(name, property.Name()) == 0) {
      return &property;
    }
  }
  return NULL;
}

JsonObject &Thing::JsonDescription(JsonBuffer &jsonBuffer) const {
  JsonObject &root = jsonBuffer.createObject();
  root["id"] = mId;
  root["name"] = mName;
  root["type"] = mType;
  root["description"] = mDescription;
  root["propertyCount"] = mProperties.size();
  return root;
}

void Thing::SetManager(ManagerBase *managerBase) {
  for (size_t propertyIdx = 0; propertyIdx < PropertyCount(); ++propertyIdx) {
    mProperties[propertyIdx].SetManager(this, managerBase);
  }
}
