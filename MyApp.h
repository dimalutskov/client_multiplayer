#pragma once

#include <engine/App.h>
#include <engine/render/opengl/OpenGLRenderer.h>
#include <engine/layout/view/ViewGroup.h>
#include <engine/platform/PlatformWebSocket.h>

#include "AppShaderHandler.h"
#include "game/network/NetworkProtocol.h"
#include "MainMenuScreen.h"
#include "game/GameScreen.h"


class MyApp : public App,
        public WebSocketListener,
        public MainMenuScreenCallback,
        public GameScreenCallback {
private:

    PlatformWebSocket *mWebSocket;

    MainMenuScreen *mMainMenuScreen;
    GameScreen *mGameScreen;

public:

    MyApp(Platform *platform) : App(platform) {
        mWebSocket = platform->addWebSocket(0, NetworkProtocol::SERVER_URL);
        mWebSocket->addListener(this);

        mMainMenuScreen = 0;
        mGameScreen = 0;
    }

    virtual ~MyApp() {}

    void onAttachedToWindow(Renderer *renderer) override {
        App::onAttachedToWindow(renderer);

        // Init custom shader handler for additional custom shaders
        OpenGLRenderer *openGlRenderer = static_cast<OpenGLRenderer *>(renderer);
        openGlRenderer->setShaderHandler(new AppShaderHandler());

        mMainMenuScreen = new MainMenuScreen(this, this);
        mMainMenuScreen->attach();
    }

    void onResume() override {}

    void onPause() override {}

    void onAppWindowSizeChanged(int oldWidth, int newWidth, int oldHeight, int newHeight) override {
//        mRootView->setSize(newWidth, newHeight);
    }

    void onMainScreenButtonClick() override {
        mMainMenuScreen->detach();

        if (mGameScreen == 0) {
            mGameScreen = new GameScreen(this, this);
        }
        mGameScreen->attach();
    }

    virtual void onGameScreenButtonClick() override {
        mGameScreen->detach();
        mMainMenuScreen->attach();
    }

    void onWebSocketConnected(int webSocketId) override {
    }

    void onWebSocketDisconnected(int webSocketId) override {
    }

    void onWebSocketError(int webSocketId, std::string errMsg) override {
    }

};
