#include "WorldEntitiesController.h"
#include "WorldEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/GameNetworkObjectState.h"
#include "GameConstants.h"

WorldEntitiesController::WorldEntitiesController(GameWorldPresenter *worldPresenter) : mWorldPresenter(worldPresenter) {

}

WorldEntityPresenter *WorldEntitiesController::addObject(const GameNetworkObjectState &state) {
    WorldEntityPresenter *objectPresenter = new WorldEntityPresenter(state);
    mEntities[state.getObjectId()] = objectPresenter;
    mWorldPresenter->addObjectPresenter(objectPresenter);
    return objectPresenter;
}

void WorldEntitiesController::removeObject(const GameNetworkObjectState &state) {
    auto entityRef = mEntities.find(state.getObjectId());
    if (entityRef != mEntities.end()) {
        entityRef->second->gameObject->destroy();
        mEntities.erase(entityRef);
    }
}

void WorldEntitiesController::update(const GameNetworkObjectState &state) {
    WorldEntityPresenter *objectPresenter = 0;
    auto entityRef = mEntities.find(state.getObjectId());
    if (entityRef == mEntities.end()) {
        objectPresenter = addObject(state);
    } else {
        objectPresenter = entityRef->second;
    }
    objectPresenter->update(state);
}