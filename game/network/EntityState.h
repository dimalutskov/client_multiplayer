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
    vector<string> stateSplits;

    uint64_t clientTime;
    uint64_t serverTime;

public:
    EntityState(uint64_t clientTime, uint64_t serverTime, const EntityState &state, int x, int y) {
        for (string stateSplit : state.stateSplits) {
            this->stateSplits.push_back(stateSplit);
        }
        stateSplits[POS_X] = to_string(x);
        stateSplits[POS_Y] = to_string(y);
    }

    EntityState(uint64_t clientTime, uint64_t serverTime, string &stateString)
    : clientTime(clientTime), serverTime(serverTime) {
        wnd::CollectionUtils::split(this->stateSplits, stateString, ',');
    }

    const uint64_t &getClientTime() const {
        return clientTime;
    }

    const uint64_t &getServerTime() const {
        return serverTime;
    }

    const string &getObjectId() const {
        return stateSplits[POS_ID];
    }

    const int &getObjectType() const {
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