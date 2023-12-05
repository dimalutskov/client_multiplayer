#pragma once

#include "WorldState.h"

class NetworkListener {
public:
    virtual void onConnection(bool connected) {}
    virtual void onGameObjectAdded(ObjectState &state) {}
    virtual void onGameObjectRemoved(ObjectState &state) {}
    virtual void onGameStateUpdated(const WorldState &state) {}
};