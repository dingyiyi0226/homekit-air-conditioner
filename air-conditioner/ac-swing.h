#ifndef _AC_SWING_H_
#define _AC_SWING_H_

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
    if (swingType->updated() && (swingType->getVal<int>() != swingType->getNewVal<int>())) {
      LOG1("Swing update swingType from [%d] to [%d]\n",  swingType->getVal<int>(), swingType->getNewVal<int>());
      tecoAC->SetSwing(TecoAC::SwingType(swingType->getNewVal<int>()));
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

#endif  // _AC_SWING_H_
