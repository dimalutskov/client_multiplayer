#pragma once

#include <engine/App.h>
#include <engine/AppLifecycleListener.h>
#include <engine/layout/view/ViewGroup.h>
#include "network/GameNetworkManager.h"
#include "GameplayMenu.h"
#include "GameplayMenuCallback.h"
#include "PlayerController.h"
#include "WorldEntities.h"

class GameScreenCallback {
public:
    virtual void onGameScreenButtonClick() = 0;
};

class GameScreen : public AppLifecycleListener, GameplayMenuCallback, NetworkListener {
private:
    App *const mApp;
    GameScreenCallback *const mCallback;

    GameWorldPresenter *const mWorldPresenter;
    GameNetworkManager *const mNetworkManager;
    PlayerController *const mPlayerController;
    WorldEntities *const mEntitiesController;

    GameplayMenu *const mGameplayMenu;

    ViewGroup *mBackgroundView;

public:
    GameScreen(App *app, GameScreenCallback *callback) : mApp(app), mCallback(callback),
            mWorldPresenter(createWorld(app)),
            mNetworkManager(new GameNetworkManager(app->getPlatform(), this)),
            mPlayerController(new PlayerController(mWorldPresenter, mNetworkManager)),
            mEntitiesController(new WorldEntities(mWorldPresenter)),
            mGameplayMenu(createGameplayMenu(app)) {
    }

    void attach() {
        mApp->addLifecycleListener(this);

        // Bg temp
        Point<int> renderSize = mApp->getRenderSize();
        RenderScreen *backgroundScreen = mApp->createRenderScreen(renderSize.getX(), renderSize.getY());
        mBackgroundView = initSpaceBackground(renderSize, 50);
        mBackgroundView->setRenderScreen(backgroundScreen);
        mApp->attachView(mBackgroundView);

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

    void detach() {
        mApp->removeLifecycleListener(this);

        mWorldPresenter->stop();

        mApp->detachView(mBackgroundView);
        mApp->detachView(mWorldPresenter->getView());
        mApp->detachView(mGameplayMenu->getView());

        mNetworkManager->disconnect();
    }

    virtual void onConnection(bool connected) override {

    }

    virtual void onJoinGame(EntityState &playerState) override {
        mGameplayMenu->showPlayerControls();
        mPlayerController->join(playerState);
    }

    virtual void onGameEntityAdded(EntityState &state) override {
        if (state.getObjectId() != mNetworkManager->getPlayerServerObjectId()) {
            mEntitiesController->addObject(state);
        }
    }

    virtual void onGameEntityDestroyed(EntityState &state) override {
        if (state.getObjectId() == mNetworkManager->getPlayerServerObjectId()) {
            mPlayerController->destroy();
        } else {
            mEntitiesController->destroyObject(state);
        }
    }

    virtual void onAttachEntityInfluence(std::string entityId, EntityInfluence &influence) override {
        if (entityId == mNetworkManager->getPlayerServerObjectId()) {
            mPlayerController->attachInfluence(influence);
        } else {
            mEntitiesController->attachInfluence(entityId, influence);
        }
    }

    virtual void onDetachEntityInfluence(std::string entityId, EntityInfluence &influence) override {
        if (entityId == mNetworkManager->getPlayerServerObjectId()) {
            mPlayerController->detachInfluence(influence);
        } else {
            mEntitiesController->detachInfluence(entityId, influence);
        }
    }

    virtual void onGameStateUpdated(const WorldState &state) override {
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

    virtual void onButtonClick(int buttonId) override {
        if (buttonId == GameplayMenuCallback::BTN_ID_PLAY) {
            mCallback->onGameScreenButtonClick();
        }
    }

    virtual void onGamePadMove(int angle, int progress) override {
        mPlayerController->onGamePadMove(angle, progress);
    }

    virtual void onSkillON(int skillId) override {
        mPlayerController->startSkill(skillId);
    }

    virtual void onSkillOFF(int skillId) override {
        mPlayerController->stopSkill(skillId);
    }

private:
    ViewGroup *initSpaceBackground(Point<int> size, int starsCount) {
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

    GameWorldPresenter *createWorld(App *app) {
        ViewGroup *worldView = new ViewGroup();
        worldView->setSize(app->getRenderSize());

        GameWorld *gameWorld = new GameWorld(Point<float>(8000, 8000));

        GameWorldPresenter *worldPresenter = new GameWorldPresenter(gameWorld, worldView, 0.8f);
        worldPresenter->setPausedOnAppPause(true);
        return worldPresenter;
    }

    GameplayMenu *createGameplayMenu(App *app) {
        ViewGroup *view = new ViewGroup();
        view->setSize(app->getRenderSize());
        return new GameplayMenu(view, mNetworkManager, this);
    }

};