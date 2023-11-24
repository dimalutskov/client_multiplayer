#include "MyApp.h"
#include "game/GameScreen.h"

using namespace wnd;

MyApp::MyApp(Platform *platform) : App(platform) {

}

MyApp::~MyApp() {

}

void MyApp::onAttachedToWindow(Renderer *renderer) {
    App::onAttachedToWindow(renderer);

    GameScreen *gameScreen = new GameScreen(this);
    gameScreen->attach();
}

void MyApp::onResume() {
    App::onResume();
}

void MyApp::onPause() {
    App::onPause();
}

