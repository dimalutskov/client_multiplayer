#pragma once

#include "WorldState.h"
#include "EntityInfluence.h"

class NetworkListener {
public:
    virtual void onConnection(bool connected) = 0;
    virtual void onJoinGame(EntityState &playerState) = 0;
    virtual void onGameEntityAdded(EntityState &state) = 0;
    virtual void onGameEntityDestroyed(EntityState &state) = 0;
    virtual void onAttachEntityInfluence(std::string entityId, EntityInfluence &influence) = 0;
    virtual void onDetachEntityInfluence(std::string entityId, EntityInfluence &influence) = 0;
    virtual void onGameStateUpdated(const WorldState &state) = 0;
};