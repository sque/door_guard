//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_LIGHTSDETECTOR_H
#define DOOR_GUARD_LIGHTSDETECTOR_H

#include <Arduino.h>

class LightsDetector {
protected:
    uint8_t pin;
    int last_value;
    int threshold;
public:
    LightsDetector(int threshold, uint8_t pin = A0);

    int get_raw_value() {
        return last_value;
    }

    bool is_turned_on() {
        return last_value > threshold;
    }

    /**
     * Update the readings
     */
    void update();
};


#endif //DOOR_GUARD_LIGHTSDETECTOR_H
