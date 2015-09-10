/*
Lidl L08037B outdoors temperature sensor OOK decoder

No checksum decoding is done and no data interpretation besides temperature values.

<1len><0len>: signal
01: 0
02: 1
03: eos
values: 550 1900 4450 9450

1011 0101 1100 0000 1111 0000 1100  240 = 24.0°C
0011 0101 1100 0001 0001 0101 1100  277 = 27.7°C
*/

#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69registers.h>

#define TSIZE 400
#define MAX_0_DUR 100000 // 100 ms
#define MIN_1_DUR 100 // 100 us
#define TOL 50 // +- tolerance

RFM69OOK radio;

uint32_t val, t0 = 0;
byte bits, s0 = 0;
bool gotone = false;


void setup() {
  Serial.begin(115200);

  radio.initialize();
  radio.setFrequencyMHz(433.9);
  radio.receiveBegin();

  Serial.println(F("start"));
  t0 = micros();
}

void loop() {

  bool s = radio.poll();
  uint32_t t = micros();
  uint32_t d = t - t0;

  if (s0 != s) {

    // end of 0
    if (s == 1) {

      if (gotone) {
        if (d > 1900 - TOL && d < 1900 + TOL) {
          val <<= 1;
          bits++;
        } else if (d > 4450 - TOL && d < 4450 + TOL) {
          val <<= 1;
          val |= 1;
          bits++;
        } else if (d > 9450 - TOL && d < 9450 + TOL) {
          if (bits == 28) {
            char buf[50];
            byte bl = sprintf(buf, "%04x", val >> 16);
            buf[bl] = 0;
            Serial.print(buf);
            sprintf(buf, "%04x", val);
            Serial.print(buf);
            Serial.print(F(" = "));
            Serial.print(((val >> 4) & 0xfff) / 10.0);
            Serial.println(F("\260C"));
          }
          val = 0;
          bits = 0;
        }
      }

    // end of 1
    } else {

      if (d > 550 - TOL && d < 550 + TOL) {
        gotone = true;
      } else {
        gotone = false;
      }

    }

    t0 = t;
    s0 = s;
  }

}

