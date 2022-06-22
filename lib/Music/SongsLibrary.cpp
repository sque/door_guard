//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#include "SongsLibrary.h"

bool SongsLibrary::add_song(song_id_t id, Song *p_song) {
    if (songs_by_id.count(id)) {
        return false; // The song already exists
    }
    songs_by_id[id] = p_song;
    return true;
}

Song *SongsLibrary::get_song(song_id_t id) {
    if (songs_by_id.count(id) == 0)
        return nullptr; // The song does not exist
    return songs_by_id[id];
}

