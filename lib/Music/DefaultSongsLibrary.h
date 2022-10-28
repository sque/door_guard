//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#ifndef DOOR_GUARD_DEFAULTSONGSLIBRARY_H
#define DOOR_GUARD_DEFAULTSONGSLIBRARY_H

#include "SongsLibrary.h"

class DefaultSongsLibrary: public SongsLibrary {
public:
    DefaultSongsLibrary();

    enum Songs {
        IMPERIAL_MARCH=0,
        SUPER_MARIO=1,
        THE_LIONS_SLEEP_TONIGHT=2,
        TETRIS=3,
        NOKIA_RINGTONE=4,
        PACMAN=5,
        HARRY_POTTER=6,
        DOOM=7,
        UGLY_ALARM=8,
        PERSISTENT_BEEP=9,
        LOCKED_BEEP=10
    };
};


#endif //DOOR_GUARD_DEFAULTSONGSLIBRARY_H
