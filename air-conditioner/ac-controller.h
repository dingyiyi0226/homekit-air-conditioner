#include <DHT_U.h>
#include <HomeSpan.h>
#include <IRsend.h>

#include "ac-fan.h"
#include "ac-swing.h"
#include "ac-thermostat.h"
#include "teco-ac.h"


struct ACController {
  ACController(DHT_Unified *dht, IRsend *ir) : tecoAC(ir) {
    new ACThermostat(dht, &tecoAC);
    new ACFan(&tecoAC);
    new ACSwing(&tecoAC);
  }

  TecoAC tecoAC;
};
