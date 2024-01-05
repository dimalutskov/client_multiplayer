#pragma once

#include <vector>
#include "engine/utils/CollectionUtils.h"

using namespace std;

class EntityInfluence {
private:
    uint64_t attachClientWorldTime;
    string id;
    int type;
    int skillType;
    string ownerId;
    vector<int> values;

public:
    EntityInfluence(std::uint64_t clientWorldTime, string &stateString) {
        vector<string> stateSplits;
        CollectionUtils::split(stateSplits, stateString, ',');
        this->attachClientWorldTime = clientWorldTime;
        this->type = stoi(stateSplits[0]);
        this->skillType = stoi(stateSplits[1]);
        this->ownerId = stateSplits[2];
        for (int i = 3; i < stateSplits.size(); i++) {
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

    uint64_t getAttachClientWorldTime() const {
        return attachClientWorldTime;
    }

    const string &getId() const {
        return id;
    }

    int getType() const {
        return type;
    }

    int getSkillType() const {
        return skillType;
    }

    const string &getOwnerId() const {
        return ownerId;
    }

    const vector<int> &getValues() const {
        return values;
    }

};