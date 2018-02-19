#pragma once

#include "./Game.h"

class Game;

class Sample {
    protected:
        Game &game;

    public:
        Sample(Game &_game) : game(_game) {}
        virtual ~Sample() {}

        virtual void init() {};
        virtual void update(float /*elapsed_seconds*/) {};
        virtual void render() {};
        virtual void resize() {};
};
