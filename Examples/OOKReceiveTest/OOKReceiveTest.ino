
#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69registers.h>

#define TSIZE 100

RFM69OOK radio;

unsigned long lastt = 0;
bool s0 = false;

unsigned long tt[TSIZE];
byte pos = 0;

void setup() {
  Serial.begin(115200);
  radio.initialize();
  radio.setFrequencyMHz(868.88);
  // radio.setFrequencyMHz(433.99);
  radio.receiveBegin();

  while (!radio.poll());
}

void loop() {
  bool s = radio.poll();
  unsigned long t = micros();
  
  if (s != s0) {
    unsigned long d = t - lastt;
    if (d > 50 && d < 600 || d > 930000 && d < 1000000) tt[pos++] = d;
    if (pos == TSIZE) {
      s = 1;
      for (byte i = 0; i < pos; i++) {
        Serial.print(i);
        Serial.print(' ');
        Serial.print(s);
        Serial.print(' ');
        Serial.println(tt[i]);
        s = !s;
      }
      pos = 0;
      while (!radio.poll());
    }
    s0 = s;
    lastt = t;
  }
}

