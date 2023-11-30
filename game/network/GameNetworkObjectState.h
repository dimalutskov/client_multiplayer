#pragma once

#include "wnd_engine/utils/CollectionUtils.h"

using namespace std;

class GameNetworkObjectState {
private:
    std::uint64_t time;
    std::string stateString; // original state
    std::string objectId;
    std::string objectType;
    int x;
    int y;
    int angle;

public:
    GameNetworkObjectState(std::uint64_t time, std::string &stateString) : time(time), stateString(stateString) {
        vector<string> stateSplits;
        wnd::CollectionUtils::split(stateSplits, stateString, ',');
        objectId = stateSplits[0];
        objectType = stateSplits[1];
        x = std::stoi(stateSplits[2]);
        y = std::stoi(stateSplits[3]);
        angle = std::stoi(stateSplits[4]);
    }

    const std::string &getStateString() const {
        return stateString;
    }

    const std::string &getObjectId() const {
        return objectId;
    }

    const std::string &getObjectType() const {
        return objectType;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    int getAngle() const {
        return angle;
    }
};