
#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69registers.h>

RFM69OOK radio;
unsigned long cnt;

void setup() {
  Serial.begin(115200);
  radio.initialize();
  radio.transmitBegin();
  radio.setFrequencyMHz(868.88);
  // radio.setFrequencyMHz(433.99);
  radio.setPowerLevel(20);
}

void delayMicros(uint32_t d) {
  uint32_t t = micros() + d;
  while(micros() < t);
}

void loop() {
  radio.send(1);
  delayMicros(300L);

  radio.send(0);
  delayMicros(300L);
  
  radio.send(1);
  delayMicros(200L);
  
  radio.send(0);
  delayMicros(200L);

  radio.send(1);
  delayMicros(100L);
  
  radio.send(0);
  delayMicros(950 * 1000L);
}

