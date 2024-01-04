#include "PlayerController.h"
#include "network/GameNetworkManager.h"
#include "BaseEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "../AppConstants.h"
#include "wnd_engine/Logger.h"

PlayerController::PlayerController(GameWorldPresenter *worldPresenter, GameNetworkManager *networkManager)
    : AppActionGroup(APP_ACTION_GROUP_TOGETHER), mWorldPresenter(worldPresenter), mNetworkManager(networkManager) {
    speedSkill = false;

    mObjectPresenter = 0;
}

//void PlayerController::onGamePadMove(Point<int> viewLocation) {
//    if (mMoveAction && mMoveAction->getState() != AppActionState::FINISHED) {
//        mMoveAction->stop();
//        delete mMoveAction;
//    }
//
//    LayoutObject<float> *destination = new LayoutObject<float>(); // TODO keep
//    destination->setLocation(mWorldPresenter->getWorldLocation(viewLocation));
//
//    mMoveAction = new ObjectMovementTargetAction<float>(mObjectPresenter->gameObject, destination);
//    mMoveAction->setSpeed(60.0);
//    mMoveAction->setSpeedChangeValue(40.0);
//    mWorldPresenter->getWorld()->addAction(mMoveAction);
//}

void PlayerController::onGameObjectCollision(GameObject *objA, GameObject *objB) {
    if (mMoveAction) {
        mMoveAction->stop();
    }
    mObjectPresenter->gameObject->setCenterLocation(0, 0);
}

void PlayerController::onGameObjectCollisionEnd(GameObject *objA, GameObject *objB) {
}

void PlayerController::join(const EntityState &state) {
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

void PlayerController::update(const EntityState &state) {
}

void PlayerController::attachInfluence(EntityInfluence &influence) {
    mObjectPresenter->attachInfluence(influence);
}

void PlayerController::detachInfluence(EntityInfluence &influence) {
    mObjectPresenter->detachInfluence(influence);
}

void PlayerController::destroy() {
    mObjectPresenter->destroy(0);
}

void PlayerController::onGamePadMove(int angle, int progress) {
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

void PlayerController::startSkill(int skillId) {
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

void PlayerController::stopSkill(int skillId) {
    mNetworkManager->skillOFF(skillId);
    if (skillId == AppConstants::SKILL_TYPE_ACCELERATION) {
        speedSkill = false;
        // TODO Check if gamepad is active
        mMoveAction->setSpeed(AppConstants::MAX_SPEED);
    }
}

void PlayerController::onActionMove(float x, float y, float angle) {
    mObjectPresenter->gameObject->setCenterLocation(x, y);
    mObjectPresenter->gameObject->setAngle(angle);
    mNetworkManager->updatePlayerMovement(x, y, angle, mMoveAction->getSpeed());
}