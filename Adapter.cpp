/*
 * Adapter.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Adapter.h"

const Thing *Adapter::GetThing(size_t thingIdx) const {
  if (thingIdx < mThings.size()) {
    return &mThings[thingIdx];
  }
  return NULL;
}

const Thing *Adapter::GetThing(const char *id) const {
  for (size_t thingIdx = 0; thingIdx < mThings.size(); ++thingIdx) {
    const Thing &thing = mThings[thingIdx];
    if (strcmp(id, thing.Id()) == 0) {
      return &thing;
    }
  }
  return NULL;
}

JsonObject &Adapter::JsonDescription(JsonBuffer &jsonBuffer) const {
  JsonObject &root = jsonBuffer.createObject();
  root["id"] = mName;
  root["name"] = mName;
  root["thingCount"] = mThings.size();
  return root;
}

void Adapter::SetManager(ManagerBase *managerBase) {
  for (size_t thingIdx = 0; thingIdx < ThingCount(); ++thingIdx) {
    mThings[thingIdx].SetManager(managerBase);
  }
}

