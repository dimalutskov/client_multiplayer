#include "PlayerController.h"
#include "network/GameNetworkManager.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "GameConstants.h"

PlayerController::PlayerController(GameWorldPresenter *worldPresenter) : mWorldPresenter(worldPresenter) {
    lastServerUpdateIteration = 0;
    lastClientMoveIteration = 0;

    // Game object
    GameObject *playerObject = new GameObject(GameConstants::GAME_OBJECT_PLAYER);
    playerObject->setSize(100, 100);
    playerObject->addListener(this);

//    // Collisions
//    GameObjectCollisionHandler *playerCollisions = new GameObjectCollisionHandler(playerObject);
//    playerCollisions->addCollideType(GameConstants::GAME_OBJECT_OBSTACLE);
//    mWorldPresenter->getWorld()->registerCollisionHandler(playerCollisions);

    // View
    View *objectView = new View();
    objectView->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
        canvas->draw(canvas->newObject()
                             ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                             ->setRenderData(RenderData(RgbData(255, 0, 255))));
    }));

    mObjectPresenter = new GameObjectPresenter(playerObject, objectView);

    mWorldPresenter->addObjectPresenter(mObjectPresenter);
    mWorldPresenter->setCameraObject(mObjectPresenter->gameObject);

    mMoveAction = new ObjectMovementAction(playerObject->getX(), playerObject->getY(), playerObject->getAngle(), [this](float x, float y, float angle) {
        onActionMove(x, y, angle);
    });
    mWorldPresenter->getWorld()->addAction(mMoveAction);
}

//void PlayerController::onMove(Point<int> viewLocation) {
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
    mObjectPresenter->gameObject->setLocation(0, 0);
}

void PlayerController::onGameObjectCollisionEnd(GameObject *objA, GameObject *objB) {
}

void PlayerController::update(long serverIteration, const GameNetworkObjectState &state) {
    if (lastClientMoveIteration < lastServerUpdateIteration) {
        float x = state.getX();
        float y = state.getY();
        mMoveAction->update(x, y);
        mObjectPresenter->gameObject->setLocation(x, y);
    }
    lastServerUpdateIteration = lastClientMoveIteration;

}

void PlayerController::onMove(int angle, int progress) {
    mMoveAction->setAngle(angle);
    mMoveAction->setSpeed(GameConstants::MAX_SPEED * (progress / 100.0f));
}

void PlayerController::startSkill(int skillId) {

}

void PlayerController::stopSkill(int skillId) {

}

void PlayerController::onActionMove(float x, float y, float angle) {
    mObjectPresenter->gameObject->setLocation(x, y);
    mObjectPresenter->gameObject->setAngle(angle);
    lastClientMoveIteration = lastServerUpdateIteration;
}

