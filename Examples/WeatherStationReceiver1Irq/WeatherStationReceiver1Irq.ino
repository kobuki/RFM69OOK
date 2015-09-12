/*
Lidl L08037B outdoors temperature sensor OOK decoder - interrupt-driven version

No checksum decoding is done and no data interpretation besides temperature values.

<1len><0len>: signal
01: 0
02: 1
03: eos
values: 550 1900 4450 9450

Period: about 35.6 s

1011 0101 1100 0000 1111 0000 1100  0x0f0 = 240 = 24.0°C
0011 0101 1100 0001 0001 0101 1100  0x115 = 277 = 27.7°C
               ^^^^ ^^^^ ^^^^
*/

#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69OOKregisters.h>
#include <SimpleFIFO.h>

#define TOL 50 // +- pulse tolerance in us

RFM69OOK radio;
SimpleFIFO<uint32_t, 10> fifo;

volatile uint32_t val, t0 = 0;
volatile byte bits, s0 = 0;
volatile bool gotone = false;

void setup() {
  Serial.begin(115200);

  radio.attachUserInterrupt(interrupHandler);
  radio.initialize();

  // some easily tunable parameters
  // radio.setBandwidth(OOK_BW_10_4); // generally this default value seems fine (10.4 kHz)
  // radio.setRSSIThreshold(-70);     // |- use higher values when signal is too strong or too much noise
  // radio.setFixedThreshold(6);      // |
  radio.setSensitivityBoost(SENSITIVITY_BOOST_HIGH);

  radio.setFrequencyMHz(433.9);
  radio.receiveBegin();

  Serial.println(F("start"));
  t0 = micros();
}

void loop() {
  if (fifo.count() > 0) {
    uint32_t lval = fifo.dequeue();

    char buf[50];
    byte bl = sprintf(buf, "%04x", lval >> 16);
    buf[bl] = 0;

    Serial.print(micros() / 1000);
    Serial.print(F(": "));
    Serial.print(buf);
    sprintf(buf, "%04x", lval);
    Serial.print(buf);
    Serial.print(F(" = "));
    Serial.print(((lval >> 4) & 0xfff) / 10.0);
    Serial.println(F("\260C"));
  }
}

void interrupHandler(void) {

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
          if (bits == 28) fifo.enqueue(val);
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

