MCU side code which talks with the serial-adapter

There is a platformio.ini file which sets up the project for an MKR100.
If you build that variant then it will expose the builtin LED.

For testing/debugging, if you build from the command line by typing `make` then it will build a program called `example` which will run a little
server on port 7788. If you also modify serial-adapter.js and change
the USE_NET constant at the top of the file to true, then the serial
adapter will connect to localhost:7788 instead of using the serial port.

Right now, there is only a BooleanProperty exposed, but this will change
in the future.

When the serial-mcu program wakes up, it will start flashing the led until
the serial port is opened by the host. The flash pattern is twice per second
kind of like a heart beat and indicates that the program is running.

As soon as the usb-serial port is opened, then the LED will go out and
can be controlled via the gateway software.
