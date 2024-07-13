#include <array>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "data.h"

#define IRPIN 25


IRsend irsend(IRPIN);


const uint16_t kHdrMark = 3300;
const uint16_t kHdrSpace = 1600;
const uint16_t kBitMark = 400;
const uint16_t kOneSpace = 1200;
const uint16_t kZeroSpace = 400;
const uint16_t kFreq = 38000;  // Hz. (Guessing the most common frequency.)

void setup() {
  irsend.begin();
  Serial.begin(115200);
}

void loop() {
  Serial.println("Send signal to power on the A/C");

  std::array<uint8_t, 9> data;
  TecoData::power(25, data);

  irsend.sendGeneric(
    kHdrMark, kHdrSpace,
    kBitMark, kOneSpace,
    kBitMark, kZeroSpace,
    kBitMark, kDefaultMessageGap,
    data.data(), 9,  // Bytes
    kFreq, true, kNoRepeat, kDutyDefault
  );

  delay(5000);
}
