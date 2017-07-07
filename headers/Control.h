#pragma once

class Control {
  public:
    bool up;
    bool down;
    bool left;
    bool right;
    int zoom;
    int xrel;
    int yrel;
    bool play;
    bool fire;

    const int zoom_min = -15;
    const int zoom_max = 15;

    Control() :
        up(false),
        down(false),
        left(false),
        right(false),
        zoom(0),
        xrel(0),
        yrel(0),
        play(true),
        fire(false)
    {};
};
