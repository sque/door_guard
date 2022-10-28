//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#ifndef DOOR_GUARD_GHOSTANIMATION_H
#define DOOR_GUARD_GHOSTANIMATION_H

#include <Arduino.h>
#include <vector>
#include <Adafruit_GFX.h>
#include <Animation.h>

class GhostAnimation : public Animation {
protected:
    Bitmap * bitmaps;
public:
    GhostAnimation(Adafruit_GFX &display, uint8_t x, uint8_t y);

    virtual ~GhostAnimation();
};


#endif //DOOR_GUARD_GHOSTANIMATION_H
