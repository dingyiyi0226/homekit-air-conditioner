#include "ac-thermostat.h"

#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <HomeSpan.h>

#include "teco-ac.h"


ACThermostat::ACThermostat(DHT_Unified *dht, TecoAC *tecoAC) : dht(dht), tecoAC(tecoAC) {
  new Characteristic::ConfiguredName("Thermostat");

  targetState = new Characteristic::TargetHeatingCoolingState();
  currentState = new Characteristic::CurrentHeatingCoolingState();
  targetTemperature = new Characteristic::TargetTemperature(26);
  currentTemperature = new Characteristic::CurrentTemperature(10);
  currentHumidity = new Characteristic::CurrentRelativeHumidity(10);
  tempUnits = new Characteristic::TemperatureDisplayUnits(Characteristic::TemperatureDisplayUnits::CELSIUS);

  targetState->setValidValues(2, Characteristic::TargetHeatingCoolingState::OFF, Characteristic::TargetHeatingCoolingState::COOL);
  targetTemperature->setRange(16, 31, 1);


  check_interval_ms = 60 * 1000;          // 60 sec
  force_update_interval_ms = 600 * 1000;  // 10 min
  temp_diff_threshold = 0.5;              // 0.5 degree Celsius
  humid_diff_threshold = 1;               // 1%
}

bool ACThermostat::update() {
  if (targetState->updated() && (targetState->getVal<uint8_t>() != targetState->getNewVal<uint8_t>())) {
    LOG1("Thermo update targetState from [%d] to [%d]\n", targetState->getVal<uint8_t>(), targetState->getNewVal<uint8_t>());
    tecoAC->Power();
  }
  if (targetTemperature->updated() && (targetTemperature->getVal<int>() != targetTemperature->getNewVal<int>())) {
    LOG1("Thermo update targetTemperature from [%d] to [%d]\n", targetTemperature->getVal<int>(), targetTemperature->getNewVal<int>());
    tecoAC->SetTemperature(targetTemperature->getNewVal<int>());

    temp_history.push_back(targetTemperature->getNewVal<int>());
    if (temp_history.size() > 3) {
      temp_history.pop_front();
    }
    checkResetPower();
  }

  return true;
}

/**
 * Reset the power status without sending the signal.
 * Magic temperature sequence: 28, 24, 28
 *
 */
void ACThermostat::checkResetPower() {
  if (temp_history.size() < 3) {
    return;
  }

  int i = 0;
  for (const auto &temp: temp_history) {
    if ((i == 0 && temp != 28) || (i == 1 && temp != 24) || (i == 2 && temp != 28)) {
      return;
    }
    ++i;
  }

  LOG1("Reset power status\n");
  temp_history.clear();
  targetState->setVal(Characteristic::TargetHeatingCoolingState::OFF);
}


void ACThermostat::loop() {
  checkTemperature();
  checkHumidity();
  regulatePowerState();
}

/**
 * Check the temperature every 60 seconds.
 * Update HomeKit only if the temperature difference is greater than 0.5 degrees.
 * Force update the temperature every 10 minutes.
 *
 */
void ACThermostat::checkTemperature() {
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
void ACThermostat::checkHumidity() {
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
    (abs(event.relative_humidity - currentHumidity->getVal<float>()) < humid_diff_threshold) &&
    (currentHumidity->timeVal() < force_update_interval_ms)
  ) {
    return;
  }

  currentHumidity->setVal(event.relative_humidity);
  LOG1("Humidity Update: %f\n", event.relative_humidity);
}

/**
 * Regulate power state to cooling state.
 * Although we cannot change to other states via the Home app UI,
 * it is possible to set the state via Siri.
 *
 * Note: we cannot update the state in the update() function, so check it in the loop.
 *
 */
void ACThermostat::regulatePowerState() {
  if (
    (targetState->getVal<uint8_t>() == Characteristic::TargetHeatingCoolingState::HEAT) ||
    (targetState->getVal<uint8_t>() == Characteristic::TargetHeatingCoolingState::AUTO)
  ) {
    LOG1("Regulate power state from [%d] to [%d]\n", targetState->getVal<uint8_t>(), Characteristic::TargetHeatingCoolingState::COOL);
    targetState->setVal(Characteristic::TargetHeatingCoolingState::COOL);
  }
}
