#pragma once

#include <string>
#include <vector>
#include "ObjectState.h"

using namespace std;

class WorldState {
private:
    uint64_t clientTime; // time when state received
    uint64_t clientWorldTime; // calculated client time to show this world state
    uint64_t serverWorldTime; // server time when this state was composed (received from server)
    vector<ObjectState> objects;

public:
    WorldState() {
        this->clientTime = 0;
        this->clientWorldTime = 0;
        this->serverWorldTime = 0;
    }

    WorldState(uint64_t clientTime, uint64_t clientWorldTime, vector<string> &messageSplits) {
        this->clientTime = clientTime;
        this->clientWorldTime = clientWorldTime;
        this->serverWorldTime = stol(messageSplits[1]);
        for (int i = 2; i < messageSplits.size(); i++) {
            objects.push_back(ObjectState(clientWorldTime, serverWorldTime, messageSplits[i]));
        }
    }

    const uint64_t &getClientTime() const {
        return clientTime;
    }

    const uint64_t getClientWorldTime() const {
        return clientWorldTime;
    }

    const uint64_t getServerTime() const {
        return serverWorldTime;
    }

    const vector <ObjectState> &getObjects() const {
        return objects;
    }

};