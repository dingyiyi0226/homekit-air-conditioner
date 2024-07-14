#include <HomeSpan.h>

#include "teco-ac.h"


struct ACFan : Service::Fan {
  ACFan(TecoAC *tecoAC) : tecoAC(tecoAC) {
    new Characteristic::ConfiguredName("Fan");
    active = new Characteristic::Active(Characteristic::Active::INACTIVE);
    fanType = new Characteristic::RotationSpeed(0);
    fanType->setRange(0, 3, 1);
  }

  bool update() override {
    if (fanType->updated() && (fanType->getVal<int>() != fanType->getNewVal<int>())) {
      LOG1("Fan update fanType from [%d] to [%d]\n",  fanType->getVal<int>(), fanType->getNewVal<int>());
      tecoAC->SetFan(TecoAC::FanType(fanType->getNewVal<int>()));
    }

    return true;
  }

  void loop() override {}

  SpanCharacteristic *active;
  SpanCharacteristic *currentState;
  SpanCharacteristic *targetState;
  SpanCharacteristic *fanType;

  TecoAC *tecoAC;
};
