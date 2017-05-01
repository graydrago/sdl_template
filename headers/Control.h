#pragma once

class Control {
  public:
    bool up;
    bool down;
    bool left;
    bool right;
    int zoom;
    bool play;

    const int zoom_min = -15;
    const int zoom_max = 15;

    Control() :
      up(false),
      down(false),
      left(false),
      right(false),
      zoom(0),
      play(true)
    {};
};
