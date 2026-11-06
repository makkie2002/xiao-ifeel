#pragma once
#include "ir_Electra.h"

// XIAO Smart IR Mate TX = GPIO3
static IRElectraAc ifeel_ac(3);
static bool ifeel_init = false;

inline void ifeel_setup() {
  if (!ifeel_init) {
    ifeel_ac.begin();
    ifeel_init = true;
  }
}

// Full command with IFeel ON (AC beeps, applies mode/temp)
inline void ifeel_on(uint8_t temp) {
  ifeel_setup();
  ifeel_ac.stateReset();
  ifeel_ac.on();
  ifeel_ac.setTemp(temp);
  ifeel_ac.setIFeel(true);
  ifeel_ac.setSensorUpdate(false);
  ifeel_ac.setSensorTemp(temp);
  ifeel_ac.send();
}

// Silent sensor-only update (AC takes temp, no beep)
inline void ifeel_sensor(uint8_t sensor_temp) {
  ifeel_setup();
  ifeel_ac.setIFeel(true);
  ifeel_ac.setSensorUpdate(true);
  ifeel_ac.setSensorTemp(sensor_temp);
  ifeel_ac.send();
}
