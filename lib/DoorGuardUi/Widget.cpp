//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#include "Widget.h"

Widget::Widget(Adafruit_GFX &display, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
        display(display),
        x(x),
        y(y),
        width(width),
        height(height) {
}

void Widget::draw() {
    on_draw();
}
