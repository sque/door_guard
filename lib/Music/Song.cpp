//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#include "Song.h"

Song::Song(const char *name, const int *notes, uint16_t total_notes, uint16_t tempo) :
        name(name),
        notes(notes),
        total_notes(total_notes),
        tempo(tempo),
        current_index(0) {
    // this calculates the duration of a whole note in ms
    whole_note_duration = (60000 * 4) / this->tempo;
}


bool Song::is_finished() const {
    return current_index >= total_notes;
}

void Song::restart() {
    current_index = 0;

}

void Song::next() {
    current_index++;

}

int Song::get_frequency() const {
    return notes[current_index * 2];
}

int Song::get_duration() const {
    int divider = notes[(current_index * 2) + 1];
    int noteDuration = (whole_note_duration) / abs(divider);
    if (divider < 0) {
        noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    return noteDuration;
}
