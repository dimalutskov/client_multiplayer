#pragma once

#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game/action/ObjectMovementAction.h"
#include "network/EntityInfluence.h"

class EntityState;
class GameObject;

class BaseEntityPresenter : public GameObjectPresenter {
protected:
    int entityType;

    std::vector<EntityInfluence> pendingInfluences;
    std::vector<EntityInfluence> attachedInfluences;
    // key - influenceId
    std::unordered_map<std::string, ViewDrawer*> influenceDrawers;

    std::uint64_t destroyTime;

public:
    BaseEntityPresenter(const EntityState &state);

    void attachInfluence(EntityInfluence &influence);
    void detachInfluence(EntityInfluence &influence);

    virtual void update(const EntityState &state);

    void destroy(std::uint64_t clientWorldTime) {
        destroyTime = clientWorldTime;
    }

    virtual void step(std::uint64_t time) override;

protected:
    virtual GameObject *createObject(const EntityState &state);
    virtual View *createView(const EntityState &state);

    void onAttachInfluence(EntityInfluence &influence);
};