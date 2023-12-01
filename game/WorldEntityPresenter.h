#pragma once

#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

class GameNetworkObjectState;
class GameObject;

class WorldEntityPresenter : public GameObjectPresenter {
private:
    std::vector<GameNetworkObjectState> lastStates;

public:
    WorldEntityPresenter(const GameNetworkObjectState &state);

    void update(const GameNetworkObjectState &state);

    virtual void step(std::uint64_t time) override;

private:
    GameObject *createObject(const GameNetworkObjectState &state);
    View *createView(const GameNetworkObjectState &state);
};