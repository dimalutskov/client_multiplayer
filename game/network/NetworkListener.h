#pragma once

#include "WorldState.h"

class NetworkListener {
public:
    virtual void onConnection(bool connected) {}
    virtual void onGameEntityAdded(EntityState &state) {}
    virtual void onGameEntityDestroyed(EntityState &state) {}
    virtual void onGameStateUpdated(const WorldState &state) {}
};