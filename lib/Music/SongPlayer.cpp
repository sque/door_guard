//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#include "SongPlayer.h"


SongPlayer::SongPlayer(uint8_t buzzer_pin, Song *song, float breath_ratio):
        buzzer_pin(buzzer_pin),
        next_note_play_time(0),
        song(nullptr),
        breath_ratio(breath_ratio),
        auto_restart(false)
{
    if (song) {
        play(song);
    };
}


void SongPlayer::refresh() {
    unsigned int current_time = millis();

    if (song == nullptr){
        return; // Nothing to play here
    }

    if (current_time < next_note_play_time) {
        return; // Still nothing to do, let's idle
    }

    if (song->is_finished()) {
        next_note_play_time = 0;
        if (auto_restart) {
            song->restart();
        } else {
            return; // Song has finished
        }
    }

    // Play the tone
    int duration = song->get_duration();
    noTone(buzzer_pin);
    tone(buzzer_pin, song->get_frequency(), duration * (1.0 - breath_ratio));

    if (next_note_play_time) {
        // Adjust time to keep stable rythm instead of playing all the notes correctly
        next_note_play_time = next_note_play_time + duration;
    } else {
        // On the first note we don't have something to follow.
        next_note_play_time = current_time + duration;
    }


    // Move to next note
    song->next();
}

void SongPlayer::play(Song * new_song) {
    song = new_song;
    new_song->restart(); // Restart song from the beginning.
    next_note_play_time = 0;
}

void SongPlayer::stop() {
    song = nullptr;
    next_note_play_time = 0;
    noTone(buzzer_pin);
}
