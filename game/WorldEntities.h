#pragma once

#include <string>
#include <unordered_map>
#include "WorldEntityPresenter.h"

class WorldEntities {
private:
    GameWorldPresenter *const mWorldPresenter;

    // key - networkObjectId
    std::unordered_map<std::string, WorldEntityPresenter*> mEntities;

public:
    WorldEntities(GameWorldPresenter *worldPresenter) : mWorldPresenter(worldPresenter) {}

    WorldEntityPresenter *addObject(const EntityState &state) {
        WorldEntityPresenter *objectPresenter = new WorldEntityPresenter(state);
        mEntities[state.getObjectId()] = objectPresenter;
        mWorldPresenter->addObjectPresenter(objectPresenter);
        return objectPresenter;
    }

    void destroyObject(const EntityState &state) {
        auto entityRef = mEntities.find(state.getObjectId());
        if (entityRef != mEntities.end()) {
            entityRef->second->update(state);
            entityRef->second->destroy(state.getClientTime());
            mEntities.erase(entityRef);
        }
    }

    void update(const EntityState &state) {
        WorldEntityPresenter *objectPresenter = 0;
        auto entityRef = mEntities.find(state.getObjectId());
        if (entityRef == mEntities.end()) {
            objectPresenter = addObject(state);
        } else {
            objectPresenter = entityRef->second;
        }
        objectPresenter->update(state);
    }

    void attachInfluence(std::string entityId, EntityInfluence &influence) {
        auto entityRef = mEntities.find(entityId);
        if (entityRef != mEntities.end()) {
            entityRef->second->attachInfluence(influence);
        }
    }

    void detachInfluence(std::string entityId, EntityInfluence &influence) {
        auto entityRef = mEntities.find(entityId);
        if (entityRef != mEntities.end()) {
            entityRef->second->detachInfluence(influence);
        }
    }

};