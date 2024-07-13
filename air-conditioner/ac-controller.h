#include <HomeSpan.h>

#include "ac-fan.h"
#include "ac-swing.h"
#include "ac-thermostat.h"


struct ACController {
  ACController() {
    new ACThermostat();
    new ACFan();
    new ACSwing();
  }
};
