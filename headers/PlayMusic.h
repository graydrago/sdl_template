#pragma once
#include <string>

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif

class PlayMusic {
    private:
        Mix_Chunk *music;
        std::string m_file_path;

    public:
        PlayMusic() : music(nullptr) {};
        virtual ~PlayMusic() { Mix_FreeChunk(music); }

        void load(std::string file_path);
        void play();
        //void stop() {};
};
