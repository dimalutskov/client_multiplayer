#pragma once

#include <engine/App.h>
#include <engine/render/opengl/OpenGLRenderer.h>
#include <engine/layout/view/ViewGroup.h>
#include <engine/platform/PlatformWebSocket.h>

#include "AppShaderHandler.h"
#include "game/network/NetworkProtocol.h"
#include "game/GameScreen.h"

class MyApp : public App, public WebSocketListener {
private:

    PlatformWebSocket *mWebSocket;

    ViewGroup *mRootView;

    View *mBtnConnect;


public:

    MyApp(Platform *platform) : App(platform) {
        mWebSocket = platform->addWebSocket(0, NetworkProtocol::SERVER_URL);
        mWebSocket->addListener(this);
    }

    virtual ~MyApp() {}

    void onAttachedToWindow(Renderer *renderer) override {
        App::onAttachedToWindow(renderer);

        // Init custom shader handler for additional custom shaders
        OpenGLRenderer *openGlRenderer = static_cast<OpenGLRenderer *>(renderer);
        openGlRenderer->setShaderHandler(new AppShaderHandler());

        mRootView = new ViewGroup();
        mRootView->setSize(renderer->getWindow()->getWidth(), renderer->getWindow()->getHeight());
        attachView(mRootView);

        mBtnConnect = new View();
        mBtnConnect->setSize(300, 100);

        mBtnConnect->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
            auto *thisClass = static_cast<MyApp*>(customData);
            auto color = thisClass->mWebSocket->isConnecting() ? RgbData(100, 100, 100) :
                    thisClass->mWebSocket->isConnected() ? RgbData(255, 0, 0) : RgbData(255, 255, 0);
            canvas->draw(canvas->newObject()
                                 ->setRenderData(RenderData(color)));
        }, this));

        mBtnConnect->setClickListener([this](View *view) {
            if (mWebSocket->isConnected()) {
                mWebSocket->disconnect();
            } else {
                mWebSocket->connect();
            }
            mBtnConnect->invalidate();
        });


        mRootView->subscribeSizeChanges([this](Point<int> oldSize, Point<int> newSize) {
            mBtnConnect->setLocation(mRootView->getWidth() / 2 - mBtnConnect->getWidth() / 2, 100);
        }, true);

        mRootView->addChild(mBtnConnect);
    }

    void onResume() override {}

    void onPause() override {}

    void onAppWindowSizeChanged(int oldWidth, int newWidth, int oldHeight, int newHeight) override {
        mRootView->setSize(newWidth, newHeight);
    }

    void onWebSocketConnected(int webSocketId) override {
        mBtnConnect->invalidate();
    }

    void onWebSocketDisconnected(int webSocketId) override {
        mBtnConnect->invalidate();
    }

    void onWebSocketError(int webSocketId, std::string errMsg) override {
        mBtnConnect->invalidate();
    }

};
