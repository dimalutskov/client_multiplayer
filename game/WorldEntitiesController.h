#pragma once

#include <string>
#include <unordered_map>

class WorldEntityPresenter;
class GameWorldPresenter;
class ObjectState;

class WorldEntitiesController {
private:
    GameWorldPresenter *const mWorldPresenter;

    // key - networkObjectId
    std::unordered_map<std::string, WorldEntityPresenter*> mEntities;

public:
    WorldEntitiesController(GameWorldPresenter *worldPresenter);

    WorldEntityPresenter *addObject(const ObjectState &state);
    void removeObject(const ObjectState &state);
    void update(const ObjectState &state);

};