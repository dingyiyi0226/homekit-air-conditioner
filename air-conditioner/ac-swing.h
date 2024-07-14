#include <HomeSpan.h>

#include "teco-ac.h"

struct ACSwing : Service::Fan {
  ACSwing(TecoAC *tecoAC) : tecoAC(tecoAC) {
    new Characteristic::ConfiguredName("Swing");
    active = new Characteristic::Active(Characteristic::Active::INACTIVE);
    swingType = new Characteristic::RotationSpeed(0);
    swingType->setRange(0, 5, 1);
  }

  bool update() override {
    if (swingType->updated()) {
      LOG1("Swing update swingType from [%d] to [%d]\n",  swingType->getVal<int>(), swingType->getNewVal<int>());
      tecoAC->SetSwing(TecoAC::SwingType(swingType->getNewVal<int>()));
      tecoAC->Send();
    }

    return true;
  }

  void loop() override {}


  SpanCharacteristic *active;
  SpanCharacteristic *currentState;
  SpanCharacteristic *targetState;
  SpanCharacteristic *swingType;

  TecoAC *tecoAC;
};
