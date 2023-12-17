#pragma once

#include <wnd_engine/App.h>
#include <wnd_engine/render/opengl/OpenGLRenderer.h>
#include "AppShaderHandler.h"
#include "game/GameScreen.h"

using namespace wnd;

class MyApp : public App {
public:

    MyApp(Platform *platform) : App(platform) {}

    virtual ~MyApp() {}

    void onAttachedToWindow(Renderer *renderer) override {
        App::onAttachedToWindow(renderer);

        // Init custom shader handler for additional custom shaders
        OpenGLRenderer *openGlRenderer = static_cast<OpenGLRenderer *>(renderer);
        openGlRenderer->setShaderHandler(new AppShaderHandler());

        GameScreen *gameScreen = new GameScreen(this);
        gameScreen->attach();
    }

    void onResume() override {}

    void onPause() override {}

};
