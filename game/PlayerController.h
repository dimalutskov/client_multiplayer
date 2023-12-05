#pragma once

#include <wnd_engine/action/AppActionGroup.h>
#include "wnd_engine/game_v2/object/GameObjectListener.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

using namespace wnd;

class GameWorldPresenter;
class GameObjectPresenter;
class GameNetworkManager;
class ObjectState;
class GameNetworkManager;

class PlayerController : public AppActionGroup, public GameObjectListener {
private:
    GameWorldPresenter *const mWorldPresenter;
    GameNetworkManager *const mNetworkManager;

    GameObjectPresenter *mObjectPresenter;

    ObjectMovementAction *mMoveAction;

    /**
     * Used to sync client move and server state updates to apply "server-side-prediction"
     */
    long lastServerUpdateIteration;
    long lastClientMoveIteration;

public:
    PlayerController(GameWorldPresenter *worldPresenter, GameNetworkManager *networkManager);

    void update(long serverIteration, const ObjectState &state);

    void onGamePadMove(int angle, int progress);
    void startSkill(int skillId);
    void stopSkill(int skillId);

//    void onGameObjectBeforeDestroy(GameObject *obj) override ;
//    void onGameObjectDestroyed(GameObject *obj) override ;
    void onGameObjectCollision(GameObject *objA, GameObject *objB) override ;
    void onGameObjectCollisionEnd(GameObject *objA, GameObject *objB) override ;

    virtual void step(std::uint64_t time) override;

private:

    void onActionMove(float x, float y, float angle);

};