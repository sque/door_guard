//
// Created by Konstantinos Paliouras on 16/06/2022.
//

#ifndef DOOR_GUARD_SONGSLIBRARY_H
#define DOOR_GUARD_SONGSLIBRARY_H

#include <map>
#include "Song.h"

typedef uint8_t song_id_t;
typedef std::map<song_id_t, Song *> songs_map_t;

class SongsLibrary {
protected:
    songs_map_t songs_by_id;
public:

    typedef songs_map_t::const_iterator  const_iterator;

    /**
     * Get a song by id
     * @param id A unique id of the song
     * @return The found song or null
     */
    Song * get_song(song_id_t id);

    /**
     * Add a new song in the library
     * @param id A unique id of the song
     * @param p_song A pointer to the song
     * @return True if it was added succesfully
     */
    bool add_song(song_id_t id, Song * p_song);

    const_iterator begin() const {
        return songs_by_id.begin();
    }

    const_iterator end() const {
        return songs_by_id.end();
    }
};


#endif //DOOR_GUARD_SONGSLIBRARY_H
