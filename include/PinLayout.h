//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#ifndef DOOR_GUARD_PIN_LAYOUT_H
#define DOOR_GUARD_PIN_LAYOUT_H

#if defined DOOR_GUARD_1F
#define DOOR_GUARD_HW_VERSION 11
#elif defined DOOR_GUARD_1G
#define DOOR_GUARD_HW_VERSION 10
#else
#error "You need to define DOOR_GUARD_1G or DOOR_GUARD_1F"
#endif

// Default configuration
#define INTERNAL_BUZZER_PIN D0
#define EXTERNAL_BUZZER_PIN D5

#define LOCKER_LED_PIN D7
#define HAPPY_FACE_LED_PIN D6

#define DEBUG_BUTTON_PIN D8

// Hardware specific configuration
#if DOOR_GUARD_HW_VERSION == 10
    #define SCREEN_WIDTH 64
    #define SCREEN_HEIGHT 48
    #define SCREEN_WEMOS_OLED true
#elif DOOR_GUARD_HW_VERSION == 11
    #define SCREEN_WIDTH 128
    #define SCREEN_HEIGHT 64
    #define SCREEN_WEMOS_OLED false
#else
#error "You need to define DOOR_GUARD_HW_VERSION"
#endif

#endif //DOOR_GUARD_PIN_LAYOUT_H
