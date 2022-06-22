//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Animation.h"

#define FRAME_WIDTH 38
#define FRAME_HEIGHT 40

#define WHITE 1

Animation::Animation(Adafruit_GFX &display, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
        Widget(display, x, y, width, height) {
}


void Animation::on_draw() {
    unsigned long current_time_ms = millis();

    if (current_time_ms > next_frame_ms) {
        // Advance to next frame
        current_frame ++;
        if (current_frame == frames.cend()) {
            stop();
            if (loop_mode) {
                // Restart
                start();
            };
        }
        // If it is playing project the next stop time.
        if (is_playing) {
            next_frame_ms = next_frame_ms + current_frame->duration_ms;
        }
    }

    // Draw current bitmap
    if (is_playing) {
        current_frame->bitmap.draw(display, x, y);
    }

}


Bitmap::Bitmap(const uint8_t *bitmap, uint16_t width, uint16_t height) :
        bitmap(bitmap),
        width(width),
        height(height) {

}

void Bitmap::draw(Adafruit_GFX &display, uint16_t x, uint16_t y) const{
    display.drawBitmap(x, y, bitmap, width, height, WHITE);
}
