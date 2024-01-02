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
#include "../resources/AppRes.h"

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

    // Bg temp
    Point<int> renderSize = mApp->getRenderSize();
    RenderScreen *backgroundScreen = mApp->createRenderScreen(renderSize.getX(), renderSize.getY());
    ViewGroup *backgroundView = initSpaceBackground(renderSize, 50);
    backgroundView->setRenderScreen(backgroundScreen);
    mApp->attachView(backgroundView);

    mApp->attachView(mWorldPresenter->getView());
    mApp->attachView(mGameplayMenu->getView());

    mApp->registerAction(mWorldPresenter);

    for (int i = 0; i < 10; i++) {
        GameObject *obj = new GameObject(3);
        float size = 80;
        obj->setSize(size, size);
        obj->setCenterLocation(size * i + size * 2, size * i + size * 2);
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

void GameScreen::onJoinGame(EntityState &playerState) {
    mGameplayMenu->showPlayerControls();

    mPlayerController->join(playerState);
}

void GameScreen::onConnection(bool connected) {
}

void GameScreen::onGameEntityAdded(EntityState &state) {
    NetworkListener::onGameEntityAdded(state);
    if (state.getObjectId() != mNetworkManager->getPlayerServerObjectId()) {
        mEntitiesController->addObject(state);
    }
}

void GameScreen::onGameEntityDestroyed(EntityState &state) {
    if (state.getObjectId() == mNetworkManager->getPlayerServerObjectId()) {
        mPlayerController->destroy();
    } else {
        NetworkListener::onGameEntityDestroyed(state);
        mEntitiesController->destroyObject(state);
    }
}

void GameScreen::onGameStateUpdated(const WorldState &state) {
    for (const EntityState &obj : state.getObjects()) {
        if (obj.getObjectId() == mNetworkManager->getPlayerServerObjectId()) {
            mPlayerController->update(obj);
            mGameplayMenu->update(obj);
        } else {
//            if (!mNetworkManager->isPlayerSkillObject(obj.getObjectId())) { // TODO keep for testing
                mEntitiesController->update(obj);
//            }
        }
    }
}

void GameScreen::onButtonClick(int buttonId) {
    if (buttonId == GameplayMenuCallback::BTN_ID_PLAY) {
        mNetworkManager->join();
    }
}

void GameScreen::onGamePadMove(int angle, int progress) {
    mPlayerController->onGamePadMove(angle, progress);
}

void GameScreen::onSkillON(int skillId) {
    mPlayerController->startSkill(skillId);
}

void GameScreen::onSkillOFF(int skillId) {
    mPlayerController->stopSkill(skillId);
}

ViewGroup *GameScreen::initSpaceBackground(Point<int> size, int starsCount) {
    ViewGroup *result = new ViewGroup();
    result->setSize(size);

    int windowWidth = result->getWidth();
    int windowHeight = result->getHeight();
    for (int i = 0; i < starsCount; i++) {
        View *circle = new View();
        int size = MathUtils::randomInt(4, 10) * APP_RES.dimens()->DENSITY;
        circle->setSize(size, size);

        int x, y;
        int yStep = windowHeight / starsCount;
//            if (i < stars / 2) {
        x = MathUtils::randomInt(0, windowWidth);
        y = i * yStep;
//            } else {
//                x = i * 45;
//                y = MathUtils::randomInt(0, windowHeight);
//            }

//            circle->setLocation(windowWidth * MathUtils::randRatio(), windowHeight * MathUtils::randRatio());
        circle->setLocation(x, y);
        circle->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
            float alpha = MathUtils::randomInt(60, 210) / 255.0f;
            canvas->draw(canvas->newObject()
                                 ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                                 ->setRenderData(RenderData(RgbData(255, 255, 255), alpha)));
        }));
        result->addChild(circle);
    }
    return result;
}