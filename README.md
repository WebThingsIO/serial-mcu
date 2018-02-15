MCU side code which talks with the serial-adapter

Currently, I'm using TCP sockets to emulate the serial port. This makes
development easier. A SerialPort.h/.cpp will plugin the actual serial comms.

To build, clone a copy of ArduinoJson at the same level as the serial-mcu
directory.

You should then be able to build the various examples by typing: `make`

- json-test.cpp - Some test code for playing around with then ArduinoJson library.

- echo-server.cpp - Reads input data and sends it back. Tries to deserialize
the input stream as packets.

- echo-client.cpp - Connects to the echo server. Reads several JSON formatted strings and sends them to the server. Responses are deserialized
into packets, and then further parsed as JSON using the ArduinoJson library.

- example.cpp - Example code that would run on the MCU side of things.

