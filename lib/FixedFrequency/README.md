## Fixed Frequency

A lightweight module to perform operations in loop at
specific frequency. It helps when you want to process
different components with different frequency in the main
loop

## Examples

```c++
#include <FixedFrequency.h>

FixedFrequency networkFrequency(10);  // Process network at 10Hz
FixedFrequency readSensorsFrequency(2);  // Read new sensor values 2 times per second

void loop() {

    if (networkFrequency.process()){
        // Process network stuff
    }
    
    if (readSensorsFrequency.process()) {
        // Read sensors.
    }
    
}

```