#include <HomeSpan.h>

#include "ac-controller.h"


void setup() {
  Serial.begin(115200);

  homeSpan.begin(Category::AirConditioners,"HomeSpan AirConditioner");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Air Conditioner");
    new ACController();
}

void loop(){
  homeSpan.poll();
  delay(1000);
}
