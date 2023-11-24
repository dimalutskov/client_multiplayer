#pragma once

#include "GameplayMenuCallback.h"
#include "wnd_engine/game_v2/object/GameObjectListener.h"
#include "wnd_engine/game/action/ObjectMovementTargetAction.h"

using namespace wnd;

class GameWorldPresenter;
class GameObjectPresenter;
class GameNetworkManager;
class GameNetworkObjectState;

class PlayerController : GameObjectListener, GameplayMenuCallback {
private:
    GameWorldPresenter *const mWorldPresenter;

    GameObjectPresenter *mObjectPresenter;

    ObjectMovementTargetAction<float> *mMoveAction;

public:
    PlayerController(GameWorldPresenter *worldPresenter);

    void tempUpdate(GameNetworkObjectState state);

    void onMove(Point<int> viewLocation) override;

//    void onGameObjectBeforeDestroy(GameObject *obj) override ;
//    void onGameObjectDestroyed(GameObject *obj) override ;
    void onGameObjectCollision(GameObject *objA, GameObject *objB) override ;
    void onGameObjectCollisionEnd(GameObject *objA, GameObject *objB) override ;

};