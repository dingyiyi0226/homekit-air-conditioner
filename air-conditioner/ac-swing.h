#include <HomeSpan.h>


struct ACSwing : Service::Fan {
  SpanCharacteristic *active;
  SpanCharacteristic *currentState;
  SpanCharacteristic *targetState;
  SpanCharacteristic *swingType;

  ACSwing() {
    new Characteristic::ConfiguredName("Swing");
    active = new Characteristic::Active(Characteristic::Active::INACTIVE);
    swingType = new Characteristic::RotationSpeed(0);
    swingType->setRange(0, 5, 1);
  }

  bool update() {
    if (active->updated()) {
      LOG1("Swing update active [%d]\n", active->getNewVal<uint8_t>());
    }
    if (swingType->updated()) {
      LOG1("Swing update swingType from [%d] to [%d]\n",  swingType->getVal<int>(), swingType->getNewVal<int>());
    }

    return true;
  }

  void loop() {}
};
