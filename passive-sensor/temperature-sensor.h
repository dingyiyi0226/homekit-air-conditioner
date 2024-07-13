#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <HomeSpan.h>


/**
 * Check the temperature every 60 seconds.
 * Update HomeKit only if the temperature difference is greater than 0.5 degrees.
 * Force update the temperature every 10 minutes.
 *
 */
struct TemperatureSensor : Service::TemperatureSensor {
  SpanCharacteristic *temperature_kit;
  DHT_Unified *dht;
  int check_interval_ms;
  int force_update_interval_ms;
  float temp_diff_threshold;

  TemperatureSensor(DHT_Unified *dht) : Service::TemperatureSensor(), dht(dht) {
    temperature_kit = new Characteristic::CurrentTemperature(0);
    temperature_kit->setRange(-50, 100);    // Default 0-100
    check_interval_ms = 60 * 1000;          // 60 sec
    force_update_interval_ms = 600 * 1000;  // 10 min
    temp_diff_threshold = 0.5;              // 0.5 degree Celsius
  }

  void loop() {
    if (temperature_kit->timeVal() < check_interval_ms) {
      return;
    }

    sensors_event_t event;
    dht->temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      LOG1("Error reading temperature!\n");
      return;
    }

    if (
      (abs(event.temperature - temperature_kit->getVal<float>()) < temp_diff_threshold) &&
      (temperature_kit->timeVal() < force_update_interval_ms)
    ) {
      return;
    }

    temperature_kit->setVal(event.temperature);
    LOG1("Temperature Update: %f\n", event.temperature);
  }
};
