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
        IMPERIAL_MARCH,
        SUPER_MARIO,
        THE_LIONS_SLEEP_TONIGHT,
        TETRIS,
        NOKIA_RINGTONE,
        PACMAN,
        HARRY_POTTER,
        DOOM,
        UGLY_ALARM,
        PERSISTENT_BEEP,
        LOCKED_BEEP
    };
};


#endif //DOOR_GUARD_DEFAULTSONGSLIBRARY_H
