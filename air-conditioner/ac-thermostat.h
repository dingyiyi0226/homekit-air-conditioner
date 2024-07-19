#include <list>

#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <HomeSpan.h>

#include "teco-ac.h"


struct ACThermostat : Service::Thermostat {
  ACThermostat(DHT_Unified *dht, TecoAC *tecoAC);

  bool update() override;
  void loop() override;

  void checkTemperature();
  void checkHumidity();
  void checkResetPower();


  SpanCharacteristic *currentState;
  SpanCharacteristic *targetState;
  SpanCharacteristic *currentTemperature;
  SpanCharacteristic *targetTemperature;
  SpanCharacteristic *tempUnits;
  SpanCharacteristic *currentHumidity;

  DHT_Unified *dht;
  TecoAC *tecoAC;

  // temperature/humidity sensor constants
  int check_interval_ms;
  int force_update_interval_ms;
  float temp_diff_threshold;
  float humid_diff_threshold;

  std::list<int> temp_history;
};
