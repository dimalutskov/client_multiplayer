#pragma once

#include <wnd_engine/App.h>
#include <wnd_engine/AppLifecycleListener.h>
#include <wnd_engine/layout/view/ViewGroup.h>
#include "GameplayMenuCallback.h"
#include "network/NetworkListener.h"

using namespace wnd;

class GameWorldPresenter;
class PlayerController;
class GameplayMenu;
class GameNetworkManager;
class WorldEntitiesController;

class GameScreen : public AppLifecycleListener, GameplayMenuCallback, NetworkListener {
private:
    App *const mApp;
    GameWorldPresenter *const mWorldPresenter;
    GameNetworkManager *const mNetworkManager;
    PlayerController *const mPlayerController;
    WorldEntitiesController *const mEntitiesController;

    GameplayMenu *const mGameplayMenu;

public:
    GameScreen(App *app);

    void attach();
    void detach();

    virtual void onAppResumed() override;
    virtual void onAppPaused() override;
    virtual void onAppWindowSizeChanged(int oldWidth, int newWidth, int oldHeight, int newHeight) override;

    virtual void onConnection(bool connected) override;
    virtual void onGameEntityAdded(EntityState &state) override;
    virtual void onGameEntityDestroyed(EntityState &state) override;
    virtual void onGameStateUpdated(const WorldState &state) override;

    virtual void onGamePadMove(int angle, int progress) override;
    virtual void onSkillON(int skillId) override;
    virtual void onSkillOFF(int skillId) override;

private:
    ViewGroup *initSpaceBackground(Point<int> size, int starsCount);

    GameWorldPresenter *createWorld(App *app);
    GameplayMenu *createGameplayMenu(App *app);

};