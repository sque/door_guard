//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#ifndef DOOR_GUARD_YODAANIMATION_H
#define DOOR_GUARD_YODAANIMATION_H

#include <Arduino.h>
#include <vector>
#include <Adafruit_GFX.h>
#include <Animation.h>

class YodaAnimation : public Animation {
public:
    YodaAnimation(Adafruit_GFX &display, uint8_t x, uint8_t y);

    virtual ~YodaAnimation();
};


#endif //DOOR_GUARD_YODAANIMATION_H
