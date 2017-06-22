#include <iostream>

#include "../headers/PlayMusic.h"


void PlayMusic::load(std::string file_path) {
    m_file_path = file_path;
    music = Mix_LoadWAV(m_file_path.c_str());
    if (!music) {
        std::cerr << ("Can't load a music: " + m_file_path) << std::endl;
    }
}


void PlayMusic::play() {
    if (music) {
        if (Mix_PlayChannel(-1, music, -1) < 0) {
            std::cerr << "SDL_Mixer Error:" << Mix_GetError() << std::endl;
        }
    } else {
        std::cerr << "Can't find an instance of Mix_Chunk" << std::endl;
    }
}

