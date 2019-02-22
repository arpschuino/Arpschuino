Arpschuino
=======

This repo contains a set of "cores" which adds support for the Arpschuino Board.
www.arpschuino.fr

The following Boards are supported:

- arpschuino
- arpsensors
- arpsensorsRF
- wilulu

Installation and usage
----------------------

To install it: 
In the Arduino IDE (>=1.6.0) open the preference menu and in the board manager add the url : 
https://raw.githubusercontent.com/arpschuino/Arpschuino/master/Core/package_arpschuino_index.json
Open your board manager, i will find the arpschuino Board. Just Install and can select your board in your board menu.
This repo include all the lib used by the Arpschuino project: The Toni Merino DMX lib http://blog.deskontrol.net/arduino-four-universes-dmx-512-library/, A special version of the Jeelib named ArpRFlib, Adafruit_NeoPixel, a modified version of SoftPwm by Karistouf, and the TinyWire used for the I2C communication with Attiny processor.
You will find all the Jacques Bouault exemple sketch for each board under a lib name Arpschuino in your example menu.
