## Fixed Frequency

A lightweight module to perform operations in loop at
specific frequency. It helps when you want to process
different components with different frequency in the main
loop

## Examples

```c++
#include <FrequencyLimiter.h>

FrequencyLimiter networkFrequency(Frequency(10));  // Process network at 10Hz
FrequencyLimiter readSensorsFrequency(PeriodMs(500));  // Read new sensor with a period of 500ms

void loop() {

    if (networkFrequency.process()){
        // Process network stuff
    }
    
    if (readSensorsFrequency.process()) {
        // Read sensors.
    }
    
};
```