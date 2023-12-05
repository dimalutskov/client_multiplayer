#pragma once

#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

class ObjectState;
class GameObject;

class WorldEntityPresenter : public GameObjectPresenter {
private:
    std::vector<ObjectState> lastStates;

public:
    WorldEntityPresenter(const ObjectState &state);

    void update(const ObjectState &state);

    virtual void step(std::uint64_t time) override;

private:
    GameObject *createObject(const ObjectState &state);
    View *createView(const ObjectState &state);
};