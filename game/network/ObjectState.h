#pragma once

#include "wnd_engine/utils/CollectionUtils.h"

using namespace std;

class ObjectState {
private:
    std::uint64_t clientTime;
    std::uint64_t serverTime;
    std::string stateString; // original state
    std::string objectId;
    int objectType;
    int x;
    int y;
    int angle;

public:
    ObjectState(std::uint64_t clientTime, std::uint64_t serverTime, std::string &stateString)
    : clientTime(clientTime), serverTime(serverTime), stateString(stateString) {
        vector<string> stateSplits;
        wnd::CollectionUtils::split(stateSplits, stateString, ',');
        objectId = stateSplits[0];
        objectType = std::stoi(stateSplits[1]);
        x = std::stoi(stateSplits[2]);
        y = std::stoi(stateSplits[3]);
        angle = std::stoi(stateSplits[4]);
    }

    const std::uint64_t &getClientTime() const {
        return clientTime;
    }

    const std::uint64_t &getServerTime() const {
        return serverTime;
    }

    const std::string &getStateString() const {
        return stateString;
    }

    const std::string &getObjectId() const {
        return objectId;
    }

    const int &getObjectType() const {
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