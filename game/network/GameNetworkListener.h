#pragma once

#include "GameNetworkState.h"

class GameNetworkListener {
public:
    virtual void onConnection(bool connected) {}
    virtual void onGameStateUpdated(GameNetworkState state) {}
};