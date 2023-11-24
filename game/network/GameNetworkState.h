#pragma once

#include <string>
#include <vector>
#include "GameNetworkObjectState.h"

using namespace std;

class GameNetworkState {
private:
    long serverIteration;
    vector<GameNetworkObjectState> objects;

public:
    GameNetworkState() {
        this->serverIteration = 0;
    }

    GameNetworkState(vector<string> &messageSplits) {
        this->serverIteration = stol(messageSplits[1]);

        for (int i = 2; i < messageSplits.size(); i++) {
            objects.push_back(GameNetworkObjectState(messageSplits[i]));
        }
    }

    long getServerIteration() const {
        return serverIteration;
    }

    const vector <GameNetworkObjectState> &getObjects() const {
        return objects;
    }

};