#include "GameScreen.h"
#include "GameplayMenu.h"
#include "PlayerController.h"
#include "network/GameNetworkManager.h"
#include "WorldEntitiesController.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "app/game/controls/GamePadController.h"
#include "wnd_engine/Logger.h"

using namespace wnd;

GameScreen::GameScreen(App *app) : mApp(app),
                                   mWorldPresenter(createWorld(app)),
                                   mNetworkManager(new GameNetworkManager(app->getPlatform(), this)),
                                   mPlayerController(new PlayerController(mWorldPresenter, mNetworkManager)),
                                   mEntitiesController(new WorldEntitiesController(mWorldPresenter)),
                                   mGameplayMenu(createGameplayMenu(app)) {
}

void GameScreen::attach() {
    mApp->addLifecycleListener(this);

    mApp->attachView(mWorldPresenter->getView());
    mApp->attachView(mGameplayMenu->getView());

    mApp->registerAction(mWorldPresenter);

    for (int i = 0; i < 10; i++) {
        GameObject *obj = new GameObject(3);
        float size = 80;
        obj->setSize(size, size);
        obj->setLocation(size * i + size * 2, size * i + size * 2);
        View *view = new View();
        view->setSize(100, 100);
        view->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
            canvas->draw(canvas->newObject()
                                 ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                                 ->setRenderData(RenderData(RgbData(0, 100, 255))));
        }));
        GameObjectPresenter *objectPresenter = new GameObjectPresenter(obj, view);
        mWorldPresenter->addObjectPresenter(objectPresenter);
    }
}

void GameScreen::detach() {
    mApp->removeLifecycleListener(this);

    mWorldPresenter->stop();

    mApp->detachView(mWorldPresenter->getView());
    mApp->detachView(mGameplayMenu->getView());

    mNetworkManager->disconnect();
}

GameWorldPresenter *GameScreen::createWorld(App *app) {
    ViewGroup *worldView = new ViewGroup();
    worldView->setSize(app->getRenderSize());

    GameWorld *gameWorld = new GameWorld(Point<float>(8000, 8000));

    GameWorldPresenter *worldPresenter = new GameWorldPresenter(gameWorld, worldView, 0.8f);
    worldPresenter->setPausedOnAppPause(true);
    return worldPresenter;
}

GameplayMenu *GameScreen::createGameplayMenu(App *app) {
    ViewGroup *view = new ViewGroup();
    view->setSize(app->getRenderSize());
    return new GameplayMenu(view, mNetworkManager, this);
}

void GameScreen::onAppResumed() {
    mNetworkManager->connect();
}

void GameScreen::onAppPaused() {
    mNetworkManager->disconnect();
}

void GameScreen::onAppWindowSizeChanged(int oldWidth, int newWidth, int oldHeight, int newHeight) {
}

void GameScreen::onConnection(bool connected) {
}

void GameScreen::onGameObjectAdded(GameNetworkObjectState &state) {
    GameNetworkListener::onGameObjectAdded(state);
    if (state.getObjectId() != mNetworkManager->getPlayerServerObjectId()) {
        mEntitiesController->addObject(state);
    }
}

void GameScreen::onGameObjectRemoved(GameNetworkObjectState &state) {
    GameNetworkListener::onGameObjectRemoved(state);
    mEntitiesController->removeObject(state);
}

void GameScreen::onGameStateUpdated(const GameNetworkState &state) {
    for (const GameNetworkObjectState obj : state.getObjects()) {
        if (obj.getObjectId() == mNetworkManager->getPlayerServerObjectId()) {
            mPlayerController->update(state.getServerTime(), obj);
        } else {
            mEntitiesController->update(obj);
        }
    }
}

void GameScreen::onMove(int angle, int progress) {
    mPlayerController->onMove(angle, progress);
}

void GameScreen::onSkillON(int skillId) {
    mPlayerController->startSkill(skillId);
}

void GameScreen::onSkillOFF(int skillId) {
    mPlayerController->stopSkill(skillId);
}