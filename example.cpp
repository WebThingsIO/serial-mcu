/*
 * example.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <ArduinoJson.h>

#include "Adapter.h"
#include "Manager.h"
#include "ServerSocketPort.h"
#include "Packet.h"
#include "Property.h"
#include "StaticArray.h"
#include "StaticBuffer.h"
#include "Thing.h"

//---------------------------------------------------------------------
// Everything below here is the stuff that the user would write. The
// stuff abover here will be extracted into a library
//---------------------------------------------------------------------

bool ledValue = false;

JsonVariant ledGetter(const Property &property) {
  printf("ledGetter %s is %s\n", property.Name(), ledValue ? "on" : "off");

  return JsonVariant(ledValue);
}

void ledSetter(const Property &property, const JsonVariant &val) {
  ledValue = val.as<bool>();
  printf("ledSetter setting %s to %s\n", property.Name(), ledValue ? "on" : "off");
}

const BooleanProperty on("on", ledGetter, ledSetter);

Thing ledThing("led",
          "onOffSwitch",
          "Arduino LED",
          &on, 1);

// The adapter ID should be unique, You can create one using
// https://www.uuidgenerator.net/version1

Adapter ledAdapter("468241c6-103f-11e8-b642-0ed5f89f718b",
  "Arduino-LED",
  &ledThing, 1);

ServerSocketPort port;
StaticBuffer<256> rxPacketBuffer;
StaticBuffer<256> txPacketBuffer;

Manager<512> manager(port,
                     rxPacketBuffer, txPacketBuffer,
                     &ledAdapter, 1);

int main(int argc, char **argv) {

#if 0
  on.Set(true);
  printf("Value is %s\n", on.Get() ? "on" : "off");
  on.Set(false);
  printf("Value is %s\n", on.Get() ? "on" : "off");

  JsonObject &ledDescr = ledThing.Description(jsonBuffer);
  char printBuf[512];
  ledDescr.prettyPrintTo(printBuf, sizeof(printBuf));
  printf("%s\n", printBuf);
  ledDescr.printTo(printBuf, sizeof(printBuf));
  printf("%s\n", printBuf);
#endif

  while (true) {
    if (!port.Connect()) {
      exit(1);
    }
    while (manager.Process()) {
      ;
    }
  }
  return 0;
}
