#pragma once

#include <string>
#include <vector>
#include "GameNetworkObjectState.h"

using namespace std;

class GameNetworkState {
private:
    uint64_t clientTime;
    uint64_t serverTime;
    vector<GameNetworkObjectState> objects;

public:
    GameNetworkState() {
        this->clientTime = 0;
        this->serverTime = 0;
    }

    GameNetworkState(uint64_t clientTime, vector<string> &messageSplits) {
        this->clientTime = clientTime;
        this->serverTime = stol(messageSplits[1]);
        for (int i = 2; i < messageSplits.size(); i++) {
            objects.push_back(GameNetworkObjectState(serverTime, messageSplits[i]));
        }
    }

    uint64_t getClientIteration() const {
        return clientTime;
    }

    uint64_t getServerTime() const {
        return serverTime;
    }

    const vector <GameNetworkObjectState> &getObjects() const {
        return objects;
    }

};