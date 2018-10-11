#ifndef GENERAL_INO
#define GENERAL_INO

#define __SAM3X8E__
#include "chip.h"
#include <Arduino.h>

void pause() {
    while (!Serial.available()) { }
    Serial.read();
}

#endif //GENERAL_INO