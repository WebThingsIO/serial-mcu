/*
 * Property.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Property.h"
#include "Manager.h"

JsonVariant Property::Get() const {
  if (mGetter) {
    return mGetter(*this);
  }
  JsonVariant unknown;
  return unknown;
}

void Property::Set(const JsonVariant &val) const {
  if (mSetter) {
    mSetter(*this, val);
  }
  if (mManagerBase) {
    mManagerBase->OnPropertyChanged(*this, val);
  }
}

void Property::SetManager(Thing *thing, ManagerBase *managerBase) {
  mThing = thing;
  mManagerBase = managerBase;
}

// virtual
JsonObject &BooleanProperty::JsonDescription(JsonBuffer &jsonBuffer) const {
  JsonObject &descr = jsonBuffer.createObject();
  descr["name"] = Name();
  descr["type"] = Type();
  descr["value"] = BooleanProperty::Get();
  return descr;
}

// virtual
const char *BooleanProperty::Type() const {
  return "boolean";
}
