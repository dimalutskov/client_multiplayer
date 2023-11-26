#pragma once

#include "wnd_engine/layout/Point.h"

using namespace wnd;

class GameplayMenuCallback {
public:
    virtual void onMove(int angle, int progress) {}
    virtual void onSkillON(int skillId) {}
    virtual void onSkillOFF(int skillId) {}
};