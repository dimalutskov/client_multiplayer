#pragma once

#include <string>
#include <unordered_map>

class GameWorldPresenter;
class GameObjectPresenter;
class GameNetworkObjectState;

class WorldEntitiesController {
private:
    GameWorldPresenter *const mWorldPresenter;

    // key - networkObjectId
    std::unordered_map<std::string, GameObjectPresenter*> mEntities;

public:
    WorldEntitiesController(GameWorldPresenter *worldPresenter);

    void addObject(const GameNetworkObjectState &state);
    void removeObject(const GameNetworkObjectState &state);
    void update(const GameNetworkObjectState &state);

private:

    GameObjectPresenter *createObject();
};