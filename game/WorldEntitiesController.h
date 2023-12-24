#pragma once

#include <string>
#include <unordered_map>

class WorldEntityPresenter;
class GameWorldPresenter;
class EntityState;

class WorldEntitiesController {
private:
    GameWorldPresenter *const mWorldPresenter;

    // key - networkObjectId
    std::unordered_map<std::string, WorldEntityPresenter*> mEntities;

public:
    WorldEntitiesController(GameWorldPresenter *worldPresenter);

    WorldEntityPresenter *addObject(const EntityState &state);
    void destroyObject(const EntityState &state);
    void update(const EntityState &state);

};