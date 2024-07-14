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
}

void loop(){
  homeSpan.poll();
  delay(1000);
}
