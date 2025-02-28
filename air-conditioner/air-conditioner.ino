#include <ctime>
#include <esp_sleep.h>

#include <DHT.h>
#include <DHT_U.h>
#include <HomeSpan.h>
#include <IRsend.h>

#include "ac-controller.h"

#define DHTPIN 32
#define DHTTYPE DHT22
#define IRPIN 17

DHT_Unified dht(DHTPIN, DHTTYPE);
IRsend ir(IRPIN);


void setup() {
  Serial.begin(115200);
  dht.begin();
  ir.begin();

  homeSpan.begin(Category::AirConditioners,"HomeSpan AirConditioner");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Air Conditioner");
    new ACController(&dht, &ir);

  configTzTime("UTC-08", "pool.ntp.org");
}

void loop() {
  homeSpan.poll();

  static std::tm localtm;

  // Check if still syncing with NTP server
  if (!getLocalTime(&localtm)) {
    delay(1000);
    return;
  }
  // Serial.println(&localtm, "%A, %B %d %Y %H:%M:%S");

  // Check the homespan request more frequently if someone is at home
  if (isPeakHour(localtm)) {
    delay(1000);
    return;
  }

  lightSleep();
}

bool isPeakHour(const std::tm &localtm) {
  if (localtm.tm_wday == 0 || localtm.tm_wday == 6) {
    return true;
  }

  if (localtm.tm_hour >= 10 && localtm.tm_hour <= 16) {
    return false;
  }

  if (localtm.tm_hour >= 2 && localtm.tm_hour <= 5) {
    return false;
  }

  return true;
}

/**
 * During light sleep, the WiFi connection is lost, and the Home app UI shows Not Responding.
 *
 */
void lightSleep() {
  int sleep_duration = 30 * 60 * 1000000; // 30 min

  if (esp_sleep_enable_timer_wakeup(sleep_duration) != ESP_OK) {
    Serial.println("Failed to enable timer wakeup");
    return;
  }

  Serial.println("Light sleep start");
  Serial.flush();

  if (esp_light_sleep_start() != ESP_OK) {
    Serial.println("Failed to light sleep");
    return;
  }
}
