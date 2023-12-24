#pragma once

#include "wnd_engine/utils/CollectionUtils.h"

using namespace std;

class EntityState {
public:
    static const int POS_ID = 0;
    static const int POS_TYPE = 1;
    static const int POS_SIZE = 2;
    static const int POS_X = 3;
    static const int POS_Y = 4;
    static const int POS_ANGLE = 5;
    static const int POS_HEALTH = 6;
    static const int POS_ENERGY = 7;

private:
    // Client world time time related to server time
    uint64_t clientWorldTime;
    uint64_t serverTime;

    vector<string> stateSplits;

public:
    EntityState(uint64_t clientTime, uint64_t serverTime, const EntityState &state, int x, int y)
            : clientWorldTime(clientTime), serverTime(serverTime){
        for (string stateSplit : state.stateSplits) {
            this->stateSplits.push_back(stateSplit);
        }
        this->stateSplits[POS_X] = to_string(x);
        this->stateSplits[POS_Y] = to_string(y);
    }

    EntityState(uint64_t clientTime, uint64_t serverTime, string &stateString)
            : clientWorldTime(clientTime), serverTime(serverTime) {
        wnd::CollectionUtils::split(this->stateSplits, stateString, ',');
    }

    const uint64_t &getClientTime() const {
        return clientWorldTime;
    }

    const uint64_t &getServerTime() const {
        return serverTime;
    }

    const string &getObjectId() const {
        return stateSplits[POS_ID];
    }

    const int getObjectType() const {
        return stoi(stateSplits[POS_TYPE]);
    }

    int getSize() const {
        return stoi(stateSplits[POS_SIZE]);
    }

    int getX() const {
        return stoi(stateSplits[POS_X]);
    }

    int getY() const {
        return stoi(stateSplits[POS_Y]);
    }

    int getAngle() const {
        return stoi(stateSplits[POS_ANGLE]);
    }

    string get(int position) const {
        return stateSplits[position];
    }
};