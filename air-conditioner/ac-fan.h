#include <HomeSpan.h>


struct ACFan : Service::Fan {
  ACFan() {
    new Characteristic::ConfiguredName("Fan");
    active = new Characteristic::Active(Characteristic::Active::INACTIVE);
    fanType = new Characteristic::RotationSpeed(0);
    fanType->setRange(0, 3, 1);
  }

  bool update() override {
    if (active->updated()) {
      LOG1("Fan update active [%d]\n", active->getNewVal<uint8_t>());
    }
    if (fanType->updated()) {
      LOG1("Fan update fanType from [%d] to [%d]\n",  fanType->getVal<int>(), fanType->getNewVal<int>());
    }

    return true;
  }

  void loop() override {}

  SpanCharacteristic *active;
  SpanCharacteristic *currentState;
  SpanCharacteristic *targetState;
  SpanCharacteristic *fanType;
};
