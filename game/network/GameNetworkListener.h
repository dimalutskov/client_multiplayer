#pragma once

#include "GameNetworkState.h"

class GameNetworkListener {
public:
    virtual void onConnection(bool connected) {}
    virtual void onPlayerConnection(std::string serverObjectId) {}
    virtual void onGameStateUpdated(const GameNetworkState &state) {}
};