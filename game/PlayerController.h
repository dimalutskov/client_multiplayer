#pragma once

#include <wnd_engine/action/AppActionGroup.h>
#include "wnd_engine/game_v2/object/GameObjectListener.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

using namespace wnd;

class GameWorldPresenter;
class GameObjectPresenter;
class GameNetworkManager;
class EntityState;
class GameNetworkManager;

class PlayerController : public AppActionGroup, public GameObjectListener {
private:
    GameWorldPresenter *const mWorldPresenter;
    GameNetworkManager *const mNetworkManager;

    GameObjectPresenter *mObjectPresenter;

    ObjectMovementAction *mMoveAction;

    bool speedSkill;

public:
    PlayerController(GameWorldPresenter *worldPresenter, GameNetworkManager *networkManager);

    void update(long serverIteration, const EntityState &state);

    void onGamePadMove(int angle, int progress);
    void startSkill(int skillId);
    void stopSkill(int skillId);

//    void onGameObjectBeforeDestroy(GameObject *obj) override ;
//    void onGameEntityDestroyed(GameObject *obj) override ;
    void onGameObjectCollision(GameObject *objA, GameObject *objB) override ;
    void onGameObjectCollisionEnd(GameObject *objA, GameObject *objB) override ;

    virtual void step(std::uint64_t time) override;

private:

    void onActionMove(float x, float y, float angle);

};