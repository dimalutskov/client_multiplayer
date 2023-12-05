#pragma once

#include <string>
#include <vector>

#include <wnd_engine/utils/CollectionUtils.h>

using namespace std;

class PlayerInfo {
public:
    int health;
    int energy;
    int maxSpeed;
    int shotSpeed;

    PlayerInfo(string stringInfo) {
        vector<string> splits;
        CollectionUtils::split(splits, stringInfo, ',');
        health = stoi(splits[0]);
        energy = stoi(splits[1]);
        maxSpeed = stoi(splits[2]);
        shotSpeed = stoi(splits[3]);
    }
};