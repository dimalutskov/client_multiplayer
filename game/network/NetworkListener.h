#pragma once

#include "../data/WorldState.h"
#include "../data/EntityInfluence.h"

class NetworkListener {
public:
    virtual void onConnection(bool connected) {}
    virtual void onJoinGame(EntityState &playerState) {}
    virtual void onGameEntityAdded(EntityState &state) {}
    virtual void onGameEntityDestroyed(EntityState &state) {}
    virtual void onAttachEntityInfluence(std::string entityId, EntityInfluence &influence) {}
    virtual void onDetachEntityInfluence(std::string entityId, EntityInfluence &influence) {}
    virtual void onGameStateUpdated(const WorldState &state) {}
};