//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_WIDGET_H
#define DOOR_GUARD_WIDGET_H

#include <Adafruit_GFX.h>

/**
 * Abstract definition of a UI widget. A widget is placeable and drawable.
 */
class Widget {
protected:
    Adafruit_GFX &display;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;

    virtual void on_draw() = 0;

public:
    Widget(Adafruit_GFX &display, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    void draw();

};


#endif //DOOR_GUARD_WIDGET_H
