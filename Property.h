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

class Property {
public:
  Property(const char *name,
           JsonVariant (*getter)(const Property &),
           void (*setter)(const Property &, const JsonVariant &))
    : mName(name),
      mGetter(getter),
      mSetter(setter) {}

  const char *Name() const {
    return mName;
  }

  virtual JsonObject &Description(JsonBuffer &jsonBuffer) const = 0;

  void Set(const JsonVariant &val) const {
    if (mSetter) {
      mSetter(*this, val);
    }
  }

  JsonVariant Get() const {
    if (mGetter) {
      return mGetter(*this);
    }
    JsonVariant unknown;
    return unknown;
  }

private:
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

  virtual JsonObject &Description(JsonBuffer &jsonBuffer) const {
    JsonObject &descr = jsonBuffer.createObject();
    descr["type"] = "boolean";
    return descr;
  }

  bool Get() const {
    return Property::Get().as<bool>();
  }

  void Set(bool value) const {
    Property::Set(JsonVariant(value));
  }
};

#endif // Property_h
