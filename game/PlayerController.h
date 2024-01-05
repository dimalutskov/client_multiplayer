#pragma once

#include <engine/action/AppActionGroup.h>
#include <engine/game_v2/object/GameObjectListener.h>
#include <engine/game_v2/action/ObjectMovementAction.h>
#include "BaseEntityPresenter.h"
#include "network/GameNetworkManager.h"

class GameNetworkManager;

class PlayerController : public AppActionGroup, public GameObjectListener {
private:
    GameWorldPresenter *const mWorldPresenter;
    GameNetworkManager *const mNetworkManager;

    BaseEntityPresenter *mObjectPresenter;

    ObjectMovementAction *mMoveAction;

    bool speedSkill;

public:
    PlayerController(GameWorldPresenter *worldPresenter, GameNetworkManager *networkManager)
        : AppActionGroup(APP_ACTION_GROUP_TOGETHER), mWorldPresenter(worldPresenter), mNetworkManager(networkManager) {
        speedSkill = false;
        mObjectPresenter = 0;
    }

    void join(const EntityState &state) {
        mObjectPresenter = new BaseEntityPresenter(state);

        mWorldPresenter->addObjectPresenter(mObjectPresenter);
        mWorldPresenter->setCameraObject(mObjectPresenter->gameObject);

        mMoveAction = new ObjectMovementAction(mObjectPresenter->gameObject->getCX(), mObjectPresenter->gameObject->getCY(), mObjectPresenter->gameObject->getAngle(),
                                               [this](std::uint64_t time, float x, float y, float angle) {
                                                   onActionMove(x, y, angle);
                                               });
        mObjectPresenter->addAction(mMoveAction);
        mObjectPresenter->addAction(this);
    }

    void update(const EntityState &state) {}

    void attachInfluence(EntityInfluence &influence) {
        mObjectPresenter->attachInfluence(influence);
    }

    void detachInfluence(EntityInfluence &influence) {
        mObjectPresenter->detachInfluence(influence);
    }

    void destroy() {
        mObjectPresenter->destroy(0);
    }

    // GamePad callbacks
    void onGamePadMove(int angle, int progress) {
        if (mWorldPresenter == 0) {
            return;
        }
        mMoveAction->setAngle(angle);
        int speed = speedSkill ? AppConstants::MAX_SPEED * 2 : AppConstants::MAX_SPEED * (progress / 100.0f);
        mMoveAction->setSpeed(speed);
        if (progress == 0) {
            GameObject *obj = mObjectPresenter->gameObject;
            mObjectPresenter->gameObject->getAngle();
            mNetworkManager->updatePlayerMovement(obj->getCX(), obj->getCY(), obj->getAngle(), mMoveAction->getSpeed());
        }
    }
    void startSkill(int skillId) {
        GameObject *playerObj = mObjectPresenter->gameObject;
        if (skillId == AppConstants::SKILL_TYPE_SHOT) {
            // Game object
            auto *obj = new GameObject(AppConstants::ENTITY_TYPE_SPACESHIP); // TODO
            obj->setSize(AppConstants::SHOT_SIZE, AppConstants::SHOT_SIZE);
            obj->setCenterLocation(playerObj->getCX(), playerObj->getCY());
            obj->setAngle(playerObj->getAngle());

            // View
            View *view = new View();
            view->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
                canvas->draw(canvas->newObject()
                                     ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                                     ->setAngle(canvas->getAngle())
                                     ->setRenderData(RenderData(RgbData(0, 0, 140))));
            }));

            auto *moveAction = new ObjectMovementAction(obj->getCX(), obj->getCY(), obj->getAngle(),
                                                        [obj](std::uint64_t time, float x, float y, float angle) {
                                                            obj->setCenterLocation(x, y);
                                                            obj->setAngle(time);
                                                        });
            moveAction->setSpeed(AppConstants::SHOT_SPEED); // TODO

            mWorldPresenter->addObjectPresenter(new GameObjectPresenter(obj, view));
            mWorldPresenter->getWorld()->addAction(moveAction);
        } else if (skillId == AppConstants::SKILL_TYPE_ACCELERATION) {
            mMoveAction->setSpeed(AppConstants::MAX_SPEED * 2);
            speedSkill = true;
        }

        mNetworkManager->skillON(skillId, playerObj->getCX(), playerObj->getCY(), playerObj->getAngle());
    }

    void stopSkill(int skillId) {
        mNetworkManager->skillOFF(skillId);
        if (skillId == AppConstants::SKILL_TYPE_ACCELERATION) {
            speedSkill = false;
            // TODO Check if gamepad is active
            mMoveAction->setSpeed(AppConstants::MAX_SPEED);
        }
    }

private:

    void onActionMove(float x, float y, float angle) {
        mObjectPresenter->gameObject->setCenterLocation(x, y);
        mObjectPresenter->gameObject->setAngle(angle);
        mNetworkManager->updatePlayerMovement(x, y, angle, mMoveAction->getSpeed());
    }

};