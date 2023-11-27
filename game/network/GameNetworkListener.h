#pragma once

#include "GameNetworkState.h"

class GameNetworkListener {
public:
    virtual void onConnection(bool connected) {}
    virtual void onGameObjectAdded(GameNetworkObjectState &state) {}
    virtual void onGameObjectRemoved(GameNetworkObjectState &state) {}
    virtual void onGameStateUpdated(const GameNetworkState &state) {}
};