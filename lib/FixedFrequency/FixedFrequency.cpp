//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#include "FixedFrequency.h"


FixedFrequency::FixedFrequency(float frequency):
frequency(frequency),
period_ms(1000/frequency),
next_process_time_ms(0){
    next_process_time_ms = 0;
}

bool FixedFrequency::process()
{
    unsigned long current_time_ms = millis();

    if (current_time_ms > next_process_time_ms) {
        // TODO: Add a guard that when millis rotate this still works.
        next_process_time_ms += period_ms;
        return true;
    } else {
        return false;
    }

}

FixedFrequency FixedFrequency::from_period(float period_ms) {

    return FixedFrequency(1000.0 / period_ms);
}
