# Arduino Keyboard

A few files I coded to connect an Arduino Mega 2560 to a keyboard with a broken MIDI controller.

Keyboards are wired in a multiplexing matrix. In order to figure out that matrix, all wires are connected to any of the Arduino pins. `keyboard_init.ino` tests if there is a short circuit (key press) between any pin pair and sends them via serial.

When the matrix is known, `keyboard_midi.ino` can be modified accordingly, provided the pins of the rows/columns in order. This Arduino Sketch uses the [Control Surface](https://github.com/tttapa/Control-Surface) library to send appropriate midi commands via USB.

The Arduino Mega 2560 (likewise the Ardunio Uno) needs to be flashed with a custom firmware to appear natively as a midi device to a computer. For this I used [USBMidiKliK](https://github.com/TheKikGen/USBMidiKliK). It makes it possible to upload new sketches to the Arduino by shorting the MOSI and GND pins when connecting it to the computer. Otherwise, it will appear as a midi device. Flashing the firmware can be done with `dfu-programmer` with the commands in `script.sh`. Serial does not appear to work when using this firmware. The original firmware can be found [here](https://github.com/arduino/ArduinoCore-avr/tree/master/firmwares/atmegaxxu2). 
