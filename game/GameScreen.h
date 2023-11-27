#pragma once

#include <wnd_engine/App.h>
#include <wnd_engine/AppLifecycleListener.h>
#include "GameplayMenuCallback.h"
#include "network/GameNetworkListener.h"

using namespace wnd;

class GameWorldPresenter;
class PlayerController;
class GameplayMenu;
class GameNetworkManager;
class WorldEntitiesController;

class GameScreen : public AppLifecycleListener, GameplayMenuCallback, GameNetworkListener {
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
    virtual void onGameObjectAdded(GameNetworkObjectState &state) override;
    virtual void onGameObjectRemoved(GameNetworkObjectState &state) override;
    virtual void onGameStateUpdated(const GameNetworkState &state) override;

    virtual void onMove(int angle, int progress) override;
    virtual void onSkillON(int skillId) override;
    virtual void onSkillOFF(int skillId) override;

private:
    GameWorldPresenter *createWorld(App *app);
    GameplayMenu *createGameplayMenu(App *app);

};