//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#ifndef DOOR_GUARD_SONGPLAYER_H
#define DOOR_GUARD_SONGPLAYER_H

#include <Song.h>

# define DEFAULT_BREATH_RATIO 0.1

class SongPlayer {
protected:
    Song * song = nullptr;
    uint8_t buzzer_pin;
    unsigned long next_note_play_time;
    float breath_ratio;
    bool auto_restart;
public:

    SongPlayer(uint8_t buzzer_pin, Song * song = nullptr, float breath_ratio = DEFAULT_BREATH_RATIO);

    /**
     * Add this to the main loop so that song_player can play in the back ground
     */
    void refresh();

    /**
     * Ask song_player to restart when it reached the end of the song.
     * @param _auto_restart
     */
    void set_auto_restart(bool _auto_restart) {
        auto_restart = _auto_restart;
    }

    /**
     * Play a new song from the begining
     * @param song The song to play
     */
    void play(Song * song);

    /**
     * Stop playing this or any other song
     */
    void stop();


};


#endif //DOOR_GUARD_SONGPLAYER_H
