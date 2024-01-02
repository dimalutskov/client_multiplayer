#pragma once

#include <wnd_engine/action/AppActionGroup.h>
#include "wnd_engine/game_v2/object/GameObjectListener.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

using namespace wnd;

class GameWorldPresenter;
class BaseEntityPresenter;
class GameNetworkManager;
class EntityState;
class EntityInfluence;
class GameNetworkManager;

class PlayerController : public AppActionGroup, public GameObjectListener {
private:
    GameWorldPresenter *const mWorldPresenter;
    GameNetworkManager *const mNetworkManager;

    BaseEntityPresenter *mObjectPresenter;

    ObjectMovementAction *mMoveAction;

    bool speedSkill;

public:
    PlayerController(GameWorldPresenter *worldPresenter, GameNetworkManager *networkManager);

    void join(const EntityState &state);
    void update(const EntityState &state);
    void attachInfluence(EntityInfluence &influence);
    void detachInfluence(EntityInfluence &influence);
    void destroy();

    void onGamePadMove(int angle, int progress);
    void startSkill(int skillId);
    void stopSkill(int skillId);

//    void onGameObjectBeforeDestroy(GameObject *obj) override ;
//    void onGameEntityDestroyed(GameObject *obj) override ;
    void onGameObjectCollision(GameObject *objA, GameObject *objB) override ;
    void onGameObjectCollisionEnd(GameObject *objA, GameObject *objB) override ;

private:

    void onActionMove(float x, float y, float angle);

};