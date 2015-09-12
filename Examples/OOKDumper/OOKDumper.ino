//
// Simple OOK playground -- receives and dumps received signal and timing
//

#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69OOKregisters.h>

#define TSIZE 400
#define MAX_0_DUR 100000 // 100 ms
#define MIN_1_DUR 100 // 100 us
#define TOL 50 // +- tolerance

RFM69OOK radio;

unsigned long t0 = 0;
uint16_t tt[TSIZE];

byte s0 = 0;
byte pos = 0;
bool restart = true;

void setup() {
  Serial.begin(115200);

  radio.initialize();
  radio.setFrequencyMHz(433.9);
  radio.receiveBegin();

  Serial.println(F("start"));
}

void loop() {

  if (restart) {
    while(!radio.poll());
    t0 = micros();
    s0 = 1;
    pos = 0;
    restart = false;
    return;
  }

  bool s = radio.poll();
  unsigned long t = micros();
  unsigned long d = t - t0;

  if (s0 != s) {
    if (s == 0 && d < MIN_1_DUR) {
      restart = true;
      return;
    }
    tt[pos++] = d;
    t0 = t;
    s0 = s;
  }

  if (pos >= TSIZE || s0 == 0 && d > MAX_0_DUR) {
    Serial.print("time: ");
    Serial.println(micros() / 1000);
    s = 1;
    for (byte i = 0; i < pos; i++) {
      Serial.print(i);
      Serial.print(F(": "));
      Serial.print(s);
      Serial.print(' ');
      Serial.println(tt[i]);
      s = !s;
    }
    restart = true;
  }

}

