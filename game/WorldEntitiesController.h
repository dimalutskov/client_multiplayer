#pragma once

#include <string>
#include <unordered_map>

class WorldEntityPresenter;
class GameWorldPresenter;
class GameNetworkObjectState;

class WorldEntitiesController {
private:
    GameWorldPresenter *const mWorldPresenter;

    // key - networkObjectId
    std::unordered_map<std::string, WorldEntityPresenter*> mEntities;

public:
    WorldEntitiesController(GameWorldPresenter *worldPresenter);

    void addObject(const GameNetworkObjectState &state);
    void removeObject(const GameNetworkObjectState &state);
    void update(const GameNetworkObjectState &state);

};