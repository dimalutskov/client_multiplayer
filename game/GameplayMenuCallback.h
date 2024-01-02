#pragma once

#include "wnd_engine/layout/Point.h"

using namespace wnd;

class GameplayMenuCallback {
public:
    static const int BTN_ID_PLAY = 1;

    virtual void onButtonClick(int buttonId) {}
    virtual void onGamePadMove(int angle, int progress) {}
    virtual void onSkillON(int skillId) {}
    virtual void onSkillOFF(int skillId) {}
};