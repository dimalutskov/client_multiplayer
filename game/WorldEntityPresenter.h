#pragma once

#include "BaseEntityPresenter.h"

class EntityState;
class GameObject;

class WorldEntityPresenter : public BaseEntityPresenter {
private:
    std::vector<EntityState> lastStates;

public:
    WorldEntityPresenter(const EntityState &state);

    virtual void update(const EntityState &state) override;

    virtual void step(std::uint64_t time) override;

};