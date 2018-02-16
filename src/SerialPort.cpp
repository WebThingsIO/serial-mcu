/*
 * SerialPort.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SerialPort.h"

#include <Arduino.h>

SerialPort::SerialPort() {
  Serial.begin(115200);
}

bool SerialPort::Connect() {
  // Wait for the serial port to connect. Needed for native USB
  while (!Serial) {
    ;
  }
  return true;
}

// Reads a byte from the communications device. Returns the character
// read or -1 if no character was available.
// virtual
int SerialPort::ReadByte() {
  if (Serial.available() > 0) {
    return Serial.read();
  }
  return -1;
}

// Writes a block of data to the communications device.
// virtual
void SerialPort::Write(const void *data, size_t len) {
  Serial.write(static_cast<const uint8_t *>(data), len);
}
