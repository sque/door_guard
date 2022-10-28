//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#ifndef DOOR_GUARD_SONGSLIBRARY_H
#define DOOR_GUARD_SONGSLIBRARY_H

#include <map>
#include "Song.h"

typedef Song *(*song_loader_t)();

/**
 * A wrapper for songs that will load song lazily
 */
class LazySong {
protected:
    Song *pSong;
    song_loader_t songLoader;

public:

    LazySong() :
            pSong(nullptr),
            songLoader(nullptr) {}

    LazySong(song_loader_t song_loader) :
            songLoader(song_loader) {
    }

    LazySong(const LazySong &other) = default;

    LazySong &operator=(const LazySong &other) = default;


    /**
     * Load or return the loaded song from memory
     * @return
     */
    Song *load() {
        if (pSong == NULL) {
            pSong = songLoader();
        }

        return pSong;
    }

};


typedef uint8_t song_id_t;
typedef std::map<song_id_t, LazySong> songs_map_t;


class SongsLibrary {
protected:
    songs_map_t songs_by_id;
public:

    typedef songs_map_t::const_iterator const_iterator;

    /**
     * Get a song by id
     * @param id A unique id of the song
     * @return The found song or null
     */
    LazySong *get_song(song_id_t id);

    /**
     * Add a new song in the library
     * @param id A unique id of the song
     * @param song_loader A function that loads the song
     * @return True if it was added successfully
     */
    bool add_song(song_id_t id, song_loader_t song_loader);

    /**
     * Iterator interface to loop over songs
     * @return
     */
    const_iterator begin() const {
        return songs_by_id.begin();
    }

    /**
     * Iterator interface to loop over songs
     * @return
     */
    const_iterator end() const {
        return songs_by_id.end();
    }
};


#endif //DOOR_GUARD_SONGSLIBRARY_H
