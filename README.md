# RFM69OOK

This project is an attempt to create a general OOK transceiver library, primarily for LowPowerLab's Moteino (http://lowpowerlab.com/moteino/). It should also work in many other Arduino derivatives that make use of Hope RF's RFM69 radio transceiver modules.

Code is originally based on https://github.com/LowPowerLab/RFM69 by LowPowerLab, with many parts removed, completely rewritten or amended for operation in OOK mode.

OOK sampling code and a complete decoding example for a cheap weather station temperature transmitter is included.

For ongoing development and usage discussion, please see: https://lowpowerlab.com/forum/index.php/topic,1286.0.html

# Necessary modifications to Moteino

First, for this library to work, a simple change needs to be made on the Moteino. In order for the necessary signal to reach the MCU, a pair of digital I/O pins need to be connected between the module and the mote. The following table summarizes the changes necessary.

Moteino model                | RFM69 pin | Moteino pin
---------------------------- | --------- | -------------
All Moteinos with Atmega328p | DIO2      | 3
Content Cell                 | DIO2      | 10
