#pragma once

#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

class GameNetworkObjectState;
class GameObject;

class WorldEntityPresenter : public GameObjectPresenter {
private:
    ObjectMovementAction *mMoveAction;

public:
    WorldEntityPresenter(const GameNetworkObjectState &state);

private:
    GameObject *createObject(const GameNetworkObjectState &state);
    View *createView(const GameNetworkObjectState &state);

    void onActionMove(std::uint64_t time, float x, float y, float angle);
};