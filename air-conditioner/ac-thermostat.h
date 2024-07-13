#include <HomeSpan.h>


struct ACThermostat : Service::Thermostat {
  SpanCharacteristic *currentState;
  SpanCharacteristic *targetState;
  SpanCharacteristic *currentTemperature;
  SpanCharacteristic *targetTemperature;
  SpanCharacteristic *tempUnits;
  SpanCharacteristic *currentHumidity;

  ACThermostat() {
    new Characteristic::ConfiguredName("Thermostat");

    targetState = new Characteristic::TargetHeatingCoolingState();
    currentState = new Characteristic::CurrentHeatingCoolingState();
    targetTemperature = (new Characteristic::TargetTemperature(26));
    currentTemperature = new Characteristic::CurrentTemperature(10);
    currentHumidity = new Characteristic::CurrentRelativeHumidity(10);
    tempUnits = new Characteristic::TemperatureDisplayUnits(Characteristic::TemperatureDisplayUnits::CELSIUS);

    targetState->setValidValues(2, Characteristic::TargetHeatingCoolingState::OFF, Characteristic::TargetHeatingCoolingState::COOL);
    targetTemperature->setRange(20, 28, 1);
  }

  bool update() {
    if (currentState->updated()) {
      LOG1("Thermo update currentState [%d]\n", currentState->getVal<uint8_t>());
    }
    if (targetState->updated()) {
      LOG1("Thermo update targetState from [%d] to [%d]\n", targetState->getVal<uint8_t>(), targetState->getNewVal<uint8_t>());
    }
    if (currentTemperature->updated()) {
      LOG1("Thermo update currentTemperature\n");
    }
    if (targetTemperature->updated()) {
      LOG1("Thermo update targetTemperature from [%d] to [%d]\n", targetTemperature->getVal<int>(), targetTemperature->getNewVal<int>());
    }
    if (currentHumidity->updated()) {
      LOG1("Thermo update currentHumidity\n");
    }

    return true;
  }

  void loop() {}
};
