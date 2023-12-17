#pragma once

#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"

class EntityState;
class GameObject;

class WorldEntityPresenter : public GameObjectPresenter {
private:
    std::vector<EntityState> lastStates;

public:
    WorldEntityPresenter(const EntityState &state);

    void update(const EntityState &state);

    virtual void step(std::uint64_t time) override;

private:
    GameObject *createObject(const EntityState &state);
    View *createView(const EntityState &state);
};