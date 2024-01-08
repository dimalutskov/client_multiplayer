#pragma once

#include <engine/App.h>
#include <engine/AppLifecycleListener.h>
#include "GameplayController.h"

class GameScreenCallback {
public:
    virtual void onGameScreenButtonClick() = 0;
};

class GameScreen : public AppLifecycleListener, GameplayCallback, NetworkListener {
private:
    App *const mApp;
    GameScreenCallback *const mCallback;
    GameNetworkManager *const mNetworkManager;

    ViewGroup *mRootView;
    GameplayController *mGameplayController;

public:
    GameScreen(App *app, PlatformWebSocket *webSocket, GameScreenCallback *callback)
        : mApp(app), mCallback(callback), mNetworkManager(new GameNetworkManager(app->getPlatform(), webSocket)) {
        mRootView = new ViewGroup();
        mRootView->setSize(app->getRenderSize());

        mGameplayController = new GameplayController(mRootView, mNetworkManager, this);

        mNetworkManager->subscribe(this); // TODO
    }

    void attach() {
        mApp->addLifecycleListener(this);
        mApp->attachView(mRootView);

        mGameplayController->show();
    }

    void detach() {
        mApp->removeLifecycleListener(this);
        mApp->detachView(mRootView);

        mGameplayController->hide();
    }

    virtual void onConnection(bool connected) override {

    }

    virtual void onJoinGame(EntityState &playerState) override {
//        mGameplayMenu->showPlayerControls();
//        mPlayerController->join(playerState);
    }


//    virtual void onButtonClick(int buttonId) override {
//        if (buttonId == GameplayMenuCallback::BTN_ID_PLAY) {
//            mCallback->onGameScreenButtonClick();
//        }
//    }


private:

};