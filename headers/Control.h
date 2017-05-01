#pragma once

class Control {
  public:
    bool up;
    bool down;
    bool left;
    bool right;
    bool play;

    Control() :
      up(false),
      down(false),
      left(false),
      right(false),
      play(true)
    {};
};
