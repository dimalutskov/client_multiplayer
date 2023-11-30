#pragma once

#include <string>
#include <vector>
#include "GameNetworkObjectState.h"

using namespace std;

class GameNetworkState {
private:
    std::uint64_t serverTime;
    vector<GameNetworkObjectState> objects;

public:
    GameNetworkState() {
        this->serverTime = 0;
    }

    GameNetworkState(vector<string> &messageSplits) {
        this->serverTime = stol(messageSplits[1]);

        for (int i = 2; i < messageSplits.size(); i++) {
            objects.push_back(GameNetworkObjectState(serverTime, messageSplits[i]));
        }
    }

    long getServerIteration() const {
        return serverTime;
    }

    const vector <GameNetworkObjectState> &getObjects() const {
        return objects;
    }

};