#include "WorldEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/EntityState.h"
#include "wnd_engine/Logger.h"
#include "../AppConstants.h"


WorldEntityPresenter::WorldEntityPresenter(const EntityState &state) : GameObjectPresenter(createObject(state), createView(state)) {
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
    lastStates.push_back(state); // TODO Sync THREADS???
    if (lastStates.size() > 4) {
        lastStates.erase(lastStates.begin());
    }
}

GameObject *WorldEntityPresenter::createObject(const EntityState &state) {
    // Game object
    GameObject *object = new GameObject(state.getObjectType()); // TODO
    float y = state.getY();
    object->setCenterLocation(state.getX(), state.getY());
    object->setAngle(state.getAngle());
    object->setSize(state.getSize(), state.getSize());
    return object;
}

View *WorldEntityPresenter::createView(const EntityState &state) {
    View *objectView = new View();
    objectView->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
        WorldEntityPresenter *thisClass = static_cast<WorldEntityPresenter*>(customData);
        canvas->draw(canvas->newObject()
                             ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                             ->setRenderData(RenderData(RgbData(255, 0, 0))));
    }, this));
    return objectView;
}

void WorldEntityPresenter::step(std::uint64_t time) {
    GameObjectPresenter::step(time);

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
