#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <HomeSpan.h>


/**
 * Check the humidity every 60 seconds.
 * Update HomeKit only if the humidity difference is greater than 1%.
 * Force update the humidity every 10 minutes.
 *
 */
struct HumiditySensor : Service::HumiditySensor {
  SpanCharacteristic *humidity_kit;
  DHT_Unified *dht;
  int check_interval_ms;
  int force_update_interval_ms;
  float humid_diff_threshold;
  
  HumiditySensor(DHT_Unified *dht) : Service::HumiditySensor(), dht(dht) {
    humidity_kit = new Characteristic::CurrentRelativeHumidity(0);
    check_interval_ms = 60 * 1000;          // 60 sec
    force_update_interval_ms = 600 * 1000;  // 10 min
    humid_diff_threshold = 1;               // 1%
  }

  void loop() {
    if (humidity_kit->timeVal() < check_interval_ms) {
      return;
    }

    sensors_event_t event;
    dht->humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      LOG1("Error reading humidity!\n");
      return;
    }

    if (
      (abs(event.relative_humidity - humidity_kit->getVal<float>()) < humid_diff_threshold) &&
      (humidity_kit->timeVal() < force_update_interval_ms)
    ) {
      return;
    }

    humidity_kit->setVal(event.relative_humidity);
    LOG1("Humidity Update: %f\n", event.relative_humidity);
  }
};
