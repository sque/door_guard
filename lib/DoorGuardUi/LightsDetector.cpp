//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#include "LightsDetector.h"

LightsDetector::LightsDetector(int threshold, uint8_t pin) :
        pin(pin),
        threshold(threshold){
}

void LightsDetector::update() {
    last_value = analogRead(pin);
}
