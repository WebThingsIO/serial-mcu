#include <Arduino.h>

#include "Adapter.h"
#include "Log.h"
#include "Manager.h"
#include "Packet.h"
#include "Property.h"
#include "SerialPort.h"
#include "StaticArray.h"
#include "StaticBuffer.h"
#include "Thing.h"

bool ledValue = false;

JsonVariant ledGetter(const Property &property) {
  LOG("ledGetter %s is %s\n", property.Name(), ledValue ? "on" : "off");

  return JsonVariant(ledValue);
}

void ledSetter(const Property &property, const JsonVariant &val) {
  ledValue = val.as<bool>();
  LOG("ledSetter setting %s to %s\n", property.Name(), ledValue ? "on" : "off");
  digitalWrite(LED_BUILTIN, ledValue & 1);
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

SerialPort port;
StaticBuffer<256> rxPacketBuffer;
StaticBuffer<256> txPacketBuffer;

Manager<512> manager(port,
                     rxPacketBuffer, txPacketBuffer,
                     ledAdapter);

bool connected = false;
int tick = 0;
unsigned long lastTick = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (!Serial) {
    // Nobody is connected to the serial port. Blink a little heartbeat
    if (connected) {
      tick = 0;
      lastTick = 0;
      connected = false;
    }
    unsigned long now = millis();
    if (now - lastTick >= 100) {
      lastTick = now;
      tick = (tick + 1) % 10;
      if (tick <= 4) {
        digitalWrite(LED_BUILTIN, tick & 1);
      }
    }
  } else {
    if (!connected) {
      on.Set(false);
      connected = true;
    }
    manager.Process();
  }
}