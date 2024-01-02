#pragma once

#include <vector>
#include "wnd_engine/utils/CollectionUtils.h"

using namespace std;

class EntityInfluence {
private:
    string id;
    int type;
    uint64_t attachTime;
    int skillType;
    string ownerId;
    vector<int> values;

public:
    EntityInfluence(string &stateString) {
        vector<string> stateSplits;
        wnd::CollectionUtils::split(stateSplits, stateString, ',');
        this->type = stoi(stateSplits[0]);
        this->attachTime = stol(stateSplits[1]);
        this->skillType = stoi(stateSplits[2]);
        this->ownerId = stoi(stateSplits[3]);
        for (int i = 4; i < stateSplits.size(); i++) {
            this->values.push_back(stoi(stateSplits[i]));
        }

        ostringstream ss;
        ss << type << "_" << skillType << "_" << ownerId;
        this->id = ss.str();
    }

    bool operator == (const EntityInfluence& other) {
        return (this->id == other.id);
    }

    bool operator != (const EntityInfluence& other) {
        return (this->id != other.id);
    }

};