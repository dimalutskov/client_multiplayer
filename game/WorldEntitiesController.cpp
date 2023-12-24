#include "WorldEntitiesController.h"
#include "WorldEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/EntityState.h"
#include "../AppConstants.h"

WorldEntitiesController::WorldEntitiesController(GameWorldPresenter *worldPresenter) : mWorldPresenter(worldPresenter) {

}

WorldEntityPresenter *WorldEntitiesController::addObject(const EntityState &state) {
    WorldEntityPresenter *objectPresenter = new WorldEntityPresenter(state);
    mEntities[state.getObjectId()] = objectPresenter;
    mWorldPresenter->addObjectPresenter(objectPresenter);
    return objectPresenter;
}

void WorldEntitiesController::destroyObject(const EntityState &state) {
    auto entityRef = mEntities.find(state.getObjectId());
    if (entityRef != mEntities.end()) {
        entityRef->second->update(state);
        entityRef->second->destroy(state.getClientTime());
        mEntities.erase(entityRef);
    }
}

void WorldEntitiesController::update(const EntityState &state) {
    WorldEntityPresenter *objectPresenter = 0;
    auto entityRef = mEntities.find(state.getObjectId());
    if (entityRef == mEntities.end()) {
        objectPresenter = addObject(state);
    } else {
        objectPresenter = entityRef->second;
    }
    objectPresenter->update(state);
}