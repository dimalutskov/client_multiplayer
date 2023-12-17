#pragma once

#include "WorldState.h"

class NetworkListener {
public:
    virtual void onConnection(bool connected) {}
    virtual void onGameObjectAdded(EntityState &state) {}
    virtual void onGameObjectRemoved(EntityState &state) {}
    virtual void onGameStateUpdated(const WorldState &state) {}
};