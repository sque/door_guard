//
// Created by Konstantinos Paliouras on 18/06/2022.
//

#ifndef DOOR_GUARD_ANIMATION_H
#define DOOR_GUARD_ANIMATION_H

#include <Widget.h>
#include <Arduino.h>
#include <vector>
#include <Adafruit_GFX.h>

/**
 * Handler of a bitmap in memory
 */
class Bitmap {
protected:
    const uint8_t *bitmap;
    uint16_t width;
    uint16_t height;
public:
    /**
     * Construct a new handler from in memory data
     * @param bitmap A pointer to memory data
     * @param width The width of the bitmap in pixels
     * @param height The height of bitmap in pixels
     */
    Bitmap(const uint8_t *bitmap, uint16_t width, uint16_t height);


    /**
     * Draw bitmap on display
     * @param display The target display to render image
     * @param x The x of the top left position to render the bitmap
     * @param y The y of the top left position to render the bitmap
     */
    void draw(Adafruit_GFX &display, uint16_t x, uint16_t y) const;
};

/**
 * Holder of frame information. Every frame is a combination of a bitmap and duration.
 */
class Frame {
public:
    /**
     * Reference to bitmap image
     */
    const Bitmap &bitmap;

    /**
     * The frame duration in milliseconds
     */
    const unsigned long duration_ms;

    /**
     * Construct a new frame
     * @param bitmap The referenced image
     * @param duration_ms The duration in milliseconds
     */
    Frame(const Bitmap &bitmap, unsigned long duration_ms) :
            bitmap(bitmap),
            duration_ms(duration_ms) {}

};

typedef std::vector<Frame> frames_t;

/**
 * A generic animation class to play frames of bitmaps
 */
class Animation : public Widget {
protected:
    frames_t frames;
    frames_t::const_iterator current_frame;

    unsigned long next_frame_ms = 0;

    bool loop_mode;
    bool is_playing;

    void on_draw();

public:
    /**
     * Initialise the animation
     * @param display The output display to draw the animation
     * @param x The top left position of the rendered animation on the display
     * @param y The top left position of the rendered animation on the display
     * @param width The width of the animation
     * @param height The height of the animation.
     * @param frames
     * @param total_frames
     */
    Animation(Adafruit_GFX &display, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    /**
     * Stop animation from playing.
     */
    void stop() {
        is_playing = false;
    }

    /**
     * Enable or disable the looping mode in animation. When enable the animation will play for ever.
     * @param enabled
     */
    void set_loop_mode(bool enabled) {
        loop_mode = enabled;
    }

    /**
     * Start playing the animation from the begining.
     */
    void start() {
        is_playing = true;
        current_frame = frames.cbegin();
        next_frame_ms = millis() + current_frame->duration_ms;
    }
};


#endif //DOOR_GUARD_ANIMATION_H
