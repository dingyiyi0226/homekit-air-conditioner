#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <HomeSpan.h>


struct ACThermostat : Service::Thermostat {
  SpanCharacteristic *currentState;
  SpanCharacteristic *targetState;
  SpanCharacteristic *currentTemperature;
  SpanCharacteristic *targetTemperature;
  SpanCharacteristic *tempUnits;
  SpanCharacteristic *currentHumidity;

  DHT_Unified *dht;

  // temperature/humidity sensor constants
  int check_interval_ms;
  int force_update_interval_ms;
  float temp_diff_threshold;
  float humid_diff_threshold;

  ACThermostat(DHT_Unified *dht) : dht(dht) {
    new Characteristic::ConfiguredName("Thermostat");

    targetState = new Characteristic::TargetHeatingCoolingState();
    currentState = new Characteristic::CurrentHeatingCoolingState();
    targetTemperature = (new Characteristic::TargetTemperature(26));
    currentTemperature = new Characteristic::CurrentTemperature(10);
    currentHumidity = new Characteristic::CurrentRelativeHumidity(10);
    tempUnits = new Characteristic::TemperatureDisplayUnits(Characteristic::TemperatureDisplayUnits::CELSIUS);

    targetState->setValidValues(2, Characteristic::TargetHeatingCoolingState::OFF, Characteristic::TargetHeatingCoolingState::COOL);
    targetTemperature->setRange(20, 28, 1);


    check_interval_ms = 60 * 1000;          // 60 sec
    force_update_interval_ms = 600 * 1000;  // 10 min
    temp_diff_threshold = 0.5;              // 0.5 degree Celsius
    humid_diff_threshold = 1;               // 1%
  }

  bool update() {
    if (targetState->updated()) {
      LOG1("Thermo update targetState from [%d] to [%d]\n", targetState->getVal<uint8_t>(), targetState->getNewVal<uint8_t>());
    }
    if (targetTemperature->updated()) {
      LOG1("Thermo update targetTemperature from [%d] to [%d]\n", targetTemperature->getVal<int>(), targetTemperature->getNewVal<int>());
    }

    return true;
  }

  void loop() {
    checkTemperature();
    checkHumidity();
  }

  /**
   * Check the temperature every 60 seconds.
   * Update HomeKit only if the temperature difference is greater than 0.5 degrees.
   * Force update the temperature every 10 minutes.
   *
   */
  void checkTemperature() {
    if (currentTemperature->timeVal() < check_interval_ms) {
      return;
    }

    sensors_event_t event;
    dht->temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      LOG1("Error reading temperature!\n");
      return;
    }

    if (
      (abs(event.temperature - currentTemperature->getVal<float>()) < temp_diff_threshold) &&
      (currentTemperature->timeVal() < force_update_interval_ms)
    ) {
      return;
    }

    currentTemperature->setVal(event.temperature);
    LOG1("Temperature Update: %f\n", event.temperature);
  }

  /**
   * Check the humidity every 60 seconds.
   * Update HomeKit only if the humidity difference is greater than 1%.
   * Force update the humidity every 10 minutes.
   *
   */
  void checkHumidity() {
    if (currentHumidity->timeVal() < check_interval_ms) {
      return;
    }

    sensors_event_t event;
    dht->humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      LOG1("Error reading humidity!\n");
      return;
    }

    if (
      (abs(event.temperature - currentHumidity->getVal<float>()) < humid_diff_threshold) &&
      (currentHumidity->timeVal() < force_update_interval_ms)
    ) {
      return;
    }

    currentHumidity->setVal(event.relative_humidity);
    LOG1("Humidity Update: %f\n", event.relative_humidity);
  }
};
