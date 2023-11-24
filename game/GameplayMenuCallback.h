#pragma once

#include "wnd_engine/layout/Point.h"

using namespace wnd;

class GameplayMenuCallback {
public:
    virtual void onMove(Point<int> viewLocation) {}
};