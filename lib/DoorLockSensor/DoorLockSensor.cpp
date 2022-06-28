//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#include "DoorLockSensor.h"

DoorLockSensor::DoorLockSensor(uint16_t threshold, float moving_average_ratio) :
        distance_sensor(),
        threshold(threshold),
        moving_average_ratio(moving_average_ratio) {
    last_measured_time = millis();
    last_state_change_time = millis();
}

void DoorLockSensor::update() {

    distance_sensor.rangingTest(&measure);
    if (measure.RangeStatus != 4) {
        unsigned long current_time = millis();
        unsigned long passed_time = current_time - last_measured_time;
        last_measured_time = current_time;

        // Calculate effective moving ratio, by normalising ratio on time to avoid moving faster
        // when sampling is higher or lower.
        float effective_moving_ratio = min(moving_average_ratio * (passed_time / 1000.0), 1.0);

        last_measured_distance = (effective_moving_ratio * measure.RangeMilliMeter)
                                 + ((1 - effective_moving_ratio) * last_measured_distance);
    }

    // Calculate new status
    bool _is_locked_new =  (get_raw_distance() < threshold);

    if (_is_locked_new != _is_locked){
        // State change, lets mark time
        last_state_change_time = millis();
    }
    _is_locked = _is_locked_new;
}


bool DoorLockSensor::begin() {
    return distance_sensor.begin();
}
