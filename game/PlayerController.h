#pragma once

#include "wnd_engine/game_v2/object/GameObjectListener.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

using namespace wnd;

class GameWorldPresenter;
class GameObjectPresenter;
class GameNetworkManager;
class GameNetworkObjectState;

class PlayerController : GameObjectListener {
private:
    GameWorldPresenter *const mWorldPresenter;

    GameObjectPresenter *mObjectPresenter;

    ObjectMovementAction *mMoveAction;

    /**
     * Used to sync client move and server state updates to apply "server-side-prediction"
     */
    long lastServerUpdateIteration;
    long lastClientMoveIteration;

public:
    PlayerController(GameWorldPresenter *worldPresenter);

    void update(long serverIteration, const GameNetworkObjectState &state);

    void onMove(int angle, int progress);
    void startSkill(int skillId);
    void stopSkill(int skillId);

//    void onGameObjectBeforeDestroy(GameObject *obj) override ;
//    void onGameObjectDestroyed(GameObject *obj) override ;
    void onGameObjectCollision(GameObject *objA, GameObject *objB) override ;
    void onGameObjectCollisionEnd(GameObject *objA, GameObject *objB) override ;

private:

    void onActionMove(float x, float y, float angle);

};