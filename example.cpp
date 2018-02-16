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

bool ledValue = false;

JsonVariant ledGetter(const Property &property) {
  printf("ledGetter %s is %s\n", property.Name(), ledValue ? "on" : "off");

  return JsonVariant(ledValue);
}

void ledSetter(const Property &property, const JsonVariant &val) {
  ledValue = val.as<bool>();
  printf("ledSetter setting %s to %s\n", property.Name(), ledValue ? "on" : "off");
}

BooleanProperty on("on", ledGetter, ledSetter);

#define ADAPTER_ID  "ArduinoLED"

Thing ledThing(ADAPTER_ID "-led-1", // id
               "led",               // name
               "onOffSwitch",       // type
               "Arduino LED",       // Description
               &on, 1);

// The adapter ID should be unique. The thing-id's are created by
// appending the adapter-id with the thing-id, and the UI persists
// these in the database.

Adapter ledAdapter(ADAPTER_ID,
                   "Arduino-LED",
                    &ledThing, 1);

ServerSocketPort port;
StaticBuffer<256> rxPacketBuffer;
StaticBuffer<256> txPacketBuffer;

Manager<512> manager(port,
                     rxPacketBuffer, txPacketBuffer,
                     ledAdapter);

int main(int argc, char **argv) {

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
