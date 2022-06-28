//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_DOOR_LOCK_SENSOR_H
#define DOOR_GUARD_DOOR_LOCK_SENSOR_H

#include <Adafruit_VL53L0X.h>

/**
 * Handler for the distance sensor for door lock.
 */
class DoorLockSensor {
private:
    Adafruit_VL53L0X distance_sensor;
    VL53L0X_RangingMeasurementData_t measure;
    uint16_t last_measured_distance;
    unsigned long last_measured_time;
    unsigned long last_state_change_time;
    float moving_average_ratio;
    uint16_t threshold;
    bool _is_locked;

public:
    /**
     * Initialise door lock
     * @param threshold The threshold value that the lock is considered locked.
     * @param moving_average_ratio The moving average ratio in a second.
     */
    DoorLockSensor(uint16_t threshold, float moving_average_ratio = 0.95);

    /**
     * Initialise and fire up sensor
     * @return True if initialisation went well.
     */
    bool begin();

    /**
     * Check if the lock is locked
     * @return
     */
    bool is_locked() const {
        return _is_locked;
    }

    /**
     * Get total time that the lock has not changed state.
     */
     unsigned long state_unchanged_time_ms() const {
         return millis() - last_state_change_time;
     }

    /**
     * Get the distance measurement.
     * @return
     */
    uint16_t get_raw_distance() const {
        return last_measured_distance;
    }

    /**
     * Update readings from sensor
     */
    void update();
};


#endif //DOOR_GUARD_DOOR_LOCK_SENSOR_H
