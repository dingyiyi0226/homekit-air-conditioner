#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <HomeSpan.h>

#include "humidity-sensor.h"
#include "temperature-sensor.h"

#define DHTPIN 26
#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Temp DHT22");
    new TemperatureSensor(&dht);

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Humid DHT22");
    new HumiditySensor(&dht);
}

void loop(){
  homeSpan.poll();
  delay(1000);
}
