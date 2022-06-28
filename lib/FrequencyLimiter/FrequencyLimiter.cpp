//
// Created by Konstantinos Paliouras on 18/06/2022.
//
#include "FrequencyLimiter.h"


Frequency::Frequency(float _value) :
        value(_value) {}


Frequency PeriodMs::to_frequency() const {

    return Frequency(1000.0 / value);
}

PeriodMs::PeriodMs(millis_t _value) :
        value(_value) {}

PeriodMs Frequency::to_period_ms() const {
    return PeriodMs(1000.0 / value);
}


FrequencyLimiter::FrequencyLimiter(const Frequency &frequency) :
        period(frequency.to_period_ms()),
        next_process_time_ms(0) {
    next_process_time_ms = 0;
}


FrequencyLimiter::FrequencyLimiter(const PeriodMs &_period) :
        period(_period),
        next_process_time_ms(0) {
    next_process_time_ms = 0;
}

bool FrequencyLimiter::process() {
    millis_t current_time_ms = millis();

    if ((current_time_ms > next_process_time_ms)
        || ((next_process_time_ms - current_time_ms) > MAX_LIMITED_PERIOD)) {
        next_process_time_ms += period.value;
        return true;
    } else {
        return false;
    }

}
