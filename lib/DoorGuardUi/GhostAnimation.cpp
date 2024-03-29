//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "GhostAnimation.h"

#define FRAME_WIDTH 38
#define FRAME_HEIGHT 40

#define WHITE 1

// 'frame_1', 40x39px
const unsigned char frame_1 [] PROGMEM = {
        0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x01, 0x80, 0x70, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x08, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x08,
        0x00, 0x02, 0x00, 0x00, 0x11, 0x08, 0x02, 0x00, 0x00, 0x33, 0x0c, 0x01, 0x00, 0x00, 0x33, 0x0c,
        0x01, 0x00, 0x00, 0x33, 0x0e, 0x01, 0x00, 0x00, 0x33, 0x0f, 0x01, 0x00, 0x03, 0xf0, 0x00, 0x01,
        0x00, 0x0c, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x00, 0x03, 0x80, 0x80, 0x20, 0x00, 0x04, 0x00, 0x80,
        0x20, 0x00, 0x04, 0x10, 0x80, 0x40, 0x00, 0x08, 0x08, 0x80, 0x40, 0x00, 0x10, 0x04, 0x80, 0x40,
        0x00, 0x00, 0x04, 0x80, 0x3f, 0x00, 0x1c, 0x00, 0x40, 0x18, 0x80, 0x1a, 0x00, 0x40, 0x00, 0x80,
        0x0a, 0x00, 0x40, 0x04, 0x80, 0x0a, 0x00, 0x60, 0x03, 0x80, 0x05, 0x00, 0x20, 0x00, 0x80, 0x04,
        0x80, 0x10, 0x00, 0x80, 0x03, 0x80, 0x10, 0x00, 0x80, 0x00, 0x80, 0x10, 0x00, 0x80, 0x00, 0x00,
        0x10, 0x00, 0x80, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x10, 0x00, 0x40, 0x00, 0x00, 0x08,
        0x00, 0x46, 0x00, 0x00, 0x08, 0x00, 0x60, 0x00, 0x00, 0x08, 0x00, 0x60, 0x00, 0x00, 0x04, 0x00,
        0x30, 0x08, 0x00, 0x02, 0x00, 0x3f, 0xff, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00,
        0x00, 0x00, 0x00
};


// 'frame_2', 40x39px
const unsigned char frame_2 [] PROGMEM = {
        0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x01, 0x80, 0x70, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x08, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x08,
        0x00, 0x02, 0x00, 0x00, 0x11, 0x84, 0x02, 0x00, 0x00, 0x31, 0x86, 0x01, 0x00, 0x00, 0x31, 0x86,
        0x01, 0x00, 0x00, 0x31, 0x06, 0x01, 0x00, 0x1f, 0xf3, 0x07, 0x01, 0x00, 0x20, 0x30, 0x00, 0x01,
        0x00, 0x00, 0x00, 0xe0, 0x00, 0x80, 0x40, 0x00, 0x02, 0x00, 0x80, 0x40, 0x00, 0x05, 0x00, 0x80,
        0x40, 0x00, 0x18, 0xc0, 0x80, 0x40, 0x00, 0x20, 0x20, 0x80, 0x78, 0x00, 0x30, 0x10, 0x80, 0x07,
        0x80, 0x20, 0x04, 0x80, 0x01, 0x00, 0x20, 0x00, 0x70, 0x00, 0x80, 0x30, 0x00, 0x60, 0x00, 0x80,
        0x20, 0x00, 0x60, 0x00, 0x80, 0x18, 0x00, 0x60, 0x00, 0x80, 0x0c, 0x00, 0x20, 0x00, 0x80, 0x0c,
        0x80, 0x10, 0x00, 0x80, 0x03, 0x80, 0x10, 0x00, 0x80, 0x00, 0x80, 0x10, 0x00, 0x80, 0x00, 0x00,
        0x10, 0x00, 0x80, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x10, 0x00, 0x40, 0x00, 0x00, 0x08,
        0x00, 0x40, 0x00, 0x00, 0x08, 0x00, 0x40, 0x00, 0x00, 0x08, 0x00, 0x70, 0x00, 0x00, 0x04, 0x00,
        0x3f, 0x80, 0x00, 0x02, 0x00, 0x1f, 0xff, 0x0f, 0xc2, 0x00, 0x00, 0x08, 0xf8, 0x1e, 0x00, 0x00,
        0x00, 0x00, 0x00
};




GhostAnimation::GhostAnimation(Adafruit_GFX &display, uint8_t x, uint8_t y):
        Animation(display, x, y, FRAME_WIDTH, FRAME_HEIGHT){

    bitmaps = new Bitmap[2]{
            Bitmap(frame_1, FRAME_WIDTH, FRAME_HEIGHT),
            Bitmap(frame_2, FRAME_WIDTH, FRAME_HEIGHT)
    };

    frames.push_back(Frame(bitmaps[0], 500));
    frames.push_back(Frame(bitmaps[1], 500));
    frames.push_back(Frame(bitmaps[0], 500));
    frames.push_back(Frame(bitmaps[1], 3000));

}

GhostAnimation::~GhostAnimation() {

}

