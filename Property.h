/*
 * Property.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(Property_h)
#define Property_h

#include <ArduinoJson.h>

class ManagerBase;
class Thing;

class Property {
public:
  Property(const char *name,
           JsonVariant (*getter)(const Property &),
           void (*setter)(const Property &, const JsonVariant &))
    : mManagerBase(NULL),
      mThing(NULL),
      mName(name),
      mGetter(getter),
      mSetter(setter) {}

  const char *Name() const {
    return mName;
  }

  const Thing *GetThing() const {
    return mThing;
  }

  virtual const char *Type() const = 0;

  virtual JsonObject &JsonDescription(JsonBuffer &jsonBuffer) const = 0;

  void Set(const JsonVariant &val) const;
  JsonVariant Get() const;

  void SetManager(Thing *thing, ManagerBase *managerBase);

private:
  friend class ManagerBase;
  ManagerBase *mManagerBase;
  const Thing *mThing;
  const char *mName;
  JsonVariant (*mGetter)(const Property &property);
  void (*mSetter)(const Property &property, const JsonVariant &);
};

class BooleanProperty : public Property {
public:
  BooleanProperty(const char *name,
           JsonVariant (*getter)(const Property &),
           void (*setter)(const Property &, const JsonVariant &))
    : Property(name, getter, setter) {}

  virtual JsonObject &JsonDescription(JsonBuffer &jsonBuffer) const;

  virtual const char *Type() const;

  bool Get() const {
    return Property::Get().as<bool>();
  }

  void Set(bool value) const {
    Property::Set(JsonVariant(value));
  }
};

#endif // Property_h
