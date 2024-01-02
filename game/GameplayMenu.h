#pragma once

#include "GameplayMenuCallback.h"
#include "wnd_engine/layout/view/ViewGroup.h"
#include "app/game/controls/GamePadController.h"
#include "wnd_engine/layout/view/text/TextDrawer.h"

using namespace wnd;

class GameNetworkManager;
class EntityState;

class GameplayMenu : GamePadListener {
private:

    ViewGroup *const view;
    GameNetworkManager *const mNetworkManager;
    GameplayMenuCallback *const mCallback;
    GamePadController *gamePadController;

    std::list<View*> mSkillViews;
    // key - viewId, value - skill type(shot type)
    std::unordered_map<int, int> mSkillIds;
    // key - pointerId, value - touched view
    std::unordered_map<int, View*> mSkillTouchPointers;

    TextDrawer *mHealthTextDrawer;
    TextDrawer *mEnergyTextDrawer;

public:

    GameplayMenu(ViewGroup *rootView, GameNetworkManager *networkManager, GameplayMenuCallback *callback);

    ViewGroup *getView() {return view;}

    virtual void onShowGamePad(GamePadView *view, float x, float y);
    virtual void onHideGamePad(GamePadView *view);
    virtual void onUpdateGamePad(GamePadView *view, int angle, int progress);

    void showPlayerControls();
    void update(const EntityState &playerState);

private:
    void initMenu();
    void initSkills();
    void initPropertyDrawers();
    void handleTouchEvent(TouchEvent event, TouchEvent downEvent);
};