//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "YodaAnimation.h"

#define FRAME_WIDTH 38
#define FRAME_HEIGHT 40

#define WHITE 1

// 'frame_2', 38x40px
const uint8_t frame_2[] PROGMEM = {
        0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x40, 0x00, 0x00, 0x20, 0x00, 0x20, 0x0c, 0xf8, 0x40, 0x00, 0x00, 0xf4, 0x87, 0x80,
        0x00, 0x1f, 0x04, 0x43, 0x00, 0x00, 0x20, 0x08, 0x02, 0x00, 0x00, 0x20, 0x00, 0x14, 0x00, 0x00,
        0x00, 0x20, 0x0c, 0x00, 0x00, 0x00, 0x40, 0x05, 0xe0, 0x78, 0x01, 0x80, 0x04, 0x00, 0x00, 0x0e,
        0x00, 0x04, 0x00, 0x00, 0x3c, 0x00, 0x04, 0x00, 0x00, 0x38, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00,
        0x02, 0x1f, 0x80, 0x10, 0x00, 0x01, 0x80, 0x00, 0x30, 0x00, 0x00, 0x40, 0x00, 0x48, 0x00, 0x00,
        0xb0, 0x00, 0x04, 0x00, 0x01, 0xb0, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00,
        0x00, 0x41, 0x00, 0x02, 0x4f, 0x80, 0x41, 0x00, 0x02, 0x70, 0x78, 0x41, 0x00, 0x02, 0x60, 0x67,
        0x80, 0x80, 0x02, 0x26, 0x47, 0x00, 0x80, 0x04, 0x3e, 0xc0, 0x00, 0x80, 0x04, 0x23, 0x80, 0x00,
        0x80, 0x04, 0x3c, 0x80, 0x00, 0x80, 0x04, 0x20, 0x80, 0x01, 0x80, 0x04, 0x20, 0x80, 0x01, 0x80,
        0x04, 0x20, 0x80, 0x04, 0x80, 0x04, 0x10, 0x40, 0x18, 0x80, 0x02, 0x10, 0x40, 0x60, 0x80, 0x02,
        0x08, 0x07, 0x80, 0x80, 0x02, 0x0c, 0x18, 0x00, 0x80, 0x02, 0x04, 0x10, 0x00, 0x80, 0x02, 0x00,
        0x00, 0x00, 0x80, 0x01, 0xff, 0xff, 0xff, 0x00
};
// 'frame_1', 38x40px
const uint8_t frame_1[] PROGMEM = {
        0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x40, 0x00, 0x00, 0x20, 0x00, 0x20, 0x0c, 0xf8, 0x40, 0x00, 0x00, 0xf4, 0x87, 0x80,
        0x00, 0x1f, 0x04, 0x43, 0x00, 0x00, 0x20, 0x08, 0x02, 0x00, 0x00, 0x20, 0x00, 0x14, 0x00, 0x00,
        0x00, 0x20, 0x0d, 0xe0, 0x78, 0x00, 0x40, 0x05, 0xe0, 0x78, 0x01, 0x80, 0x04, 0x00, 0x00, 0x0e,
        0x00, 0x04, 0x00, 0x00, 0x3c, 0x00, 0x04, 0x00, 0x00, 0x38, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00,
        0x02, 0x1f, 0x80, 0x10, 0x00, 0x01, 0x80, 0x00, 0x30, 0x00, 0x00, 0x40, 0x00, 0x48, 0x00, 0x00,
        0xb0, 0x00, 0x04, 0x00, 0x01, 0xb0, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00,
        0x00, 0x41, 0x00, 0x02, 0x4f, 0x80, 0x41, 0x00, 0x02, 0x70, 0x78, 0x41, 0x00, 0x02, 0x60, 0x67,
        0x80, 0x80, 0x02, 0x26, 0x47, 0x00, 0x80, 0x04, 0x3e, 0xc0, 0x00, 0x80, 0x04, 0x23, 0x80, 0x00,
        0x80, 0x04, 0x3c, 0x80, 0x00, 0x80, 0x04, 0x20, 0x80, 0x01, 0x80, 0x04, 0x20, 0x80, 0x01, 0x80,
        0x04, 0x20, 0x80, 0x04, 0x80, 0x04, 0x10, 0x40, 0x18, 0x80, 0x02, 0x10, 0x40, 0x60, 0x80, 0x02,
        0x08, 0x07, 0x80, 0x80, 0x02, 0x0c, 0x18, 0x00, 0x80, 0x02, 0x04, 0x10, 0x00, 0x80, 0x02, 0x00,
        0x00, 0x00, 0x80, 0x01, 0xff, 0xff, 0xff, 0x00
};



YodaAnimation::YodaAnimation(Adafruit_GFX &display, uint8_t x, uint8_t y):
        Animation(display, x, y, FRAME_WIDTH, FRAME_HEIGHT){

    bitmaps = new Bitmap[2]{
            Bitmap(frame_1, FRAME_WIDTH, FRAME_HEIGHT),
            Bitmap(frame_2, FRAME_WIDTH, FRAME_HEIGHT)
    };

    frames.push_back(Frame(bitmaps[0], 5000));
    frames.push_back(Frame(bitmaps[1], 500));
    frames.push_back(Frame(bitmaps[0], 500));
    frames.push_back(Frame(bitmaps[1], 500));

}

YodaAnimation::~YodaAnimation() {

}

