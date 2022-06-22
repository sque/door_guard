//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#ifndef DOOR_GUARD_FIXEDFREQUENCY_H
#define DOOR_GUARD_FIXEDFREQUENCY_H

#include <Arduino.h>

/**
 * A class to enforce a specific frequency of processing
 */
class FixedFrequency {
protected:
    float frequency;
    uint16_t period_ms;
    unsigned long next_process_time_ms;

public:
    /**
     * Construct a new frequency object
     * @param frequency
     */
    FixedFrequency(float frequency);


    /**
     * Constructor to create from period instead of prequency
     * @param period
     * @return
     */
    static FixedFrequency from_period(float period_ms);

    /**
     * Process another loop.
     * @return Return true if this is time to process
     */
    bool process();
};


#endif //DOOR_GUARD_FIXEDFREQUENCY_H
