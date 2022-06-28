//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#ifndef DOOR_GUARD_FIXEDFREQUENCY_H
#define DOOR_GUARD_FIXEDFREQUENCY_H

#define MAX_LIMITED_PERIOD (30 * 24 * 3600 * 1000)

#include <Arduino.h>

typedef unsigned long millis_t;

// Prototypes
class Frequency;
class Period;

/**
 * Period defined in Milliseconds
 */
class PeriodMs {
public:
    millis_t value;

    explicit PeriodMs(millis_t _value);

    /**
     * Convert defined period in frequency
     * @return
     */
    Frequency to_frequency() const;

    /**
     * Cast to native type
     * @return
     */
    operator millis_t () {
        return value;
    }
};

/**
 * Frequency defined in HZ
 */
class Frequency {
public:
    float value;

    explicit Frequency(float _value);

    /**
     * Convert frequency to period per MS
     * @return
     */
    PeriodMs to_period_ms() const;

    /**
     * Cast to native type
     * @return
     */
    operator float () {
        return value;
    }
};

/**
 * A class to enforce a specific frequency of processing
 * The period cannot be more than 30 days.
 */
class FrequencyLimiter {
protected:
    PeriodMs period;
    millis_t next_process_time_ms;

public:
    /**
     * Construct a new frequency limiter object based on frequency value
     * @param frequency The frequency defined as frequency object
     */
    FrequencyLimiter(const Frequency & frequency);


    /**
     * Construct a new frequency limiter object based on period value
     * @param frequency The frequency defined as period object
     */
    FrequencyLimiter(const PeriodMs & period);

    /**
     * Process another loop.
     * @return Return true if this is time to process
     */
    bool process();
};


#endif //DOOR_GUARD_FIXEDFREQUENCY_H
