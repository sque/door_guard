//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#include "SongsLibrary.h"

bool SongsLibrary::add_song(song_id_t id, song_loader_t song_loader) {
    if (songs_by_id.count(id)) {
        return false; // The song already exists
    }
    songs_by_id[id] = LazySong(song_loader);
    return true;
}

LazySong * SongsLibrary::get_song(song_id_t id) {
    if (songs_by_id.count(id) == 0)
        return nullptr; // The song does not exist
    return &songs_by_id[id];
}

