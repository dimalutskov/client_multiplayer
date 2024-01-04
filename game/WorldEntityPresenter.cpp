#include "WorldEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/EntityState.h"
#include "wnd_engine/Logger.h"
#include "../AppConstants.h"


WorldEntityPresenter::WorldEntityPresenter(const EntityState &state) : BaseEntityPresenter(state) {
    if (state.getObjectType() == AppConstants::ENTITY_TYPE_SHOT) {
        // As shots created on the server with client time(in past) - need to predict previous state
        int backAngle = MathUtils::validateAngle(state.getAngle() + 270);
        long time = 2000;
        uint64_t clientTime = state.getClientTime() - time; // TODO
        uint64_t serverTime = state.getServerTime() - time; // TODO
        float distance = AppConstants::SHOT_SPEED * (time / 1000.0f);
        int x = state.getX() - distance * cos(MathUtils::toRadians(backAngle));
        int y = state.getY() - distance * sin(MathUtils::toRadians(backAngle));
        lastStates.push_back(EntityState(clientTime, serverTime, state, x, y));
    }
    lastStates.push_back(state);
}

void WorldEntityPresenter::update(const EntityState &state) {
    BaseEntityPresenter::update(state);

    lastStates.push_back(state); // TODO Sync THREADS???
    if (lastStates.size() > 4) {
        lastStates.erase(lastStates.begin());
    }
}

void WorldEntityPresenter::step(std::uint64_t time) {
    BaseEntityPresenter::step(time);

    if (destroyTime > 0 && destroyTime < time) {
        gameObject->destroy();
        return;
    }

    EntityState *stateBefore = 0;
    EntityState *stateAfter = 0;

    for (auto it = lastStates.begin(); it != lastStates.end(); ++it) {
        EntityState &state = *it;
        if (state.getClientTime() < time) {
            stateBefore = &state;
        }
        if (!stateAfter && state.getClientTime() >= time) {
            stateAfter = &state;
        }
    }

    if (stateBefore != 0 && stateAfter != 0) {
        long timeFrame = stateAfter->getClientTime() - stateBefore->getClientTime();
        long passedTime = time - stateBefore->getClientTime();
        float progress = passedTime / (float) timeFrame;

        int x = stateBefore->getX() + (stateAfter->getX() - stateBefore->getX()) * progress;
        int y = stateBefore->getY() + (stateAfter->getY() - stateBefore->getY()) * progress;
        gameObject->setCenterLocation(x, y);
//        gameObject->setAngle(state.getAngle());
    }
    // TODO ELSE???

}
