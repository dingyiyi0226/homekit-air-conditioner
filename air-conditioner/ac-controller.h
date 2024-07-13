#include <DHT_U.h>
#include <HomeSpan.h>

#include "ac-fan.h"
#include "ac-swing.h"
#include "ac-thermostat.h"


struct ACController {
  ACController(DHT_Unified *dht) {
    new ACThermostat(dht);
    new ACFan();
    new ACSwing();
  }
};
