//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#ifndef DOOR_GUARD_SONG_H
#define DOOR_GUARD_SONG_H
#include <Arduino.h>

#define DEFAULT_TEMPO 120

class Song {
protected:
    const char * name;
    const int * notes;
    uint16_t total_notes;
    uint16_t tempo;
    int current_index;
    int whole_note_duration;

public:

    Song(const char * name, const int * notes, uint16_t total_notes, uint16_t tempo=DEFAULT_TEMPO);

    /**
     * Get the duration of the current note
     * @return
     */
    int get_duration()const;

    /**
     * Get the frequency of the current note
     * @return
     */
    int get_frequency()const;

    /**
     * Start iteration from the begining
     */
    void restart();

    /**
     * Go to next note
     */
     void next();

     int get_total_notes() const {
         return this->total_notes;
     }

     /**
      * Check if there are still notes for playing
      */
     bool is_finished() const;
};


#endif //DOOR_GUARD_SONG_H
