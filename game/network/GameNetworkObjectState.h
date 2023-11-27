#pragma once

#include "wnd_engine/utils/CollectionUtils.h"

using namespace std;

class GameNetworkObjectState {
private:
    std::string stateString;
    std::string objectId;
    std::string objectType;
    int health;
    int energy;
    int x;
    int y;
    int angle;

public:
    GameNetworkObjectState(std::string &stateString) : stateString(stateString) {
        vector<string> stateSplits;
        wnd::CollectionUtils::split(stateSplits, stateString, ',');
        objectId = stateSplits[0];
        objectType = stateSplits[1];
        health = std::stoi(stateSplits[2]);
        energy = std::stoi(stateSplits[3]);
        x = std::stoi(stateSplits[4]);
        y = std::stoi(stateSplits[5]);
        angle = std::stoi(stateSplits[6]);
//        int speed = std::stoi(stateSplits[7]);
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

    int getHealth() const {
        return health;
    }

    int getEnergy() const {
        return energy;
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